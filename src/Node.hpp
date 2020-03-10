#pragma once

#include <queue>

#include "Frame.hpp"

typedef long BitsPerSecond;
typedef long Frames;
typedef long NodeNumber;
typedef long Nodes;

/**
 * Represents a node within a network.
 */
class Node
{
private:

	std::queue<Frame> frames;

	/**
	 *
	 */
	Seconds timeOfLastTransmission;

public:

	/**
	 * A unique identifier for the node; generally 0-indexed.
	 */
	NodeNumber number;

	/**
	 * The number of nodes whose channel assertions have reached this node after
	 * their respective propagation delays.
	 *
	 * This is 0 or greater, whereby a value of 1 or greater indicates that the
	 * channel is busy at this node.
	 */
	Nodes busyCount;

	/**
	 * The total number of frames to have been generated at this node within the simulation's duration.
	 */
	Frames totalFrames;

	/**
	 * The total number of times that this node attempted to transmit a frame.
	 */
	Frames totalTransmissionAttempts;

	/**
	 * The total number of frames to actually be successfully transmitted.
	 */
	Frames totalTransmittedFrames;

	/**
	 * The total number of bits successfully transmitted by this node.
	 */
	Bits totalTransmittedBits;

	/**
	 * The projected time at which the frame at the front of this node's frame queue will be transmitted.
	 */
	Seconds projectedTimeOfTransmission;
	
	/**
	 * Adds a frame to this node's frame queue. It is assumed that frames will be
	 * added in order such that the frame with the smallest timestamp is always
	 * the first frame to be transmitted or dropped.
	 */
	void addFrame(Frame frame);

	/**
	 * Pops the frame at the front of this node's frame queue while incrementing the
	 * transmitted frames counter.
	 */
	void transmitFrame();

	/**
	 * Pops the frame at the front of this node's frame queue without incremending the
	 * transmitted frames counter.
	 */
	void dropFrame();

	/**
	 * Returns true if there are frames left in the queue to be transmitted.
	 */
	bool hasFrames();

	/**
	 * Peeks the frame at the front of the frame queue.
	 */
	Frame peekFrame();

	/**
	 * Removes all frames from this node and resets all of the statistical counters.
	 */
	void flush();

	/**
	 * Initializes the node's number and initializes all other fields to zero.
	 */
	Node(NodeNumber number) :
		number(number),
		busyCount(0),
		totalFrames(0),
		totalTransmissionAttempts(0),
		totalTransmittedFrames(0),
		totalTransmittedBits(0),
		projectedTimeOfTransmission(0)
	{

	}

	/**
	 * Updates the time at which the frame at the front of this node's frame queue
	 * will be transmitted based on whether the last time of transmission would have
	 * caused a collision.
	 * 
	 * If no collision was detected, the frame is transmitted and the transmitted frames
	 * counter is incremented, whereby the underlying MAC protocol implementation will
	 * decide the time to attempt transmitting the next frame.
	 * 
	 * If a collision is detected, the underlying MAC protocol implementation will decide
	 * the next time to attempt transmitting the current frame or whether the frame
	 * should be dropped.
	 */
	virtual void updateProjectedTimeOfTransmission(
		bool collisionDetected,
		Seconds channelBusyStart,
		Seconds channelBusyEnd);

	/**
	 * Compares pointers to nodes based on the time at which they are next
	 * scheduled to transmit.
	 * Namely, in a priority_queue, this comparator will always place the
	 * _earliest_ node to transmit at the top of the heap.
	 */
	class Comparator
	{
	public:

		bool operator()(Node* lhs, Node* rhs)
		{
			bool result = lhs->projectedTimeOfTransmission > rhs->projectedTimeOfTransmission;
			return result;
		}
	};
};