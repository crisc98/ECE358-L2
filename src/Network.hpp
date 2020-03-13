#pragma once

#include <vector>

// forward declaration to avoid a circular dependency
class Node;

typedef double AverageBitsPerSecond;
typedef double Percentage;
typedef double Seconds;
typedef long Bits;
typedef long Frames;

/**
 * Represents a graph of nodes arranged in a particular network topology.
 * 
 * This interface is not responsible for the actual configuration of the nodes
 * in a network and the propagation delays between them; this is delegated to
 * specific implementations of this interface (e.g. a bus implementation or an
 * arbitrary graph) or some network factory.
 */
class Network
{
public:

	/**
	 * The set of nodes in this network.
	 * To be populated externally.
	 * Cleared via the reset() method which likewise destroys all the nodes.
	 */
	std::vector<Node*> nodes;

	/**
	 * The total number of attempts across all nodes to transmit a frame.
	 */
	Frames totalTransmissionAttempts;

	/**
	 * The total number of frames to actually be fully and successfully transmitted.
	 */
	Frames totalTransmittedFrames;

	/**
	 * The total number of bits successfully transmitted by the nodes in this network.
	 */
	Bits totalTransmittedBits;

	/**
	 * Calculates the ratio of the total number of successful transmissions over the
	 * total number of transmission attempts across all nodes in the network.
	 */
	Percentage getEfficiency();

	/**
	 * Calculates the average number of bits per second throughput perceived throughout
	 * the simulation's duration.
	 */
	AverageBitsPerSecond getThroughput(Seconds simulationDuration);

	/**
	 * Removes all nodes from the network.
	 */
	void reset();

	/**
	 * Ensures that all registered nodes in the network are destroyed once this class is destroyed.
	 */
	~Network()
	{
		reset();
	}
};