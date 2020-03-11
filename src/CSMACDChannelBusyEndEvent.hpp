#pragma once

#include "NetworkEvent.hpp"

/**
 * Indicates an event at a node at which that node stops receiving activity
 * from another node. The channel at the node's location does not stop being
 * busy until all signals arriving from other nodes have gone quiet.
 */
class CSMACDChannelBusyEndEvent : public NetworkEvent
{
private:

	bool persistent;

public:

	CSMACDChannelBusyEndEvent(
		Seconds time,
		Node *node,
		bool persistent
	) : 
		NetworkEvent(time, node),
		persistent(persistent)
	{
	}

	/**
	 * Tells the target node that a signal has stopped asserting itself at that node's
	 * location on the channel.
	 */
	void process(NetworkSimulator *simulator);
};