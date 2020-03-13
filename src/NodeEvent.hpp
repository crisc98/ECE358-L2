#pragma once

#include "NetworkEvent.hpp"

/**
 * Represents an network event situated at a particular node in that network.
 */
template <class TNode>
class NodeEvent : public NetworkEvent
{
public:

	/**
	 * The node this event is destined for.
	 */
	TNode *node;

	NodeEvent(
		Seconds time,
		TNode *node
	) :
		NetworkEvent(time),
		node(node)
	{
	}

	/**
	 * Performs some operation and processing around this node.
	 */
	void processImplementation(NetworkSimulator *simulator);
};