#include "ChannelBusyStartEvent.hpp"

/**
 * Notifies the node this event is targetting that another signal has arrived
 * and started asserting itself at that node's location on the channel.
 */
void ChannelBusyStartEvent::processImplementation(NetworkSimulator *simulator)
{
	node->acceptChannelBusyStartEvent(time, simulator);
}