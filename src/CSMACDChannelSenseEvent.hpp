#pragma once

#include "NetworkSimulator.hpp"

/**
 *
 */
class CSMACDChannelSenseEvent : NetworkEvent
{
public:

	/**
	 *
	 */
	void process(NetworkSimulator *simulator);
};