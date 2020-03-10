#pragma once

#include <vector>

#include "Node.hpp"

typedef double Seconds;
typedef double Percentage;

/**
 * Represents a graph of nodes arranged in a particular network topology
 * and encapsulates the means for calculating propagation delays between nodes.
 * 
 * This interface is not responsible for the actual configuration of the nodes
 * in a network and the propagation delays between them; this is delegated to
 * specific implementations of this interface (e.g. a bus implementation or an
 * arbitrary graph).
 */
class Network
{
public:

	std::vector<Node*> nodes;

	/**
	 * Add the specified number of nodes to the network.
	 */
	virtual void addNodes(Nodes nodes, Seconds ) = 0;

	/**
	 * Calculates the time required to fully transmit the frame currently at the front
	 * of the specified node's frame queue.
	 */
	virtual Seconds getTransmissionDelayOf(Node *node) = 0;

	/**
	 * Calculates the shortest propagation delay between any two nodes in the network
	 * based on the particular network topology implementation.
	 * E.g. A bus implementation with certain assumptions could perform optimizations
	 * on this calculation.
	 * The inputs must be non-null.
	 */
	virtual Seconds getShortestPropagationDelayBetween(Node *node1, Node *node2) = 0;

	/**
	 * Calculates the ratio of the total number of successful transmissions over the
	 * total number of transmission attempts across all nodes in the network.
	 */
	Percentage getEfficiency();

	/**
	 * Calculates the average number of bits per second throughput perceived throughout
	 * the simulation's duration.
	 */
	BitsPerSecond getThroughput(Seconds simulationDuration);
	
	/**
	 * Removes all nodes from the network.
	 */
	void reset();

	/**
	 * Ensures that all registered nodes in the network have been destroyed once this class is destroyed.
	 */
	~Network()
	{
		reset();
	}
};