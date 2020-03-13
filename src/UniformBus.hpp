#pragma once

#include "Network.hpp"

typedef double Meters;
typedef double MetersPerSecond;
typedef long FramesPerSecond;

/**
 * A uniform bus is a collection of nodes arranged in a line with equal
 * distances between adjacent nodes, identical channel transmission rates
 * and propagation speeds between each node, identical average frame arrival
 * rates across each node, and identical, constant frame lengths across all
 * nodes.
 */
class UniformBus : public Network
{
public:

	/**
	 * The average number of frames per second to be generated at each node.
	 */
	FramesPerSecond averageFrameArrivalRate;

	/**
	 * The number of bits per second that can be transmitted through the channel.
	 */
	BitsPerSecond channelTransmissionRate;

	/**
	 * The length of each frame in bits.
	 */
	Bits frameLength;

	/**
	 * The distance between adjacent nodes in meters.
	 */
	Meters interNodeDistance;

	/**
	 * The speed at which information can propagate through the channel in meters per second.
	 */
	MetersPerSecond channelPropagationSpeed;

	/**
	 * Initializes the bus data structure's fields to 0.
	 */
	UniformBus() :
		averageFrameArrivalRate(0),
		interNodeDistance(0),
		channelPropagationSpeed(0)
	{
	}
};