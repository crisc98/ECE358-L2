#pragma once

typedef double Bits;
typedef double BitsPerSecond;
typedef double MetersPerSecond;
typedef double Seconds;

class Channel
{
public:

	/**
	 * The number of bits per second that can be transmitted through the channel.
	 */
	const BitsPerSecond channelTransmissionRate;

	/**
	 * The speed at which information can propagate through the channel in meters per second.
	 */
	const MetersPerSecond channelPropagationSpeed;
	
	Channel(
		BitsPerSecond channelTransmissionRate,
		MetersPerSecond channelPropagationSpeed
	) :
		channelTransmissionRate(channelTransmissionRate),
		channelPropagationSpeed(channelPropagationSpeed)
	{
	}

	/**
	 * Calculates the amount of time it takes to transmit the specified number of bits
	 * onto the channel.
	 */
	Seconds getTransmissionDelay(Bits bits);
};