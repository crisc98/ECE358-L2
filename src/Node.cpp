#include "Node.hpp"

void Node::addFrame(Frame frame)
{
	frames.push(frame);
	++totalFrames;
}

void Node::transmitFrame()
{
	Frame transmittedFrame = frames.front();
	totalTransmittedBits += transmittedFrame.length;
	frames.pop();
	++totalTransmittedFrames;
}

void Node::dropFrame()
{
	frames.pop();
}

bool Node::hasFrames()
{
	bool result = !frames.empty();
	return result;
}

Frame Node::peekFrame()
{
	Frame top = frames.front();
	return top;
}

/**
 * Resets all of the measured statistics so that a new set of measurements may
 * be taken for a new set of packet queue events, leaving maxBufferSize and
 * transmissionRate unchanged; those may be edited manually.
 *
 * The maxBufferSize and transmissionRate MUST be set and this method called
 * before running a new simulation of the packet queue.
 */
void Node::flush()
{

}