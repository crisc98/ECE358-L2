#pragma once

#include <vector>

#include "Node.hpp"

typedef double Seconds;

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
	 * Calculates the shortest propagation delay between any two nodes in the network
	 * based on the particular network topology implementation.
	 * E.g. A bus implementation with certain assumptions could perform optimizations
	 * on this calculation.
	 * The inputs must be non-null.
	 */
	virtual Seconds getShortestPropagationDelayBetween(Node *node1, Node *node2);
};