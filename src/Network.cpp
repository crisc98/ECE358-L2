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
	AverageBitsPerSecond throughput = totalTransmittedBits / simulationDuration;
	return throughput;
}

/**
 * Removes all nodes from the network and resets all of the statistical counters.
 * Must be called before running a simulation on this network or configuring its nodes.
 */
void Network::reset()
{
	totalEnqueuedFrames = 0;
	totalVisitedFrames = 0;
	totalChannelSenses = 0;
	totalTransmissionAttempts = 0;
	totalTransmittedFrames = 0;
	totalTransmittedBits = 0;
	totalPostponedFrames = 0;
	totalWaits = 0;
	totalWaitTime = 0;
	totalNonPersistentDrops = 0;
	totalCollidedFrames = 0;
	totalDefaultBackoffs = 0;
	totalDefaultBackoffDelay = 0;
	totalCollisionDrops = 0;

	for (Node *node : nodes) delete node;
	nodes.clear();
}