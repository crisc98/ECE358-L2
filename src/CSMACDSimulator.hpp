#pragma once

#include "Bus.hpp"

class CSMACDSimulator
{
private:

	/**
	 * The bus-topology network for which CSMA/CD MAC is to be simulated.
	 */
	Bus bus;

public:

	/**
	 * The number of seconds for which the simulator will run.
	 * This controls the latest a frame can be added to a node's frame queue
	 * as well as the latest at which a node can attempt to transmit a frame.
	 */
	Seconds simulationDuration;

	/**
	 * Runs the simulation until all frames within all nodes have either been transmitted or dropped.
	 */
	void runSimulation();
};