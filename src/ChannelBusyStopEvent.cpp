#include "ChannelBusyStopEvent.hpp"

/**
 * Notifies the node this event is targetting that a signal has stopped
 * asserting itself at that node's location on the channel.
 */
void ChannelBusyStopEvent::processImplementation(NetworkSimulator *simulator)
{
	node->acceptChannelBusyStopEvent(time, simulator);
}