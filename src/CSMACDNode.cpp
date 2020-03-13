#include "CSMACDNode.hpp"

/**
 * The CSMA/CD MAC protocol implementation for collision detection.
 */
void CSMACDNode::acceptChannelBusyStartEventImplementation(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	if (isCurrentlyTransmitting)
	{
		// a collision was detected

		/**
		 * Notify all other nodes that this node has stopped transmitting its frame,
		 * taking into account the propagation delays.
		 */
		Seconds processingDelay = 0; // should we want to take this account
		Seconds transmissionStopTime = eventArrivalTime + processingDelay;
		stopTransmission(transmissionStopTime, simulator);

		/**
		 * Cancel the original transmission stop event (this event would have performed
		 * the above, but at a simulation time corresponding with the frame's having been
		 * transmitted in its entirety without interruption). The event will remain in
		 * the event queue but will do nothing once it is processed.
		 */
		currentTransmissionStopEvent->cancelled = true;
		currentTransmissionStopEvent = nullptr;

		// perform an exponential backoff
		Bits waitTime = defaultBackoff.getNextWaitTime();
		if (waitTime < 0)
		{
			// the maximum number of collisions has been succeeded such that we must drop the frame
			popFrame();
		}
		else
		{
			/**
			 * Register an event for the node to attempt to transmit the current frame again after
			 * the current backoff time.
			 */
			Seconds backoffDelay = ((double)waitTime) / channel->channelTransmissionRate;
			Seconds transmissionAttemptTime = transmissionStopTime + backoffDelay;
			TransmissionAttemptEvent transmissionAttemptEvent = new TransmissionAttemptEvent(transmissionAttemptTime, this);
			simulator->addEvent(transmissionAttemptEvent);
		}
	}
}

/**
 * The CSMA/CD MAC protocol implementation for checking if the channel whether the
 * channel went idle and responding accordingly.
 */
void CSMACDNode::acceptChannelBusyStopEventImplementation(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	/**
	 * If the node last sensed the channel as being busy (isn't currently backed off
	 * for some delay time after a collision, the CSMA/CD scheme is persistent, and
	 * the channel became idle after the arrival of this event, and immediately
	 * attempt to transmit the current frame.
	 */
	bool isBackedOff = defaultBackoff.isBackedOff();
	if (!isBackedOff && persistent && !channelIsBusy()) attemptTransmission(eventArrivalTime, simulator);
}

/**
 * The CSMA/CD MAC protocol implementation for sensing the carrier.
 */
bool CSMACDNode::shouldTransmit(
	Seconds checkTime,
	NetworkSimulator *simulator
)
{
	if (channelIsBusy())
	{
		if (!persistent)
		{
			/**
			 * In the non-persistent case, if the channel is still busy, we must
			 * perform an exponential backoff.
			 */
			Bits waitTime = nonPersistentBackoff.getNextWaitTime();
			if (waitTime < 0)
			{
				// the maximum number of collisions has been succeeded such that we must drop the frame
				popFrame();
			}
			else
			{
				/**
				 * Register an event for the node to attempt to transmit the current frame again after
				 * the current backoff time.
				 */
				Seconds backoffDelay = ((double)waitTime) / channel->channelTransmissionRate;
				Seconds transmissionAttemptTime = checkTime + backoffDelay;
				TransmissionAttemptEvent transmissionAttemptEvent = new TransmissionAttemptEvent(transmissionAttemptTime, this);
				simulator->addEvent(transmissionAttemptEvent);
			}
		}

		/**
		 * In the non-persistent case, we must wait for enough ChannelBusyStopEvent instances
		 * to have been received such that the channel becomes idle; this simulates the node
		 * _persistently_ sensing the medium until the exact moment that it becomes idle, at
		 * which point it will immediately start transmitting.
		 */

		return false;
	}

	if (!persistent)
	{
		/**
		 * In the non-persistent case, if the channel was not busy, then
		 * we can reset the exponential backoff's collision counter.
		 */
		nonPersistentBackoff->reset();
	}
	
	return true;
}

/**
 * Resets the default exponential backoff's collision counter upon a transmission
 * success per the CSMA/CD MAC protocol.
 */
void CSMACDNode::acceptTransmissionStopEventImplementation(
	Seconds eventArrivalTime,
	NetworkSimulator *simulator
)
{
	defaultBackoff.reset();
}