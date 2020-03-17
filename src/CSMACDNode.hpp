#pragma once

#include "ExponentialBackoff.hpp"
#include "Node.hpp"

/**
 * whereby the next transmission time is updated to
 * the maximum of the arrival time of the next frame and the departure time of the
 * frame that was just transmitted.
 */
class CSMACDNode : public Node
{
private:

	/**
	 * True if this node should use persistent CSMA/CD, false if it should not.
	 */
	bool persistent;

	/**
	 * Indicates whether the node is currently waiting for the channel to go idle in
	 * the persistent case.
	 */
	bool isCurrentlyWaiting;

	/**
	 * The time in the persistent case that the node last started waiting for the channel
	 * to become busy.
	 */
	Seconds lastWaitStart;

	/**
	 * The exponential backoff logic and state shared between the persistent and
	 * non-persistent CSMA/CD MAC protocol implementations.
	 */
	ExponentialBackoff defaultBackoff;

	/**
	 * The exponential backoff logic and state used only 
	 */
	ExponentialBackoff *nonPersistentBackoff;
	
	/**
	 * Performs an exponential backoff using the specific state instance and registers
	 * a new transmission attempt event to be processed after the backoff has finished.
	 * 
	 * Returns the backoff delay, else a -1 if the number of collisions has already been
	 * exceeded.
	 */
	Seconds performExponentialBackoff(
		Seconds backoffStart,
		ExponentialBackoff *backoffState,
		NetworkSimulator *simulator
	);

	/**
	 * The CSMA/CD MAC protocol implementation for collision detection.
	 */
	void acceptChannelBusyStartEventImplementation(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);

	/**
	 * The CSMA/CD MAC protocol implementation for checking if the channel whether the
	 * channel went idle and responding accordingly.
	 */
	void acceptChannelBusyStopEventImplementation(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);
	
	/**
	 * The CSMA/CD MAC protocol implementation for sensing the carrier.
	 * Assigns a value to the passed processingDelay reference.
	 */
	bool shouldTransmit(
		Seconds checkTime,
		Seconds &processingDelay,
		NetworkSimulator *simulator
	);

	/**
	 * Resets the default exponential backoff's collision counter upon a transmission
	 * success per the CSMA/CD MAC protocol.
	 */
	void acceptTransmissionStopEventImplementation(
		Seconds transmissionStopTime,
		NetworkSimulator *simulator
	);

public:

	/**
	 * Creates a node of the specified number that uses either the persistent (true)
	 * or non-persistent (false) implementation of the CSMA/CD MAC protocol.
	 */
	CSMACDNode(
		NodeNumber number,
		Channel channel,
		bool persistent,
		Collisions maxCollisions
	) : 
		Node(number, channel),
		isCurrentlyWaiting(false),
		defaultBackoff(maxCollisions)
	{
		if (persistent) nonPersistentBackoff = nullptr;
		else nonPersistentBackoff = new ExponentialBackoff(maxCollisions);
	}
	
	/**
	 * Destroy the ExponentialBackoff instance for the non-persistent case if needed.
	 */
	~CSMACDNode()
	{
		if (nonPersistentBackoff != nullptr)
		{
			delete nonPersistentBackoff;
			nonPersistentBackoff = nullptr;
		}
	}
};