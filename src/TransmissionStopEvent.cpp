#include "TransmissionStopEvent.hpp"

/**
 * Notifies the node this event is targetting that it should stop transmitting
 * its current frame.
 */
void TransmissionStopEvent::processImplementation(NetworkSimulator *simulator)
{
	node->acceptTransmissionStopEvent(this, simulator);
}