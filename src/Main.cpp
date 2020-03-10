#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "NetworkAnalyzer.hpp"
#include "UniformBusSimulatorConfigurer.hpp"

const bool REQUIRE_POSITIVE = true;
const bool ALLOW_NEGATIVE = false;

/**
 * Various design patterns and functions were retained from Lab 1.
 */

/**
 * Displays information on how to use this program and access each feature.
 */
void showUsage()
{
	std::cout 
		<< "ECE358-L2 Commands:\n\n"
		<< "t <simulation duration in seconds (double)>\n"
		<< "ni <use a single value of N (number of nodes on bus; long)>\n"
		<< "nl <N lower (long)>\n"
		<< "nu <N upper (long)>\n"
		<< "ns <N step size (long)>\n"
		<< "a <average frame arrival rate in frames per second (long)>\n"
		<< "r <channel transmission rate in bits per second (long)>\n"
		<< "l <frame length in bits for all frames (long)>\n"
		<< "d <distance between adjacent nodes on the bus in meters (double)>\n"
		<< "p <channel signal propagation speed in meters per second (double)>\n"
		<< "m <0 for persistent (default), any other (e.g. 1) for non-persistent CSMA/CD (long)>\n"
		<< "s - show the current settings\n"
		<< "oc - run the simulator and output CSV data to console\n"
		<< "of <filename without extension> - run the simulator and output CSV data to a file\n"
		<< "q - end the program>\n"
		<< "h - show this menu\n\n"
		<< "Inputs are error-checked and will revert the setting to its previous value if there is a problem.\n"
		<< "All inputs must be positive (except for m) and nonzero.\n"
		<< "There is currently no support for the representation of instantaneous propagation or transmission times.\n"
		<< "All settings start at a default, invalid (except for m) value of 0.\n"
		<< "If nl > nu, they will automatically be flipped once the simulation is started.\n\n"
		<< std::flush;
}

/**
 * Read a long integer from the console with error checking and message.
 * The specified longVariable is not updated unless the input string is successfully parsed.
 * Returns true if the read succeeded.
 * See https://stackoverflow.com/questions/24785362/converting-string-to-integer-double-float-without-having-to-catch-exceptions.
 */
bool getLong(long *longVariable, bool requirePositive, const char *errorMessage)
{
	std::string str;
	std::cin >> str;
	char *error_check;
	long longTemp = std::strtol(str.c_str(), &error_check, 10);

	if (*error_check || (requirePositive && (longTemp <= 0)))
	{
		std::cout << errorMessage << std::flush;
		return false;
	}

	*longVariable = longTemp;

	return true;
}

/**
 * Read a double-precision floating point number from the console with error checking and message.
 * The specified doubleVariable is not updated unless the input string is successfully parsed.
 * Returns true if the read succeeded.
 * See https://stackoverflow.com/questions/24785362/converting-string-to-integer-double-float-without-having-to-catch-exceptions.
 */
bool getDouble(double *doubleVariable, bool requirePositive, const char *errorMessage)
{
	std::string str;
	std::cin >> str;
	char *error_check;
	double doubleTemp = std::strtod(str.c_str(), &error_check);

	if (*error_check || (requirePositive && (doubleTemp <= 0)))
	{
		std::cout << errorMessage << std::flush;
		return false;
	}

	*doubleVariable = doubleTemp;

	return true;
}

/**
 * Double-checks that all the data is valid.
 */
