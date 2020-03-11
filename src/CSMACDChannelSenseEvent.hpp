#pragma once

#include "ChannelSenseEvent.hpp"

/**
 * Represents the event of a node performing the CSMA/CD MAC protocol for
 * determining whether and when a node should attempt to transmit.
 */
class CSMACDChannelSenseEvent : public ChannelSenseEvent
{
private:

	bool persistent;

public:

	CSMACDChannelSenseEvent(
		Seconds time,
		Node *node,
		bool persistent
	) : 
		ChannelSenseEvent(time, node),
		persistent(persistent)
	{
	}

	/**
	 * Performs the CSMA/CD MAC protocol for determining whether and when a node
	 * should attempt to transmit.
	 */
	void process(NetworkSimulator *simulator);
};