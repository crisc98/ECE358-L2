#include "CSMACDNodeFactory.hpp"

#include "CSMACDNode.hpp"

/**
 * Creates a Node instance that implements the CSMA/CD MAC protocol.
 */
Node* CSMACDNodeFactory::createNode(NodeNumber number, Channel channel)
{
	Node *node = new CSMACDNode(number, channel, persistent, maxCollisions);
	return node;
}