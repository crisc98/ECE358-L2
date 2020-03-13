#pragma once

// forward declaration to avoid a circular dependency
class Node;

typedef double Meters;
typedef double MetersPerSecond;
typedef double Seconds;

/**
 * Represents a node's connection to another node in a network graph.
 */
class ChannelConnection
{
public:

	/**
	 * The pointer to the node that the current node is being connected to on the channel.
	 */
	Node *target;

	/**
	 * The length of the connection in meters.
	 */
	const Meters connectionLength;

	/**
	 * The amount of time that it takes for a message to travel across this connection
	 * from the current node to the target node.
	 */
	const Seconds channelPropagationDelay;

	ChannelConnection(
		Node *target,
		Meters connectionLength,
		MetersPerSecond channelPropagationSpeed
	) :
		target(target),
		connectionLength(connectionLength),
		channelPropagationDelay(connectionLength / channelPropagationSpeed)
	{
	}

	/**
	 * ~Calculates the time at which the first bit of a transmission (or _any_ signal, such
	 * as the assertion or deassertion of a signal) from the current node would arrive at
	 * the target node.
	 */
	Seconds getSignalArrivalTime(Seconds startTime);
};