#pragma once

#include "UniformBus.hpp"
#include "NetworkSimulatorConfigurer.hpp"

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

public:

	UniformBusSimulatorConfigurer(
		UniformBus *bus,
		NetworkSimulator *simulator
	) :
		bus(bus),
		simulator(simulator)
	{
	}

	/**
	 * Returns a pointer to a simulation of a uniform bus with the specified number
	 * of nodes.
	 */
	NetworkSimulator* configureNetworkSimulationFor(Nodes nodes);
};