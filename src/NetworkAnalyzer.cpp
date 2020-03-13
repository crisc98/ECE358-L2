#include "NetworkAnalyzer.hpp"

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
		<< simulator->network->totalTransmissionAttempts << ","
		<< simulator->network->totalTransmittedFrames << ","
		<< simulator->network->totalTransmittedBits << ","
		<< simulator->network->getEfficiency() << ","
		<< simulator->network->getThroughput(simulator->simulationDuration) << "\n"
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