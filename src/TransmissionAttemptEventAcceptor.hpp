#pragma once

#include "TransmissionAttemptEvent.hpp"

/**
 * A class that implements the Visitor Pattern so that it can accept
 * and be "visited" by transmission attempt events.
 */
class TransmissionAttemptEventAcceptor
{
	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when it reaches the point in time at which it should attempt to
	 * transmit a frame.
	 */
	virtual void acceptTransmissionAttemptEvent(
		TransmissionAttemptEvent *event,
		NetworkSimulator *simulator
	) = 0;
};