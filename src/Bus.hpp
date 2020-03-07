#pragma once

#include "Node.hpp"

typedef double Meters;
typedef double MetersPerSecond;
typedef long Bits;
typedef long BitsPerSecond;
typedef long FramesPerSecond;
typedef long Nodes;

class Bus
{
public:

	/**
	 * The total number of nodes connected to the bus-topology network.
	 */
	Nodes nodeCount;

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
};