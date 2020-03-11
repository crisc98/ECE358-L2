#pragma once

#include "NetworkSimulator.hpp"

/**
 * Represents an network event situated at a particular node in that network.
 */
class NetworkEvent : DiscreteEvent<Seconds, NetworkSimulator>
{
public:

	/**
	 * The node this event is destined for.
	 */
	Node *node;

	NetworkEvent(
		Seconds time,
		Node *node
	) :
		DiscreteEvent(time),
		node(node)
	{
	}

	/**
	 * Performs some operation and processing around this node.
	 */
	virtual void process(NetworkSimulator *simulator) = 0;
};