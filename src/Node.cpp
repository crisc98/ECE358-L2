#include "Node.hpp"

#include <algorithm>

/**
 * Attempts to transmit the current frame at the front of the frame queue;
 * this is _after_ the channel has already been senses as being idle.
 */
void Node::attemptTransmission(
	Seconds attemptTime,
	NetworkSimulator *simulator
)
{
	++simulator->totalTransmissionAttempts;

	/**
	 * Notify all other nodes that this node has started transmitting its frame,
	 * taking into account the propagation delays.
	 */
	Seconds processingDelay = 0; // should we want to take this account
	Seconds transmissionStartTime = attemptTime + processingDelay;
	startTransmission(transmissionStartTime, simulator);

	/**
	 * Register a tentative event for the default time at which the node will stop
	 * transmitting; the node's transmission may be interrupted before its completion.
	 */
	Frame currentFrame = peekFrame();
	Seconds transmissionDelay = channel->getTransmissionDelay(currentFrame.length);
	Seconds transmissionStopTime = transmissionStartTime + transmissionDelay;
	currentTransmissionStopEvent = new TransmissionStopEvent(transmissionStopTime, this);
	simulator->addEvent(currentTransmissionStopEvent);
}

/**
 * Generates the ChannelBusyStartEvent instances for all the nodes this node is
 * connected to on the channel, in effect telling them all when the first bit
 * of its transmission has arrived at and started asserting their location on
 * the channel.
 */
void Node::startTransmission(
	Seconds transmissionStartTime,
	NetworkSimulator *simulator
)
{
	isCurrentlyTransmitting = true;
	for (ChannelConnection *connection : connections)
	{
		Seconds transmissionStartArrivalTime = transmissionStartTime + connection->channelPropagationDelay;
		ChannelBusyStartEvent channelBusyStartEvent = new ChannelBusyStartEvent(
			transmissionStartArrivalTime,
			connection->target);
		simulator->addEvent(channelBusyStartEvent);
	}
}

/**
 * Generates the ChannelBusyStopEvent instances for all the nodes this node is
 * connected to on the channel, in effect telling them all when the last bit
 * of its transmission has arrived at and stopped asserting their location on
 * the channel.
 */
void Node::stopTransmission(
	Seconds transmissionStopTime,
	NetworkSimulator *simulator
)
{
	isCurrentlyTransmitting = false;
	for (ChannelConnection *connection : connections)
	{
		Seconds transmissionStopArrivalTime = transmissionStopTime + connection->channelPropagationDelay;
		ChannelBusyStopEvent channelBusyStopEvent = new ChannelBusyStopEvent(
			transmissionStopArrivalTime,
			connection->target);
		simulator->addEvent(channelBusyStopEvent);
	}
}

/**
 * Removes the frame that was last attempted to be transmitted from the frame queue.
 */
void Node::popFrame(
	Seconds transmissionStopTime,
	NetworkSimulator *simulator
)
{
	frames.pop();

	if (hasFrames())
	{
		/**
		 * Register a transmission attempt event for the next frame.
		 * If the next frame arrived _before_ the current frame stopped being transmitted,
		 * we must wait until the transmission stop time before attempting to transmit the
		 * next frame.
		 */
		Frame currentFrame = node->peekFrame();
		Seconds transmissionAttemptTime = std::max(currentFrame.arrivalTime, transmissionStopTime);
		TransmissionAttemptEvent transmissionAttemptEvent = new TransmissionAttemptEvent(transmissionAttemptTime, node);
		simulator->addEvent(transmissionAttemptEvent);
	}
}

/**
 * Updates the simulation state in accordance with how a particular MAC protocol
 * should respond when an incoming starts asserting itself at the accepting
 * node's location on the channel.
 * 
 * By default increments the number of incoming signals.
 * 
 * This is an implementation of the Template Method Pattern; see
 * https://en.wikipedia.org/wiki/Template_method_pattern.
 */
void Node::acceptChannelBusyStartEvent(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	++node->numIncomingSignals;
	acceptChannelBusyStartEventImplementation(eventArrivalTime, simulator);
}

/**
 * Updates the simulation state in accordance with how a particular MAC protocol
 * should respond when an incoming signal stops asserting itself at the accepting
 * node's location on the channel.
 * 
 * By default decrements the number of incoming signals.
 * 
 * This is an implementation of the Template Method Pattern; see
 * https://en.wikipedia.org/wiki/Template_method_pattern.
 */
void Node::acceptChannelBusyStopEvent(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	--node->numIncomingSignals;
	acceptChannelBusyStopEventImplementation(eventArrivalTime, simulator);
}

/**
 * Updates the simulation state in accordance with how a particular MAC protocol
 * should respond when it reaches the point in time at which it should attempt to
 * transmit a frame.
 * 
 * This is an implementation of the Template Method Pattern; see
 * https://en.wikipedia.org/wiki/Template_method_pattern.
 */
void Node::acceptTransmissionAttemptEvent(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	bool shouldTransmit = shouldTransmit(eventArrivalTime, simulator);
	if (shouldTransmit) attemptTransmission(eventArrivalTime, simulator);
}

/**
 * Updates the simulation state in accordance with how a particular MAC protocol
 * should respond when it reaches the point in time at which it should stop or
 * finish transmitting.
 * 
 * This is an implementation of the Template Method Pattern; see
 * https://en.wikipedia.org/wiki/Template_method_pattern.
 */
void Node::acceptTransmissionStopEvent(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	/**
	 * Notify all other nodes that this node has finished transmitting its frame,
	 * taking into account the propagation delays.
	 */
	Seconds processingDelay = 0; // should we want to take this account
	Seconds transmissionStopTime = eventArrivalTime + processingDelay;
	stopTransmission(transmissionStopTime, simulator);

	currentTransmissionStopEvent = nullptr;

	// update network statistics
	Frame transmittedFrame = frames.front();
	simulator->totalTransmittedBits += transmittedFrame.length;
	++simulator->totalTransmittedFrames;
	
	// remove the frame from the queue now that it has been fully transmitted
	popFrame();
	
	acceptTransmissionStopEventImplementation(eventArrivalTime, simulator);
}

/**
 * Adds a frame to this node's frame queue. It is assumed that frames will be
 * added in order such that the frame with the smallest timestamp is always
 * the first frame to be transmitted or dropped.
 */
void Node::addFrame(Frame frame)
{
	frames.push(frame);
	++totalFrames;
}

/**
 * Returns true if there are frames left in the queue to be transmitted.
 */
bool Node::hasFrames()
{
	bool result = !frames.empty();
	return result;
}

/**
 * Peeks the frame at the front of the frame queue.
 */
Frame Node::peekFrame()
{
	Frame top = frames.front();
	return top;
}

/**
 * Returns true if the node has detected that the channel is busy,
 * namely that there are signals incoming from other nodes.
 */
bool Node::channelIsBusy()
{
	bool isBusy = numIncomingSignals > 0;
	return isBusy;
}