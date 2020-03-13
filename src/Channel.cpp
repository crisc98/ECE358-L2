#include "Channel.hpp"

/**
 * Calculates the amount of time it takes to transmit the specified number of bits
 * onto the channel.
 */
Seconds Channel::getTransmissionDelay(Bits bits)
{
	Seconds transmissionDelay = ((double)bits) / channelTransmissionRate;
	return transmissionDelay;
}