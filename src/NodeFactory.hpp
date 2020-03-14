#pragma once

#include "Node.hpp"

/**
 * Delegates to the derived type the decision of which implementation of the
 * Node class to use.
 */
class NodeFactory
{
public:

	/**
	 * Returns an instance of the Node abstract class.
	 */
	virtual Node* createNode(NodeNumber number, Channel channel) = 0;
};