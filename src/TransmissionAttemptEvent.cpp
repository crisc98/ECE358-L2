#include "TransmissionAttemptEvent.hpp"

/**
 * Notifies the node this event is targetting that it should now attempt
 * to transmit a frame in accordance with its particular MAC protocol
 * implementation.
 */
void TransmissionAttemptEvent::process(NetworkSimulator *simulator)
{
	node->acceptTransmissionAttemptEvent(this, simulator);
}

#include "CSMACDChannelBusyStartEvent.hpp"

/**
 * Performs the CSMA/CD MAC protocol for determining whether and when a node
 * should attempt to transmit.
 */
void CSMACDChannelSenseEvent::process(NetworkSimulator *simulator)
{
	// sense the medium
	if (node->channelIsBusy())
	{
		// in the non-persistent case, we perform an exponential backoff
		if(!persistent)
		{
			Seconds projectedTimeOfTransmission = node->getProjectedTimeOfTransmissionOnChannelBusy();
			CSMACDChannelSenseEvent channelSenseEvent = new CSMACDChannelSenseEvent(projectedTimeOfTransmission, node, persistent);
		}

		// in the persistent case, we wait for the exact moment that the channel becomes free
	}
	else
	{
		// immediately start transmitting

		node->isCurrentlyTransmitting = true;

		// propagate the start of this node's transmission to all other nodes
		for (Node *otherNode : simulator->network->nodes)
		{
			if (node != otherNode)
			{
				Seconds propagationDelay = simulator->network->getShortestPropagationDelayBetween(node, otherNode);
				Seconds channelBusyStart = time + propagationDelay;
				CSMACDChannelBusyStartEvent channelBusyStartEvent = new CSMACDChannelBusyStartEvent(channelBusyStart, otherNode, persistent);
				simulator->addEvent(channelBusyStartEvent);
			}
		}
	}
}