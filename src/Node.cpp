#include "Node.hpp"

/**
 * Adds a frame to this node's frame queue. It is assumed that frames will be
 * added in order such that the frame with the smallest timestamp is always
 * the first frame to be transmitted or dropped.
 */
void Node::addFrame(Frame frame)
{
	frames.push(frame);
	++totalFrames;
}

/**
 * Pops the frame at the front of this node's frame queue while incrementing the
 * transmitted frames counter.
 */
void Node::transmitFrame()
{
	Frame transmittedFrame = frames.front();
	totalTransmittedBits += transmittedFrame.length;
	frames.pop();
	++totalTransmittedFrames;
}

/**
 * Pops the frame at the front of this node's frame queue without incremending the
 * transmitted frames counter.
 */
void Node::dropFrame()
{
	frames.pop();
}

/**
 * Returns true if there are frames left in the queue to be transmitted.
 */
bool Node::hasFrames()
{
	bool result = !frames.empty();
	return result;
}

/**
 * Peeks the frame at the front of the frame queue.
 */
Frame Node::peekFrame()
{
	Frame top = frames.front();
	return top;
}

/**
 * Returns true if the node has detected that the channel is busy,
 * namely that there are signals incoming from other nodes.
 */
bool Node::channelIsBusy()
{
	bool isBusy = numIncomingSignals > 0;
	return isBusy;
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