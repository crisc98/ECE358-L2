#pragma once

#include "ChannelSenseEventFactory.hpp"

/**
 * Delegates to the derived type the decision of which implementation of the
 * ChannelSenseEvent class to use.
 */
class CSMACDChannelSenseEventFactory : public ChannelSenseEventFactory
{
public:

	/**
	 * Controls whether channel sense events are created for the persistent (true)
	 * or non-persistent (false) case of the CSMA/CD MAC protocol.
	 */
	bool persistent;

	/**
	 * Creates a ChannelSenseEvent instance that implements the CSMA/CD MAC protocol.
	 */
	ChannelSenseEvent* createEvent(Seconds time, Node *node);
};