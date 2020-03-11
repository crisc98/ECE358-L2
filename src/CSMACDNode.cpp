#include "CSMACDNode.hpp"

/**
 * Get the expected time at which the frame at the front of this node's frame queue
 * will next sense the channel if the channel is currently free.
 * This is just the original time the frame arrived.
 */
Seconds CSMACDNode::getProjectedTimeOfTransmissionOnChannelFree()
{
	// in the non-persistent case, we reset the secondary collision counter
	if (nonPersistentBackoff != nullptr) nonPersistentBackoff->reset();

	Frame firstFrame = node->peekFrame();
	Seconds projectedTimeOfTransmission = firstFrame.arrivalTime;

	return projectedTimeOfTransmission;
}

/**
 * Get the expected time at which the frame at the front of this node's frame queue
 * will next sense the channel if the channel is currently busy.
 * 
 * This method is only ever called in the non-persistent case, as otherwise,
 * the projected time that the node will next sense the channel is at the not yet
 * known moment that the channel ceases to be busy.
 * 
 * -1 will be returned if it is not the persistent case.
 */
Seconds CSMACDNode::getProjectedTimeOfTransmissionOnChannelBusy(BitsPerSecond channelTransmissionRate)
{
	if (nonPersistentBackoff != nullptr)
	{
		Seconds projectedTimeOfTransmission = nonPersistentBackoff->getNextWaitTime() / channelTransmissionRate;
		return projectedTimeOfTransmission;
	}

	return -1;
}

/**
 * Get the expected time at which the frame at the front of this node's frame queue
 * will next sense the channel if a collision was detected.
 */
Seconds CSMACDNode::getProjectedTimeOfTransmissionOnCollision(BitsPerSecond channelTransmissionRate)
{
	Seconds projectedTimeOfTransmission = defaultBackoff->getNextWaitTime() / channelTransmissionRate;
	return projectedTimeOfTransmission;
}