#include "UniformBusSimulatorConfigurer.hpp"

#include <random>

#include "TransmissionAttemptEvent.hpp"

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
	FramesPerSecond lambda = uniform;
	std::exponential_distribution<FramesPerSecond> interArrivalTime(lambda);

	for (int i = 0; i < nodes; ++i)
	{
		Node *node = nodeFactory->createNode(i);

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
			TransmissionAttemptEvent transmissionAttemptEvent = new TransmissionAttemptEvent(transmissionAttemptTime, node);
			simulator->addEvent(transmissionAttemptEvent);
		}
	}
}