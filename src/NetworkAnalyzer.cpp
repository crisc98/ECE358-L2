#include "NetworkAnalyzer.hpp"

/**
 * Adds the CSV headers to the start of the output.
 */
void NetworkAnalyzer::writeHeaders(std::ostream *output)
{
	*output
		<< "nodes,"
		<< "totalTransmissionAttempts(Tta),"
		<< "totalTransmittedFrames(Ttf),"
		<< "efficiency(Ttf/Tta),"
		<< "totalTransmittedBits(Ttb),"
		<< "throughput(Ttb/t),"
		<< "totalEnqueuedFrames(Tef),"
		<< "Ttf/Tef,"
		<< "totalVisitedFrames(Tvf),"
		<< "Tvf/Tef,"
		<< "Ttf/Tvf,"
		<< "totalChannelSenses(Tcs),"
		<< "Tta/Tcs,"
		<< "Ttf/Tcs,"
		<< "Tcs/Tvf,"
		<< "totalPostponedFrames(Tpf),"
		<< "totalWaits(Tw),"
		<< "Tw/Tpf,"
		<< "Tw/Tcs,"
		<< "Tw/Tvf,"
		<< "totalWaitTime(Twt),"
		<< "averageWaitTime(Twt/Tw),"
		<< "totalNonPersistentDrops(Tnpd),"
		<< "Tnpd/Tpf,"
		<< "Tnpd/Tvf,"
		<< "totalCollidedFrames(Tcf),"
		<< "totalDefaultBackoffs(Tdb),"
		<< "Tdb/Tcf,"
		<< "Tdb/Tcs,"
		<< "Tdb/Tvf,"
		<< "totalDefaultBackoffDelay(Tdbd),"
		<< "averageDefaultBackoffDelay(Tdbd/Tdb),"
		<< "totalCollisionDrops(Tcd),"
		<< "Tcd/Tcf,"
		<< "Tcd/Tvf"
		<< std::endl;
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
{
	// initialize the simulator for the specified number of nodes
	NetworkSimulator *simulator = configurer->configureNetworkSimulationFor(nodes);

	/**
	 * Process all generated events until all network performance statistics
	 * for the simulation's duration have been generated.
	 */
	simulator->runSimulation();

	// write the results to a CSV row
	*output
		<< nodes << ","
		<< simulator->network->totalTransmissionAttempts << "," // "totalTransmissionAttempts(Tta),"
		<< simulator->network->totalTransmittedFrames << "," // "totalTransmittedFrames(Ttf),"
		<< simulator->network->getEfficiency() << "," // "efficiency(Ttf/Tta),"
		<< simulator->network->totalTransmittedBits << "," // "totalTransmittedBits(Ttb),"
		<< simulator->network->getThroughput(simulator->simulationDuration) << "," // "throughput(Ttb/t),"
		<< simulator->network->totalEnqueuedFrames << "," // "totalEnqueuedFrames(Tef),"
		<< (((double)simulator->network->totalTransmittedFrames) / ((double)simulator->network->totalEnqueuedFrames)) << "," // "Ttf/Tef,"
		<< simulator->network->totalVisitedFrames << "," // "totalVisitedFrames(Tvf),"
		<< (((double)simulator->network->totalVisitedFrames) / ((double)simulator->network->totalEnqueuedFrames)) << "," // "Tvf/Tef,"
		<< (((double)simulator->network->totalTransmittedFrames) / ((double)simulator->network->totalVisitedFrames)) << "," // "Ttf/Tvf,"
		<< simulator->network->totalChannelSenses << "," // "totalChannelSenses(Tcs),"
		<< (((double)simulator->network->totalTransmissionAttempts) / ((double)simulator->network->totalChannelSenses)) << "," // "Tta/Tcs,"
		<< (((double)simulator->network->totalTransmittedFrames) / ((double)simulator->network->totalChannelSenses)) << "," // "Ttf/Tcs,"
		<< (((double)simulator->network->totalChannelSenses) / ((double)simulator->network->totalVisitedFrames)) << "," // "Tcs/Tvf,"
		<< simulator->network->totalPostponedFrames << "," // totalPostponedFrames(Tpf),"
		<< simulator->network->totalWaits << "," // "totalWaits(Tw),"
		<< (((double)simulator->network->totalWaits) / ((double)simulator->network->totalPostponedFrames)) << "," // "Tw/Tcf,"
		<< (((double)simulator->network->totalWaits) / ((double)simulator->network->totalChannelSenses)) << "," // "Tw/Tcs,"
		<< (((double)simulator->network->totalWaits) / ((double)simulator->network->totalVisitedFrames)) << "," // "Tw/Tvf,"
		<< simulator->network->totalWaitTime << "," // "totalWaitTime(Twt),"
		<< (simulator->network->totalWaitTime / ((double)simulator->network->totalWaits)) << "," // "averageWaitTime(Twt/Tw),"
		<< simulator->network->totalNonPersistentDrops << "," // "totalNonPersistentDrops(Tnpd),"
		<< (((double)simulator->network->totalNonPersistentDrops) / ((double)simulator->network->totalPostponedFrames)) << "," // "Tnpd/Tpf,"
		<< (((double)simulator->network->totalNonPersistentDrops) / ((double)simulator->network->totalVisitedFrames)) << "," // "Tnpd/Tvf,"
		<< simulator->network->totalCollidedFrames << "," // "totalCollidedFrames(Tcf)"
		<< simulator->network->totalDefaultBackoffs << "," // "totalDefaultBackoffs(Tdb),"
		<< (((double)simulator->network->totalDefaultBackoffs) / ((double)simulator->network->totalCollidedFrames)) << "," // "Tdb/Tcf,"
		<< (((double)simulator->network->totalDefaultBackoffs) / ((double)simulator->network->totalChannelSenses)) << "," // "Tdb/Tcs,"
		<< (((double)simulator->network->totalDefaultBackoffs) / ((double)simulator->network->totalVisitedFrames)) << "," // "Tdb/Tvf,"
		<< simulator->network->totalDefaultBackoffDelay << "," // "totalDefaultBackoffDelay(Tdbd),"
		<< (simulator->network->totalDefaultBackoffDelay / ((double)simulator->network->totalDefaultBackoffs)) << "," // "averageDefaultBackoffDelay(Tdbd/Tdb),"
		<< simulator->network->totalCollisionDrops << "," // "totalCollisionDrops(Tcd),"
		<< (((double)simulator->network->totalCollisionDrops) / ((double)simulator->network->totalCollidedFrames)) << "," // "Tcd/Tcf"
		<< (((double)simulator->network->totalCollisionDrops) / ((double)simulator->network->totalVisitedFrames)) << "," // "Tcd/Tvf"
		<< std::endl;
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
	if (nodesStep == 0) return;

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