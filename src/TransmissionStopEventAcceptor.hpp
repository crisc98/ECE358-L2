#pragma once

#include "NetworkSimulator.hpp"

/**
 * A class that implements the Visitor Pattern so that it can accept
 * and be "visited" by transmission stop events.
 */
class TransmissionStopEventAcceptor
{
public:

	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond after it has completely finished transmitting the current frame.
	 */
	virtual void acceptTransmissionStopEvent(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	) = 0;
};