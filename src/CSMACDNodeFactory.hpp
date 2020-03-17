#pragma once

#include "NodeFactory.hpp"

typedef long Collisions;

/**
 * Delegates to the derived type the decision of which implementation of the
 * Node class to use.
 */
class CSMACDNodeFactory : public NodeFactory
{
public:

	/**
	 * Controls whether this node will perform the CSMA/CD MAC protocol using the
	 * persistent (true) or non-persistent (false) case.
	 */
	bool persistent;

	/**
	 * The maximum number of consecutive collisions or channel busy senses before
	 * a frame must be dropped.
	 */
	Collisions maxCollisions;

	CSMACDNodeFactory()
	{
	}

	/**
	 * Creates a Node instance that implements the CSMA/CD MAC protocol.
	 */
	Node* createNode(NodeNumber number, Channel channel);
};