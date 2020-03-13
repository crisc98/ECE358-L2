#pragma once

#include "NetworkSimulatorConfigurer.hpp"
#include "NodeFactory.hpp"
#include "UniformBus.hpp"

/**
 * Used to configure simulations of a "uniform bus" for various numbers of nodes.
 * See UniformBus.hpp for the definition of a "uniform bus".
 */
class UniformBusSimulatorConfigurer : public NetworkSimulatorConfigurer
{
private:

	/**
	 * The pointer to the "uniform bus" implementation of the network to be simulated.
	 */
	UniformBus *bus;

	/**
	 * The pointer to the simulator containing the "uniform bus" implementation to be simulated.
	 */
	NetworkSimulator *simulator;

	/**
	 * Delegates to the derived type the decision of which implementation of the
	 * Node class to use.
	 */
	NodeFactory *nodeFactory;
	
public:

	UniformBusSimulatorConfigurer(
		UniformBus *bus,
		NetworkSimulator *simulator,
		NodeFactory *nodeFactory
	) :
		bus(bus),
		simulator(simulator),
		nodeFactory(nodeFactory)
	{
	}

	/**
	 * Returns a pointer to a simulation of a uniform bus with the specified number
	 * of nodes.
	 */
	NetworkSimulator* configureNetworkSimulationFor(Nodes nodes);
};