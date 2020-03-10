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
	 * The total amount of time required to transmit a frame from any node on this bus.
	 * This is constant and hence the only piece of information needed since the channel
	 * transmission rate and frame length is assumed to be constant throughout the network.
	 */
	Seconds frameTransmissionTime;

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
	Bus() :
		averageFrameArrivalRate(0),
		frameTransmissionTime(0),
		interNodeDistance(0),
		channelPropagationSpeed(0)
	{
	}
	
	/**
	 * Add the specified number of nodes to the bus, 
	 */
	void addNodes(Nodes nodes, );

	/**
	 * Calculates the time required to fully transmit the frame currently at the front
	 * of the specified node's frame queue.
	 */
	Seconds getTransmissionDelayOf(Node *node);

	/**
	 * Calculates the shortest propagation delay between two nodes in a bus configuration.
	 * The inputs must be non-null.
	 */
	Seconds getShortestPropagationDelayBetween(Node *node1, Node *node2);
};