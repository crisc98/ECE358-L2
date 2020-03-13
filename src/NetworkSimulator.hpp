#pragma once

#include "DiscreteEventSimulator.hpp"
#include "Network.hpp"

/**
 * ~Simulates the CSMA/CD MAC protocol for negotiating the transmission of frames
 * from the nodes in a network onto a single, shared channel. The exact topology
 * of the nodes and channel connections is abstracted away in the network's
 * implementation.
 * 
 * This class uses the CRTP to give its discrete network events access to the
 * the simulation state and allow them to register new events.
 * See https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern.
 */
class NetworkSimulator : public DiscreteEventSimulator<Seconds, NetworkSimulator>
{
private:

	/**
	 * Susplies an instance of the simulation state to the supertype.
	 */
	NetworkSimulator* getState()
	{
		return this;
	}

public:

	/**
	 * The network to be simulated.
	 */
	Network *network;

	/**
	 * Creates a simulator for the specified network.
	 */
	NetworkSimulator(Network *network) : network(network)
	{
	}
};