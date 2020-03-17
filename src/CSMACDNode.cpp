#include "CSMACDNode.hpp"

/**
 * Performs an exponential backoff using the specific state instance and registers
 * a new transmission attempt event to be processed after the backoff has finished.
 * 
 * Returns the backoff delay, else a -1 if the number of collisions has already been
 * exceeded.
 */
Seconds CSMACDNode::performExponentialBackoff(
	Seconds backoffStart,
	ExponentialBackoff *backoffState,
	NetworkSimulator *simulator
)
{
	Bits waitTime = backoffState->getNextWaitTime();
	if (waitTime < 0)
	{
		// the maximum number of collisions has been succeeded such that we must drop the frame
		popFrame(backoffStart, simulator);
		backoffState->reset();
		return -1;
	}

	/**
	 * Register an event for the node to attempt to transmit the current frame again after
	 * the current backoff time.
	 */
	Seconds backoffDelay = ((double)waitTime) / channel.channelTransmissionRate;
	Seconds transmissionAttemptTime = backoffStart + backoffDelay;
	TransmissionAttemptEvent *transmissionAttemptEvent = new TransmissionAttemptEvent(transmissionAttemptTime, this);
	simulator->addEvent(transmissionAttemptEvent);

	return backoffDelay;
}

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

		// perform an exponential backoff and increment the corresponding statistics counters
		if (!defaultBackoff.isBackedOff())
		{
			++simulator->network->totalCollidedFrames;
		}
		Seconds backoffDelay = performExponentialBackoff(transmissionStopTime, &defaultBackoff, simulator);
		if (backoffDelay < 0)
		{
			++simulator->network->totalCollisionDrops;
		}
		else
		{
			++simulator->network->totalDefaultBackoffs;
			simulator->network->totalDefaultBackoffDelay += backoffDelay;
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
	if (hasFrames())
	{
		/**
		 * If the node last sensed the channel as being busy, isn't currently backed off
		 * for some delay time after a collision, the CSMA/CD scheme is persistent, and
		 * the channel became idle after the arrival of this event, immediately attempt
		 * to transmit the current frame.
		 */
		bool isBackedOff = defaultBackoff.isBackedOff();
		if (isCurrentlyWaiting && !isBackedOff && persistent && !channelIsBusy())
		{
			isCurrentlyWaiting = false;
			Seconds totalWaitTime = eventArrivalTime - lastWaitStart;
			simulator->network->totalWaitTime += totalWaitTime;
			attemptTransmission(eventArrivalTime, simulator);
		}
	}
}

/**
 * The CSMA/CD MAC protocol implementation for sensing the carrier.
 * Assigns a value to the passed processingDelay reference.
 */
bool CSMACDNode::shouldTransmit(
	Seconds checkTime,
	Seconds &processingDelay,
	NetworkSimulator *simulator
)
{
	++simulator->network->totalChannelSenses;

	processingDelay = 0; // should we want to take this account

	if (channelIsBusy())
	{
		if (persistent)
		{
			/**
			 * In the persistent case, we must wait for enough ChannelBusyStopEvent instances
			 * to have been received such that the channel becomes idle; this simulates the node
			 * _persistently_ sensing the medium until the exact moment that it becomes idle, at
			 * which point it will immediately start transmitting.
			 */
			if (!isCurrentlyWaiting)
			{
				isCurrentlyWaiting = true;
				++simulator->network->totalPostponedFrames;
			}
			++simulator->network->totalWaits;
			lastWaitStart = checkTime + processingDelay;
		}
		else
		{
			/**
			 * In the non-persistent case, if the channel is still busy, we must
			 * perform an exponential backoff.
			 */
			if (!nonPersistentBackoff->isBackedOff())
			{
				++simulator->network->totalPostponedFrames;
			}
			Seconds backoffDelay = performExponentialBackoff(checkTime + processingDelay, nonPersistentBackoff, simulator);
			if (backoffDelay < 0)
			{
				++simulator->network->totalNonPersistentDrops;
			}
			else
			{
				++simulator->network->totalWaits;
				simulator->network->totalWaitTime += backoffDelay;
			}
		}

		return false;
	}

	if (persistent)
	{
		isCurrentlyWaiting = false;
	}
	else
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
	Seconds transmissionStopTime,
	NetworkSimulator *simulator
)
{
	defaultBackoff.reset();
}