bool validateSettings(
	Seconds simulationTime,
	Nodes nodesSingle,
	Nodes nodesLower,
	Nodes nodesUpper,
	Nodes nodesStep,
	FramesPerSecond averageFrameArrivalRate,
	BitsPerSecond channelTransmissionRate,
	Bits frameLength,
	Meters interNodeDistance,
	MetersPerSecond channelPropagationSpeed
)
{
	bool dataMissing = false;
	if (simulationTime <= 0)
	{
		std::cout << "t <simulation duration in seconds (double)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (nodesSingle <= 0)
	{
		// nodesSingle is mutually exclusive with nodesLower, nodesUpper and nodesStep
		bool useRangeOfNodes = true;
		if (nodesLower <= 0)
		{
			std::cout << "nl <N lower (long)> has not yet been successfully entered.\n\n";
			useRangeOfNodes = false;
		}
		if (nodesUpper <= 0)
		{
			std::cout << "nu <N upper (long)> has not yet been successfully entered.\n\n";
			useRangeOfNodes = false;
		}
		if (nodesStep <= 0)
		{
			std::cout << "ns <N step size (long)> has not yet been successfully entered.\n\n";
			useRangeOfNodes = false;
		}
		if (!useRangeOfNodes)
		{
			std::cout << "Alternatively, ni <use a single value of N (number of nodes on bus; long)> has not yet been successfully entered.\n\n";
			dataMissing = true;
		}
	}
	if (averageFrameArrivalRate <= 0)
	{
		std::cout << "a <average frame arrival rate in frames per second (long)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (channelTransmissionRate <= 0)
	{
		std::cout << "r <channel transmission rate in bits per second (long)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (frameLength <= 0)
	{
		std::cout << "l <frame length in bits for all frames (long)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (interNodeDistance <= 0)
	{
		std::cout << "d <distance between adjacent nodes on the bus in meters (double)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}
	if (channelPropagationSpeed <= 0)
	{
		std::cout << "p <channel signal propagation speed in meters per second (double)> has not yet been successfully entered.\n\n";
		dataMissing = true;
	}

	std::cout << std::flush;

	return !dataMissing;
}

/**
 * Attempts to run an analysis of packet queue performance using the specified settings and analyzer.
 * The results may be outputted either to the console or to a file.
 */
void runAnalysis(
	NetworkAnalyzer *analyzer,
	Seconds simulationDuration,
	Nodes nodesSingle,
	Nodes nodesLower,
	Nodes nodesUpper,
	Nodes nodesStep,
	FramesPerSecond averageFrameArrivalRate,
	BitsPerSecond channelTransmissionRate,
	Bits frameLength,
	Meters interNodeDistance,
	MetersPerSecond channelPropagationSpeed,
	bool persistent,
	bool outputToFile
)
{
	if (validateSettings(
		simulationDuration,
		nodesSingle,
		nodesLower,
		nodesUpper,
		nodesStep,
		averageFrameArrivalRate,
		channelTransmissionRate,
		frameLength,
		interNodeDistance,
		channelPropagationSpeed
	))
	{
		std::ostream *output = nullptr;
		std::ofstream *fileOutput = nullptr;

		// select between a console or file output stream
		if (outputToFile)
		{
			std::string filename;
			std::cin >> filename;
			filename.append(".csv");
			fileOutput = new std::ofstream(filename);
			output = fileOutput;
			std::cout << "Running the simulation using the current settings and outputting to " << filename << ":\n\n" << std::flush;
		}
		else
		{
			output = &std::cout;
			std::cout << "Running the simulation using the current settings and outputting to console:\n\n" << std::flush;
		}

		// write the current settings to the output as a CSV row with header
		*output
			<< "t,ni,nl,nu,ns,a,r,l,d,m\n"
			<< simulationDuration << ","
			<< nodesSingle << ","
			<< nodesLower << ","
			<< nodesUpper << ","
			<< nodesStep << ","
			<< averageFrameArrivalRate << ","
			<< channelTransmissionRate << ","
			<< frameLength << ","
			<< interNodeDistance << ","
			<< ((persistent) ? "" : "non-") << "persistent\n\n"
			<< std::flush;

		// add statistical data for each N (number of nodes) value as CSV rows with a header
		analyzer->writeHeaders(output);
		if (nodesSingle > 0)
		{
			analyzer->gatherDataFor(
				nodesSingle,
				output
			);
		}
		else
		{
			analyzer->gatherDataFor(
				nodesLower,
				nodesUpper,
				nodesStep,
				output
			);
		}

		// clean up the file output stream if needed
		if (outputToFile && (fileOutput != nullptr))
		{
			fileOutput->close();
			delete fileOutput;
		}

		std::cout << "Simulation execution complete.\n\n" << std::flush;
	}
}

/**
 * Encapsulates the main program loop for gathering data about different packet queue
 * configurations' performance.
 */
int main(int argc, char *argv[])
{
	showUsage();

	// the bus network state; the bus's parameters and contents may be updated throughout the session
	UniformBus bus;

	// runs events against the bus network's state; the simulation duration may be updated throughout the session
	NetworkSimulator simulator(&bus); 

	// configures network experiments for different numbers of nodes
	UniformBusSimulatorConfigurer configurer(&bus, &simulator);

	// uses the experiment configurer to generate statistical data with respect changing numbers of nodes
	NetworkAnalyzer analyzer(&configurer);

	// simulation settings; these may be updated throughout the session
	Nodes nodesSingle = 0;
	Nodes nodesLower = 0;
	Nodes nodesUpper = 0;
	Nodes nodesStep = 0;
	BitsPerSecond channelTransmissionRate = 0;
	Bits frameLength = 0;
	bool persistent = true;

	bool succeeded = false;

	// I/O loop implementing what is described by showUsage()
	while (true)
	{
		std::string str;
		char input;
		std::cin >> input;
		switch (input)
		{
		case 't': // set the simulation duration
			getDouble(&simulator.simulationDuration, REQUIRE_POSITIVE, "Invalid simulation duration (d); please enter a valid positive nonzero decimal value (double) in seconds.\n\n");
			break;
		case 'n': // set the number of nodes (N) to simulate
			std::cin >> input;
			switch (input)
			{
			case 'i': // use a single value of N
				succeeded = getLong(&nodesSingle, REQUIRE_POSITIVE, "Invalid single value for N (ni); please enter a valid positive nonzero value (double).\n\n");
				if(succeeded)
				{
					// this option is mutually exclusive of the others
					nodesLower = 0;
					nodesUpper = 0;
					nodesStep = 0;
				}
				break;
			case 'l': // set the lower bound for N
				succeeded = getLong(&nodesLower, REQUIRE_POSITIVE, "Invalid lower value for N (nl); please enter a valid positive nonzero value (double).\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					nodesSingle = 0;
				}
				break;
			case 'u': // set the upper bound for N
				succeeded = getLong(&nodesUpper, REQUIRE_POSITIVE, "Invalid upper value for N (nu); please enter a valid positive nonzero value (double).\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					nodesSingle = 0;
				}
				break;
			case 's': // set the step size for N
				succeeded = getLong(&nodesStep, REQUIRE_POSITIVE, "Invalid value for N step size (ns); please enter a valid positive nonzero value (double).\n\n");
				if (succeeded)
				{
					// this option is mutually exclusive of the first
					nodesSingle = 0;
				}
				break;
			default:
				std::cout << "Invalid command; enter \'h\' to show the list of commands.\n\n" << std::flush;
			}
			break;
		case 'a': // set the average frame arrival rate
			getLong(&bus.averageFrameArrivalRate, REQUIRE_POSITIVE, "Invalid average frame arrival rate (a); please enter a valid positive nonzero integer (long) in frames per second.\n\n");
			break;
		case 'r': // set the channel transmission rate
			getLong(&bus.channelTransmissionRate, REQUIRE_POSITIVE, "Invalid channel transmission rate (r); please enter a valid positive nonzero integer (long) in bits per second.\n\n");
			break;
		case 'l': // set the frame length for all frames
			getLong(&bus.frameLength, REQUIRE_POSITIVE, "Invalid frame length (l); please enter a valid positive nonzero integer (long) in bits.\n\n");
			break;
		case 'd': // set the inter-node distance
			getDouble(&bus.interNodeDistance, REQUIRE_POSITIVE, "Invalid inter-node distance (d); please enter a valid positive nonzero decimal value (double) in meters.\n\n");
			break;
		case 'p': // set the channel signal propagation speed
			getDouble(&bus.channelPropagationSpeed, REQUIRE_POSITIVE, "Invalid channel signal propagation speed (p); please enter a valid positive nonzero decimal value (double) in meters per second.\n\n");
			break;
		case 'm': // set the persistence mode (persistent versus non-persistent)
		{
			long persistenceModeSelect = 0;
			if (getLong(&persistenceModeSelect, ALLOW_NEGATIVE, "Invalid frame length (l); please enter a valid positive nonzero integer (long) in bits.\n\n"))
			{
				persistent = persistenceModeSelect == 0;
			}
		}
		case 's': // show the settings
			std::cout
				<< "Settings:\n\n"
				<< "ni <use a single value of N (number of nodes on bus; long)>: " << nodesSingle << "\n"
				<< "nl <N lower (long)>: " << nodesLower << "\n"
				<< "nu <N upper (long)>: " << nodesUpper << "\n"
				<< "ns <N step size (long)>: " << nodesStep << "\n"
				<< "a <average frame arrival rate in frames per second (long)>: " << bus.averageFrameArrivalRate << "\n"
				<< "r <channel transmission rate in bits per second (long)>: " << bus.channelTransmissionRate << "\n"
				<< "l <frame length in bits for all frames (long)>: " << bus.frameLength << "\n"
				<< "d <distance between adjacent nodes on the bus in meters (double)>: " << bus.interNodeDistance << "\n"
				<< "p <channel signal propagation speed in meters per second (double)>: " << bus.channelPropagationSpeed << "\n"
				<< "m <0 for persistent (default), any other (e.g. 1) for non-persistent CSMA/CD (long)>: " << ((persistent) ? "" : "non-") << "persistent\n"
				<< std::flush;
			break;
		case 'o': // run the simulation and output the results
		{
			bool successfullyParsed = true;
			bool outputToFile;

			std::cin >> input;
			switch (input)
			{
			case 'c': // output the CSV results to the console
				outputToFile = false;
				break;
			case 'f': // output the CSV results to a file
				outputToFile = true;
				break;
			default:
				std::cout << "Invalid command; enter \'h\' to show the list of commands.\n\n" << std::flush;
				successfullyParsed = false;
			}

			if (successfullyParsed)
			{
				// calculate the transmission time for all frames _once_ if possible
				if ((bus.channelTransmissionRate > 0) && (bus.frameLength > 0))
				{
					bus.frameTransmissionTime = ((double)bus.frameLength) / ((double)bus.channelTransmissionRate);
				}

				runAnalysis(
					&analyzer,
					simulator.simulationDuration,
					nodesSingle,
					nodesLower,
					nodesUpper,
					nodesStep,
					bus.averageFrameArrivalRate,
					channelTransmissionRate,
					frameLength,
					bus.interNodeDistance,
					bus.channelPropagationSpeed,
					persistent,
					outputToFile
				);
			}

			break;
		}
		case 'q': // end the program; let the destructors do their thing
			return 0;
		case 'h': // show the list of commands
			showUsage();
			break;
		default:
			std::cout << "Invalid command; enter \'h\' to show the list of commands.\n\n" << std::flush;
		}
	}
}