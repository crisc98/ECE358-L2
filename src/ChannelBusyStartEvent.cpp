#include "ChannelBusyStartEvent.hpp"

/**
 * Notifies the node this event is targetting that another signal has arrived
 * and started asserting itself at that node's location on the channel.
 */
void ChannelBusyStartEvent::process(NetworkSimulator *simulator)
{
	node->acceptChannelBusyStartEvent(this, simulator);
}

#include "CSMACDChannelBusyEndEvent.hpp"
#include "CSMACDChannelSenseEvent.hpp"

/**
 * Tells the target node that another signal has arrived and started asserting itself
 * that node's location on the channel.
 * 
 * This will also notify the node if there was a collision; i.e. the node was already
 * transmitting when this event was received.
 */
void CSMACDChannelBusyStartEvent::process(NetworkSimulator *simulator)
{
	++node->numIncomingSignals;

	// if there was a collision, immediately perform an exponential backoff and abort
	if (node->isCurrentlyTransmitting)
	{
		node->isCurrentlyTransmitting = false;

		Seconds projectedTimeOfTransmission = node->getProjectedTimeOfTransmissionOnCollision();
		CSMACDChannelSenseEvent channelSenseEvent = new CSMACDChannelSenseEvent(projectedTimeOfTransmission, node, persistent);
		simulator->addEvent(channelSenseEvent);

		// propagate the end of this node's transmission to all other nodes
		for (Node *otherNode : simulator->network->nodes)
		{
			if (node != otherNode)
			{
				Seconds propagationDelay = simulator->network->getShortestPropagationDelayBetween(node, otherNode);
				Seconds channelBusyEnd = time + propagationDelay;
				CSMACDChannelBusyEndEvent channelBusyEndEvent = new CSMACDChannelBusyEndEvent(channelBusyEnd, otherNode, persistent);
				simulator->addEvent(channelBusyEndEvent);
			}
		}
	}
}