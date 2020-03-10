#pragma once

#include "Network.hpp"

typedef double Meters;
typedef double MetersPerSecond;
typedef long Bits;
typedef long BitsPerSecond;
typedef long FramesPerSecond;
typedef long Nodes;

class Bus : Network
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

	//

	/**
	 * The distance between adjacent nodes in meters.
	 */
	Meters interNodeDistance;

	/**
	 * The speed at which information can propagate through the channel in meters per second.
	 */
	MetersPerSecond channelPropagationSpeed;

	/**
	 * Calculates the shortest propagation delay between two nodes in a bus configuration.
	 * The inputs must be non-null.
	 */
	Seconds getShortestPropagationDelayBetween(Node *node1, Node *node2);
};