#pragma once

#include "ChannelSenseEventFactory.hpp"
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

	/**
	 * Delegates to the derived type the decision of which implementation of the
	 * ChannelSenseEvent class to use.
	 */
	ChannelSenseEventFactory *channelSenseEventfactory;
	
public:

	UniformBusSimulatorConfigurer(
		UniformBus *bus,
		NetworkSimulator *simulator,
		NodeFactory *nodeFactory,
		ChannelSenseEventFactory *channelSenseEventfactory
	) :
		bus(bus),
		simulator(simulator),
		nodeFactory(nodeFactory),
		channelSenseEventfactory(channelSenseEventfactory)
	{
	}

	/**
	 * Returns a pointer to a simulation of a uniform bus with the specified number
	 * of nodes.
	 */
	NetworkSimulator* configureNetworkSimulationFor(Nodes nodes);
};