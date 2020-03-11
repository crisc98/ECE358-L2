#include "UniformBus.hpp"

/**
 * Calculates the time required to fully transmit the frame currently at the front
 * of the specified node's frame queue.
 */
Seconds UniformBus::getTransmissionDelayOf(Node *node)
{
	return frameTransmissionTime;
}

/**
 * Calculates the shortest propagation delay between two nodes in a bus configuration
 * assuming that the inter-node distance and channel propagation speeds are the same
 * throughout.
 * The inputs are assumed to be non-null.
 */
Seconds UniformBus::getShortestPropagationDelayBetween(Node *node1, Node *node2)
{
	Nodes nodalDistance = std::abs(node1->number - node2->number);
	Meters totalDistance = interNodeDistance * ((double)nodalDistance);
	Seconds propagationDelay = totalDistance / channelPropagationSpeed;
	return propagationDelay;
}