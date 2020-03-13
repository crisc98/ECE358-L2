#pragma once

#include "ChannelBusyStopEvent.hpp"

/**
 * A class that implements the Visitor Pattern so that it can accept
 * and be "visited" by channel busy end events.
 */
class ChannelBusyStopEventAcceptor
{
public:

	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when an incoming signal stops asserting itself at the accepting
	 * node's location on the channel.
	 */
	virtual void acceptChannelBusyStopEvent(
		ChannelBusyStopEvent *event,
		NetworkSimulator *simulator
	) = 0;
};