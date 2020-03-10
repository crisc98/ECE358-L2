#pragma once

#include "Network.hpp"

/**
 * Simulates the CSMA/CD MAC protocol for negotiating the transmission of frames
 * from the nodes in a network onto a single, shared channel. The exact topology
 * of the nodes and channel connections is abstracted away in the network's
 * implementation.
 */
class CSMACDSimulator
{
private:

	/**
	 * The network for which CSMA/CD MAC is to be simulated.
	 */
	Network *network;

	/**
	 * Transforms the network's nodes list into a min heap.
	 */
	void sortNodes();

	/**
	 * Obtains the pointer to the next node to attempt to transmit a frame.
	 */
	Node* getSenderNode();

	/**
	 * Pops the frame at the top of the current sending node's frame queue
	 * while maintaining the min heap of nodes.
	 * This is only done after the node's frame has been successfully transmitted
	 * without collision, or if it has been dropped.
	 */
	void popSenderFrame();

public:

	/**
	 * The number of seconds for which the simulator will run.
	 * This controls the latest a frame can be added to a node's frame queue
	 * as well as the latest at which a node can attempt to transmit a frame.
	 */
	Seconds simulationDuration;

	/**
	 * Returns true if the specified time is between 0 seconds and the current simulation duration.
	 */
	bool isWithinSimulationDuration(Seconds time);

	/**
	 * Runs the simulation until all frames within all nodes have either been transmitted or dropped.
	 */
	void runSimulation();
};