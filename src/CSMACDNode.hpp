#pragma once

#include "ExponentialBackoff.hpp"
#include "Node.hpp"

/**
 * whereby the next transmission time is updated to
 * the maximum of the arrival time of the next frame and the departure time of the
 * frame that was just transmitted.
 */
class CSMACDNode : Node
{
private:

	/**
	 * True if this node should use persistent CSMA/CD, false if it should not.
	 */
	bool persistent;

	/**
	/

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
	 */
	bool shouldTransmit(
		Seconds checkTime,
		NetworkSimulator *simulator
	);

	/**
	 * Resets the default exponential backoff's collision counter upon a transmission
	 * success per the CSMA/CD MAC protocol.
	 */
	void acceptTransmissionStopEventImplementation(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);

public:

	/**
	 * Creates a node of the specified number that uses either the persistent (true)
	 * or non-persistent (false) implementation of the CSMA/CD MAC protocol.
	 */
	CSMACDNode(NodeNumber number, bool persistent) : Node(number)
	{
		if (persistent) nonPersistentBackoff = nullptr;
		else nonPersistentBackoff = new ExponentialBackoff();
	}

	/**
	 * Get the expected time at which the frame at the front of this node's frame queue
	 * will next sense the channel if the channel is currently free.
	 */
	Seconds getProjectedTimeOfTransmissionOnChannelFree();

	/**
	 * Get the expected time at which the frame at the front of this node's frame queue
	 * will next sense the channel if the channel is currently busy.
	 */
	Seconds getProjectedTimeOfTransmissionOnChannelBusy(BitsPerSecond channelTransmissionRate);

	/**
	 * Get the expected time at which the frame at the front of this node's frame queue
	 * will next sense the channel if a collision was detected.
	 */
	Seconds getProjectedTimeOfTransmissionOnCollision(BitsPerSecond channelTransmissionRate);
	
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