#include "Network.hpp"

// put here to avoid a circular dependency
#include "Node.hpp"

/**
 * Calculates the ratio of the total number of successful transmissions over the
 * total number of transmission attempts across all nodes in the network.
 */
Percentage Network::getEfficiency()
{
	Percentage efficiency = ((double)totalTransmittedFrames) / ((double)totalTransmissionAttempts);
	return efficiency;
}

/**
 * Calculates the average number of bits per second throughput perceived throughout
 * the simulation's duration.
 */
AverageBitsPerSecond Network::getThroughput(Seconds simulationDuration)
{
	AverageBitsPerSecond throughput = ((double)totalTransmittedBits) / simulationDuration;
	return throughput;
}

/**
 * Removes and deletes all nodes from the network.
 */
void Network::reset()
{
	for (Node *node : nodes) delete node;
	nodes.clear();
}