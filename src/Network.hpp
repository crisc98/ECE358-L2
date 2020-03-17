#pragma once

#include <vector>

// forward declaration to avoid a circular dependency
class Node;

typedef double AverageBitsPerSecond;
typedef double Bits;
typedef double Percentage;
typedef double Seconds;
typedef long Frames;
typedef long Times;

/**
 * Represents a graph of nodes arranged in a particular network topology.
 * 
 * This interface is not responsible for the actual configuration of the nodes
 * in a network and the propagation delays between them; this is delegated to
 * specific implementations of this interface (e.g. a bus implementation or an
 * arbitrary graph) or some network factory.
 */
class Network
{
public:

	/**
	 * The set of nodes in this network.
	 * To be populated externally.
	 * Cleared via the reset() method which likewise destroys all the nodes.
	 */
	std::vector<Node*> nodes;

	// ***Default data***

	/**
	 * The total number of attempts across all nodes to transmit a frame through the channel without collision.
	 */
	Frames totalTransmissionAttempts;

	/**
	 * The total number of frames to actually be fully and successfully transmitted.
	 */
	Frames totalTransmittedFrames;

	/**
	 * The total number of bits successfully transmitted by the nodes in this network.
	 */
	Bits totalTransmittedBits;

	// ***Extra data***

	/**
	 * The total number of frames to have been enqueued across all nodes.
	 */
	Frames totalEnqueuedFrames;

	/**
	 * The total number of frames out of all enqueued that were attempted to be transmitted.
	 */
	Frames totalVisitedFrames;

	/**
	 * The total number of times the channel was checked for whether it was busy.
	 */
	Times totalChannelSenses;

	/**
	 * The total number of frames for which the channel was initially sensed as busy.
	 */
	Frames totalPostponedFrames;

	/**
	 * The total number of times spent waiting for the channel to go idle in the persistent case
	 * or for the channel busy exponential backoff to expire in the non-persistent case.
	 */
	Times totalWaits;

	/**
	 * The total number of seconds spent waiting for the channel to go idle in the persistent case
	 * or for the channel busy exponential backoff to expire in the non-persistent case.
	 */
	Seconds totalWaitTime;

	/**
	 * The total number of frames in the non-persistent case to be dropped after exceeding the maximum
	 * number of consecutive times to sense the channel as being busy.
	 */
	Frames totalNonPersistentDrops;

	/**
	 * The total number of frames for which at least one collision was detected when attempting to be transmitted.
	 */
	Frames totalCollidedFrames;

	/**
	 * The total number of collisions for which an exponential backoff was performed.
	 */
	Times totalDefaultBackoffs;

	/**
	 * The total number of seconds spent in the default exponential backoff.
	 */
	Seconds totalDefaultBackoffDelay;

	/**
	 * The total number of frames dropped due to exceeding the maximum number of consecutive collisions.
	 */
	Frames totalCollisionDrops;

	// ***Default statistical calculations***

	/**
	 * Calculates the ratio of the total number of successful transmissions over the
	 * total number of transmission attempts across all nodes in the network.
	 */
	Percentage getEfficiency();

	/**
	 * Calculates the average number of bits per second throughput perceived throughout
	 * the simulation's duration.
	 */
	AverageBitsPerSecond getThroughput(Seconds simulationDuration);
	
	/**
	 * Removes all nodes from the network and resets all of the statistical counters.
	 * Must be called before running a simulation on this network or configuring its nodes.
	 */
	void reset();

	/**
	 * Ensures that all registered nodes in the network are destroyed once this class is destroyed.
	 */
	~Network()
	{
		reset();
	}
};