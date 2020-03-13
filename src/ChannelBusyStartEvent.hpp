#pragma once

#include "ChannelBusyStartEventAcceptor.hpp"
#include "NodeEvent.hpp"

/**
 * Indicates an event at a node at which that node should start sensing
 * the channel as being busy.
 */
typedef NodeEvent<ChannelBusyStartEventAcceptor> ChannelBusyStartEvent;

class CSMACDChannelBusyStartEvent : public NetworkEvent
{
private:

	bool persistent;

public:

	CSMACDChannelBusyStartEvent(
		Seconds time,
		Node *node
	) : 
		NetworkEvent(time, node)
	{
	}

	/**
	 * Tells the target node that another signal has arrived and started asserting itself
	 * that node's location on the channel.
	 * 
	 * This will also notify the node if there was a collision; i.e. the node was already
	 * transmitting when this event was received.
	 */
	void process(NetworkSimulator *simulator);
};