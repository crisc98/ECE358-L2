#include "Bus.hpp"

/**
 * Add the specified number of nodes to the bus.
 */
void Bus::addNodes(Nodes nodes)
{

}

/**
 * Calculates the time required to fully transmit the frame currently at the front
 * of the specified node's frame queue.
 */
Seconds Bus::getTransmissionDelayOf(Node *node)
{
	return frameTransmissionTime;
}

/**
 * Calculates the shortest propagation delay between two nodes in a bus configuration
 * assuming that the inter-node distance and channel propagation speeds are the same
 * throughout.
 * The inputs are assumed to be non-null.
 */
Seconds Bus::getShortestPropagationDelayBetween(Node *node1, Node *node2)
{
	Nodes nodalDistance = std::abs(node1->number - node2->number);
	Meters totalDistance = interNodeDistance * ((double)nodalDistance);
	Seconds propagationDelay = totalDistance / channelPropagationSpeed;
	return propagationDelay;
}