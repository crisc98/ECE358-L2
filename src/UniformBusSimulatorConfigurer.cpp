#include "UniformBusSimulatorConfigurer.hpp"

#include <random>

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

		// create events for the first attempts by each node to sense the channel
		Seconds projectedTimeOfTransmission = node->getProjectedTimeOfTransmissionOnChannelFree();
		ChannelSenseEvent channelSenseEvent = channelSenseEventfactory->createEvent(projectedTimeOfTransmission, node);
		simulator->addEvent(channelSenseEvent);

		/**
		 * Currently, there is no way to really know if a node experiences a collision
		 * a priori. This implementation requires that an additional type of event, say, a
		 * CSMACDStopTransmissionEvent, be created, implemented and registered so that
		 * the discrete event simulator can begin to dispatch CSMACDChannelBusyEndEvents
		 * to each node at the right time. The problem is that if we do register such an
		 * even from the start, at the same time that we register the CSMACDChannelSenseEvent,
		 * then if there is a collision, we need an elegant way to cancel that event, which
		 * may require changing up the priority queue to allow arbitrary removals and
		 * reinsertions.
		 */
	}
}