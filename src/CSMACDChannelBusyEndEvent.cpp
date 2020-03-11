#include "CSMACDChannelBusyEndEvent.hpp"

#include "CSMACDChannelSenseEvent.hpp"

/**
 * Tells the target node that a signal has stopped asserting itself at that node's
 * location on the channel.
 */
void CSMACDChannelBusyEndEvent::process(NetworkSimulator *simulator)
{
	--node->numIncomingSignals;

	/**
	 * If there was a collision, the channel became idle after the arrival of this event,
	 * and the CSMA/CD scheme is persistent, immediately schedule a channel sense event
	 * at the time of this channel busy end event's arrival.
	 */
	if (!node->channelIsBusy() && persistent)
	{
		CSMACDChannelSenseEvent channelSenseEvent = new CSMACDChannelSenseEvent(time, node, persistent);
		simulator->addEvent(channelSenseEvent);
	}
}