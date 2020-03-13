#include "ChannelConnection.hpp"

/**
 * Calculates the time at which the first bit of a transmission (or _any_ signal, such
 * as the assertion or deassertion of a signal) from the current node would arrive at
 * the target node.
 */
Seconds ChannelConnection::getSignalArrivalTime(Seconds startTime)
{
	Seconds transmissionStartArrivalTime = startTime + channelPropagationDelay;
	return transmissionStartArrivalTime;
}