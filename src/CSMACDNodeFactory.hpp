#pragma once

#include "NodeFactory.hpp"

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
	 * Creates a Node instance that implements the CSMA/CD MAC protocol.
	 */
	Node* createNode(NodeNumber number, Channel channel);
};