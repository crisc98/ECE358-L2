#include "UniformBusSimulatorConfigurer.hpp"

#include <algorithm>
#include <random>

#include "TransmissionAttemptEvent.hpp"

// put here to avoid a circular dependency
#include "Node.hpp"

/**
 * Returns a pointer to a simulation of a uniform bus with the specified number
 * of nodes.
 */
NetworkSimulator* UniformBusSimulatorConfigurer::configureNetworkSimulationFor(Nodes nodes)
{
	// initialize the simulator
	simulator->flush();
	simulator->network->reset();

	// initialize random number generation; see https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
	std::random_device rd;
	std::mt19937_64 gen(rd());

	// create nodes and populate them with frames

	/**
	 * The average seconds per frame (inter-arrival time) is equal to 1 / lambda.
	 * lambda happens to be the average frames per second, which we already have,
	 * so no additional calculation is needed.
	 */
	typedef double SecondsPerFrame;
	FramesPerSecond lambda = bus->averageFrameArrivalRate;
	std::exponential_distribution<SecondsPerFrame> interArrivalTime(lambda);

	Channel channel(bus->channelTransmissionRate, bus->channelPropagationSpeed);

	for (int i = 0; i < nodes; ++i)
	{
		Node *node = nodeFactory->createNode(i, channel);

		// give the network data structure the responsibility of destroying the node
		simulator->network->nodes.push_back(node);

		/**
		 * Populate the node with frames whose inter-arrival times follow an
		 * exponential distribution.
		 */
		Seconds lastArrivalTime = 0;
		while (true)
		{
			Seconds currentInterArrivalTime = interArrivalTime(gen);
			Seconds currentArrivalTime = lastArrivalTime + currentInterArrivalTime;
			lastArrivalTime = currentArrivalTime;
			if (simulator->isWithinSimulationDuration(currentArrivalTime))
			{
				Frame frame(currentArrivalTime,	bus->frameLength);
				node->addFrame(frame);
			}
			else
			{
				break;
			}
		}

		if (node->hasFrames())
		{
			// create the event for this node's first attempt to transmit to the channel
			Frame firstFrame = node->peekFrame();
			Seconds transmissionAttemptTime = firstFrame.arrivalTime;
			TransmissionAttemptEvent *transmissionAttemptEvent = new TransmissionAttemptEvent(transmissionAttemptTime, node);
			simulator->addEvent(transmissionAttemptEvent);
			++simulator->network->totalVisitedFrames;
			simulator->network->totalEnqueuedFrames += node->frameCount();
		}
	}

	// configure the network topology as a bus by creating the appropriate network edges
	for (Node *node : bus->nodes)
	{
		for (Node *targetNode : bus->nodes)
		{
			if (node != targetNode)
			{
				Nodes nodalDistance = std::abs(node->number - targetNode->number);
				Meters connectionLength = bus->interNodeDistance * ((double)nodalDistance);
				ChannelConnection *connection = new ChannelConnection(targetNode, connectionLength, bus->channelPropagationSpeed);
				node->connections.push_back(connection);
			}
		}
	}

	return simulator;
}