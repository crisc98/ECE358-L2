#include "CSMACDChannelSenseEventFactory.hpp"

#include "CSMACDChannelSenseEvent.hpp"

/**
 * Creates a ChannelSenseEvent instance that implements the CSMA/CD MAC protocol.
 */
ChannelSenseEvent* CSMACDChannelSenseEventFactory::createEvent(Seconds time, Node *node)
{
	ChannelSenseEvent *event = new CSMACDChannelSenseEvent(time, node, persistent);
	return event;
}