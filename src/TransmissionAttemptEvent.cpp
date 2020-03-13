#include "TransmissionAttemptEvent.hpp"

/**
 * Notifies the node this event is targetting that it should now attempt
 * to transmit a frame in accordance with its particular MAC protocol
 * implementation.
 */
void TransmissionAttemptEvent::processImplementation(NetworkSimulator *simulator)
{
	node->acceptTransmissionAttemptEvent(time, simulator);
}