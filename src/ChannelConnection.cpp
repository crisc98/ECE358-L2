#include "ChannelConnection.hpp"

/**
 * Calculates the time at which the first bit of a transmission from the current node
 * would arrive at the target node.
 */
Seconds ChannelConnection::getTransmissionStartArrivalTime(Seconds startTime)
{
	Seconds transmissionStartArrivalTime = startTime + channelPropagationDelay;
	return transmissionStartArrivalTime;
}

/**
 * Calculates the amount of time it takes to transmit the specified number of bits
 * onto the channel.
 */
Seconds ChannelConnection::getTransmissionDelay(Bits bits)
{
	Seconds transmissionDelay = ((double)bits) / channelTransmissionRate;
	return transmissionDelay;
}

/**
 * Calculates the time at which the last bit of a transmission from the current node
 * would arrive at the target node.
 */
Seconds ChannelConnection::getTransmissionStopArrivalTime(Seconds startTime, Bits bits)
{
	Seconds transmissionStartArrivalTime = getTransmissionStartArrivalTime(startTime);
	Seconds transmissionDelay = getTransmissionDelay(bits);
	Seconds transmissionStopArrivalTime = transmissionStartArrivalTime + transmissionDelay;
	return transmissionStopArrivalTime;
}