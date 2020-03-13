#pragma once

#include "NetworkSimulator.hpp"

// put here to avoid a circular dependency
typedef long Nodes;

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
	 * 
	 * The control of the lifespan of the returned instance may vary with the
	 * implementation, but generally, it is _not_ the responsibility of this
	 * interface's user to dispose of the instance.
	 */
	virtual NetworkSimulator* configureNetworkSimulationFor(Nodes nodes) = 0;
};