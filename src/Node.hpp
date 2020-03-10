#pragma once

#include <queue>

#include "Frame.hpp"

typedef long Frames;
typedef long NodeNumber;

/**
 * Represents a node within a network.
 */
class Node
{
private:

	std::queue<Frame> frames;
	
	/**
	 * The total number of frames to have been generated at this node within the simulation's duration.
	 */
	Frames totalFrames;

	/**
	 * The total number of frames to actually be successfully transmitted.
	 */
	Frames totalTransmittedFrames;

	/**
	 *
	 */
	Seconds timeOfLastTransmission;

public:

	/**
	 * A unique identifier for the node.
	 */
	NodeNumber number;

	Seconds nextTransmissionTime;

	void addFrame(Frame frame);

	/**
	 * Pops the frame at the top of this node's frame queue.
	 */
	void popFrame();

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
			bool result = lhs->nextTransmissionTime > rhs->nextTransmissionTime;
			return result;
		}
	};
};