#pragma once

#include "TransmissionStopEvent.hpp"

/**
 * A class that implements the Visitor Pattern so that it can accept
 * and be "visited" by transmission stop events.
 */
class TransmissionStopEventAcceptor
{
public:

	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when it reaches the point in time at which it should stop or
	 * finish transmitting.
	 */
	virtual void acceptTransmissionStopEvent(
		TransmissionStopEvent *event,
		NetworkSimulator *simulator
	) = 0;
};