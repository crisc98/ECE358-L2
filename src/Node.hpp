#pragma once

#include <queue>
#include <vector>

#include "Channel.hpp"
#include "ChannelBusyStartEvent.hpp"
#include "ChannelBusyStopEvent.hpp"
#include "ChannelConnection.hpp"
#include "Frame.hpp"
#include "TransmissionAttemptEvent.hpp"
#include "TransmissionStopEvent.hpp"

typedef long Frames;
typedef long NodeNumber;
typedef long Nodes;

/**
 * Represents a node within a network, namely one connected to one channel
 * that can only have one signal on it at a time for a transmission to be valid
 * (signals must not interfere or intersect, else they will be corrupted).
 */
class Node :
	public ChannelBusyStartEventAcceptor,
	public ChannelBusyStopEventAcceptor,
	public TransmissionAttemptEventAcceptor,
	public TransmissionStopEventAcceptor
{
private:

	/**
	 * The set of frames that have been scheduled to arrive at and be transmitted
	 * by this node at a particular minimum time.
	 */
	std::queue<Frame> frames;

	/**
	 *
	 */
	Seconds timeOfLastTransmission;

protected:
	
	/**
	 * The parameters of the channel that this node is connected to.
	 */
	Channel *channel;
	
	/**
	 * The number of nodes (excluding this node) whose channel assertions have
	 * reached and are passing through this node after their respective propagation
	 * delays.
	 *
	 * This is 0 or greater, whereby a value of 1 or greater indicates that the
	 * channel is busy at this node, while a value of 0 indicates that it is
	 * currently idle.
	 */
	Nodes numIncomingSignals;

	/**
	 * True if this node is currently in the process of transmitting a frame.
	 */
	bool isCurrentlyTransmitting;

	/**
	 * A pointer to the currently scheduled event for the default time at which the
	 * node's current transmission will have been completed.
	 */
	TransmissionStopEvent *currentTransmissionStopEvent;

	/**
	 * Any operations to be performed by a specific MAC protocol when a node starts
	 * receiving signals transmitted from another node; e.g. detect a collision.
	 */
	virtual void acceptChannelBusyStartEventImplementation(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	) = 0;

	/**
	 * Any operations to be performed by a specific MAC protocol when a node stops
	 * receiving signals transmitted from another node; e.g. detect the channel as
	 * having gone idle.
	 */
	virtual void acceptChannelBusyStopEventImplementation(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	) = 0;

	/**
	 * Delegates to the specific MAC protocol implementation whether the node should
	 * attempt to transmit its current frame, and what to do if it can or can't.
	 */
	virtual bool shouldTransmit(
		Seconds checkTime,
		NetworkSimulator *simulator
	) = 0;

	/**
	 * Any operations to be performed by a specific MAC protocol after the current
	 * frame has been fully transmitted.
	 */
	virtual void acceptTransmissionStopEventImplementation(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	) = 0;
	
	/**
	 * Attempts to transmit the current frame at the front of the frame queue;
	 * this is _after_ the channel has already been senses as being idle.
	 */
	void attemptTransmission(
		Seconds attemptTime,
		NetworkSimulator *simulator
	);

	/**
	 * Generates the ChannelBusyStartEvent instances for all the nodes this node is
	 * connected to on the channel, in effect telling them all when the first bit
	 * of its transmission has arrived at and started asserting their location on
	 * the channel.
	 */
	void startTransmission(
		Seconds transmissionStartTime,
		NetworkSimulator *simulator
	);

	/**
	 * Generates the ChannelBusyStopEvent instances for all the nodes this node is
	 * connected to on the channel, in effect telling them all when the last bit
	 * of its transmission has arrived at and stopped asserting their location on
	 * the channel.
	 */
	void stopTransmission(
		Seconds transmissionStopTime,
		NetworkSimulator *simulator
	);

	/**
	 * Removes the frame that was last attempted to be transmitted from the frame queue.
	 */
	void popFrame(
		Seconds transmissionStopTime,
		NetworkSimulator *simulator
	);

public:

	/**
	 * A unique identifier for the node; generally 0-indexed.
	 */
	NodeNumber number;

	/**
	 * The set of connections between this node and other nodes in the network graph.
	 */
	std::vector<ChannelConnection*> connections;

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
	 * Initializes the node's number and initializes all other fields to zero.
	 */
	Node(NodeNumber number, Channel *channel) :
		number(number),
		channel(channel),
		numIncomingSignals(0),
		totalFrames(0),
		totalTransmissionAttempts(0),
		totalTransmittedFrames(0),
		totalTransmittedBits(0),
		projectedTimeOfTransmission(0),
		currentTransmissionStopEvent(nullptr)
	{
	}
		
	/**
	 * ~Get the expected time at which the frame at the front of this node's frame queue
	 * will next sense the channel if the channel is currently free.
	 */
	virtual Seconds getProjectedTimeOfTransmissionOnChannelFree() = 0;

	/**
	 * ~Get the expected time at which the frame at the front of this node's frame queue
	 * will next sense the channel if the channel is currently busy.
	 */
	virtual Seconds getProjectedTimeOfTransmissionOnChannelBusy(BitsPerSecond channelTransmissionRate) = 0;

	/**
	 * ~Get the expected time at which the frame at the front of this node's frame queue
	 * will next sense the channel if a collision was detected.
	 */
	virtual Seconds getProjectedTimeOfTransmissionOnCollision(BitsPerSecond channelTransmissionRate) = 0;
	
	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when an incoming starts asserting itself at the accepting
	 * node's location on the channel.
	 */
	void acceptChannelBusyStartEvent(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);
	
	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when an incoming signal stops asserting itself at the accepting
	 * node's location on the channel.
	 */
	void acceptChannelBusyStopEvent(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);

	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when it reaches the point in time at which it should attempt to
	 * transmit a frame.
	 */
	void acceptTransmissionAttemptEvent(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);

	/**
	 * Updates the simulation state in accordance with how a particular MAC protocol
	 * should respond when it reaches the point in time at which it should stop or
	 * finish transmitting.
	 */
	void acceptTransmissionStopEvent(
		Seconds eventArrivalTime,
		NetworkSimulator *simulator
	);

	/**
	 * Adds a frame to this node's frame queue. It is assumed that frames will be
	 * added in order such that the frame with the smallest timestamp is always
	 * the first frame to be transmitted or dropped.
	 */
	void addFrame(Frame frame);

	/**
	 * Returns true if there are frames left in the queue to be transmitted.
	 */
	bool hasFrames();

	/**
	 * Peeks the frame at the front of the frame queue.
	 */
	Frame peekFrame();

	/**
	 * Returns true if the node has detected that the channel is busy.
	 */
	bool channelIsBusy();
};