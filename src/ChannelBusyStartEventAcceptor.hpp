#pragma once

#include "ChannelBusyStartEvent.hpp"

/**
 * A class that implements the Visitor Pattern so that it can accept
 * and be "visited" by channel busy start events.
 */
class ChannelBusyStartEventAcceptor
{
public:

	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when an incoming starts asserting itself at the accepting
	 * node's location on the channel.
	 */
	virtual void acceptChannelBusyStartEvent(
		ChannelBusyStartEvent *event,
		NetworkSimulator *simulator
	) = 0;
};