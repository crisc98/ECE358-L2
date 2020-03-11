#pragma once

#include "NetworkEvent.hpp"

/**
 * Represents the event of a node attempting to transmit a frame on the channel
 * or determining whether it can do so.
 */
class ChannelSenseEvent : public NetworkEvent
{
public:

	ChannelSenseEvent(Seconds time, Node *node) : NetworkEvent(time, node)
	{
	}

	/**
	 * Attempts to transmit a frame on the channel or determine whether it can do so.
	 */
	virtual void process(NetworkSimulator *simulator) = 0;
};