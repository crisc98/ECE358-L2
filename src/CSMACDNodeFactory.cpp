#include "CSMACDNodeFactory.hpp"

#include "CSMACDNode.hpp"

/**
 * Creates a Node instance that implements the CSMA/CD MAC protocol.
 */
Node* CSMACDNodeFactory::createNode(NodeNumber number)
{
	Node *node = new CSMACDNode(number, persistent);
	return node;
}