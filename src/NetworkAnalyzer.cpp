#include "PacketArrivalEvent.hpp"
#include "NetworkAnalyzer.hpp"
#include "PacketQueueObservationEvent.hpp"

/**
 * Adds the CSV headers to the start of the output.
 */
void NetworkAnalyzer::writeHeaders(std::ostream *output)
{
	*output << "nodes,totalTransmissionAttempts,totalTransmittedFrames,totalTransmittedBits,efficiency,throughput\n" << std::flush;
}

/**
 * Runs a simulation of the packet queue for the specified number of nodes.
 * 
 * For the specified rho, the specified settings are used to generate all of
 * the packet arrival and packet queue observation events to be added to the
 * simulator, after which the simulator is run, at which point packet departure
 * events may be dynamically generated. After the simulation has finished
 * running, the generated packet queue performance statistics are written to
 * a CSV row.
 */
void NetworkAnalyzer::gatherDataFor(
	Nodes nodes,
	std::ostream *output
)
	typedef double AveragePacketLength;
	typedef double AverageObservationsPerSecond;

	// initialize the simulator
	simulator->flush();
	simulator->network->reset();

	// initialize random number generation; see https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
	std::random_device rd;
	std::mt19937_64 gen(rd());

	// generate packet arrival events

	// generate packet queue observation events

	/**
	 * The average seconds per observation (inter-observational time) is equal to 1 / alpha.
	 */
	AverageObservationsPerSecond alpha = lambda * sampleRateFactor;

	std::exponential_distribution<FramesPerSecond> interObservationTime(alpha);
	
	/**
	 * Create packet queue observation events for observations whose inter-observation time
	 * follows an exponential distribution.
	 */
	Seconds lastObservationTime = 0;
	while (true)
	{
		Seconds currentInterObservationTime = interObservationTime(gen);
		Seconds currentObservationTime = lastObservationTime + currentInterObservationTime;
		lastObservationTime = currentObservationTime;
		if (simulator->isWithinSimulationDuration(currentObservationTime))
		{
			PacketQueueObservationEvent *observation = new PacketQueueObservationEvent(currentObservationTime);
			simulator->addEvent(observation);
		}
		else
		{
			break;
		}
	}

	/**
	 * Process all generated events until all packet queue performance statistics
	 * for the simulation's duration have been generated.
	 */
	simulator->runSimulation();

	// write the results to a CSV row
	*output
		<< rho << ","
		<< simulator->packetQueue->numArrivals << ","
		<< simulator->packetQueue->numDepartures << ","
		<< simulator->packetQueue->numDropped << ","
		<< simulator->packetQueue->numIdles << ","
		<< simulator->packetQueue->numObservations << ","
		<< simulator->packetQueue->getAverageBufferSize() << ","
		<< simulator->packetQueue->getQueueIdleRatio() << ","
		<< simulator->packetQueue->getPacketLossRatio() << "\n"
		<< std::flush;
}

/**
 * Runs a simulation of the network for the specified range of numbers of nodes.
 * 
 * See the other overload for more details.
 * 
 * Swaps nodesLower with nodesUpper if nodesLower is greater than nodesUpper.
 * 
 * Immediately aborts if nodesStep is 0.
 */
void NetworkAnalyzer::gatherDataFor(
	Nodes nodesLower,
	Nodes nodesUpper,
	Nodes nodesStep,
	std::ostream *output
)
{
	if (nodesStep == 0)
	{
		return;
	}

	if (nodesLower > nodesUpper)
	{
		std::swap(nodesLower, nodesUpper);
	}

	Nodes nodesCurrent = nodesLower;

	while (nodesCurrent <= nodesUpper)
	{
		gatherDataFor(
			nodesCurrent,
			output
		);

		nodesCurrent += nodesStep;
	}
}