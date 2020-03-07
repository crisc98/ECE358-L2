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
	 * A unique identifier for the
	 */
	NodeNumber number;

public:

	/**
	 * The total number of frames to have been generated at this node within the simulation's duration.
	 */
	Frames totalFrames;

	/**
	 * The total number of frames to actually be successfully transmitted.
	 */
	Frames totalTransmittedFrames;
};