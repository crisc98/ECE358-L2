#pragma once

#include "ChannelSenseEvent.hpp"

/**
 * ~Delegates to the derived type the decision of which implementation of the
 * ChannelSenseEvent class to use.
 */
class ChannelSenseEventFactory
{
public:

	/**
	 * Returns an instance of the ChannelSenseEvent abstract class, such
	 * to be situated at the specified node.
	 */
	virtual ChannelSenseEvent* createEvent(Seconds time, Node *node) = 0;
};