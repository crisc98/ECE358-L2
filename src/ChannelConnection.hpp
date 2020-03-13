#pragma once

#include "Node.hpp"

typedef double Meters;
typedef double MetersPerSecond;
typedef double Seconds;
typedef long BitsPerSecond;

/**
 * Represents a node's connection to another node in a network graph.
 */
class ChannelConnection
{
public:

	/**
	 * The pointer to the node that the current node is being connected to on the channel.
	 */
	const Node *target;

	/**
	 * The number of bits per second that can be transmitted through the channel.
	 */
	const BitsPerSecond channelTransmissionRate;

	/**
	 * The length of the connection in meters.
	 */
	const Meters connectionLength;

	/**
	 * The speed at which information can propagate through the channel in meters per second.
	 */
	const MetersPerSecond channelPropagationSpeed;

	/**
	 * The amount of time that it takes for a message to travel across this connection
	 * from the current node to the target node.
	 */
	const Seconds channelPropagationDelay;

	ChannelConnection(
		Node *target,
		BitsPerSecond channelTransmissionRate,
		Meters connectionLength,
		MetersPerSecond channelPropagationSpeed
	) :
		target(target),
		channelTransmissionRate(channelTransmissionRate),
		connectionLength(connectionLength),
		channelPropagationSpeed(channelPropagationSpeed),
		channelPropagationDelay(connectionLength / channelPropagationSpeed)
	{
	}
	
	/**
	 * Calculates the time at which the first bit of a transmission from the current node
	 * would arrive at the target node.
	 */
	Seconds getTransmissionStartArrivalTime(Seconds startTime);

	/**
	 * Calculates the amount of time it takes to transmit the specified number of bits
	 * onto the channel.
	 */
	Seconds getTransmissionDelay(Bits bits);

	/**
	 * Calculates the time at which the last bit of a transmission from the current node
	 * would arrive at the target node.
	 */
	Seconds getTransmissionStopArrivalTime(Seconds startTime, Bits bits);
};