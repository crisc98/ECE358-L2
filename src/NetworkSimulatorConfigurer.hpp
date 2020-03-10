#pragma once

#include "NetworkSimulator.hpp"

/**
 * Maintains an instance of a network implementation and simulator and the logic for
 * configuring them for varying numbers of nodes.
 */
class NetworkSimulatorConfigurer
{
public:
	/**
	 * Returns a pointer to a network simulator that has been configured for a
	 * certain topology with the specified number of nodes.
	 */
	virtual NetworkSimulator* configureNetworkSimulationFor(Nodes nodes) = 0;
};