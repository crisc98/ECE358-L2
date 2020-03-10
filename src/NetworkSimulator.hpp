#pragma once

#include "DiscreteEventSimulator.hpp"
#include "Network.hpp"

class NetworkSimulator;

typedef DiscreteEvent<Seconds, NetworkSimulator> NetworkEvent;

/**
 * ~Simulates the CSMA/CD MAC protocol for negotiating the transmission of frames
 * from the nodes in a network onto a single, shared channel. The exact topology
 * of the nodes and channel connections is abstracted away in the network's
 * implementation.
 * 
 * This class uses the CRTP to give its discrete network events access to the
 * the simulation state and allow them to register new events.
 */
class NetworkSimulator : public DiscreteEventSimulator<Seconds, NetworkSimulator>
{
private:
	/**
	 * Transforms the network's nodes list into a min heap.
	 */
	void sortNodes();

	/**
	 * Obtains the pointer to the next node to attempt to transmit a frame.
	 */
	Node* getSenderNode();

	const bool TRANSMITTED = true;
	const bool DROPPED = false;

	/**
	 * Pops the frame at the front of the current sending node's frame queue
	 * while maintaining the min heap of nodes.
	 * This is only done after the node's frame has been successfully transmitted
	 * without collision, or if it has been dropped.
	 */
	void popSenderFrame(bool transmittedOrDropped);

	/**
	 * Attempts to transmit the sender node's frame, likewise checking if any collisions
	 * will happen during that transmission.
	 * Returns true if there are more frames to be transmitted and the simulation duration
	 * has not yet been exceeded.
	 */
	bool attemptTransmission();

public:

	/**
	 * The network to be simulated.
	 */
	Network *network;

	/**
	 * ~Creates a simulator for the specified network.
	 */
	NetworkSimulator(Network *network) : network(network)
	{
	}
};