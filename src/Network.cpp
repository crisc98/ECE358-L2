#include "Network.hpp"

/**
 * Calculates the ratio of the total number of successful transmissions over the
 * total number of transmission attempts across all nodes in the network.
 */
Percentage Network::getEfficiency()
{
	Frames totalTransmittedFrames = 0;
	Frames totalTransmissionAttempts = 0;

	for (Node *node : nodes)
	{
		totalTransmittedFrames += node->totalTransmittedFrames;
		totalTransmissionAttempts += node->totalTransmissionAttempts;
	}

	Percentage efficiency = ((double)totalTransmittedFrames) / ((double)totalTransmissionAttempts);
	
	return efficiency;
}

/**
 * Calculates the average number of bits per second throughput perceived throughout
 * the simulation's duration.
 */
BitsPerSecond Network::getThroughput(Seconds simulationDuration)
{
	Bits totalTransmittedBits = 0;

	for (Node *node : nodes)
	{
		totalTransmittedBits += node->totalTransmittedBits;
	}

	BitsPerSecond throughput = totalTransmittedBits / simulationDuration;

	return throughput;
}

/**
 * Removes and deletes all nodes from the network.
 */
void Network::reset()
{
	for (Node *node : nodes) delete node;
}