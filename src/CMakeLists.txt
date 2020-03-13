# CMakeList.txt : CMake project for ECE358-L2, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (
	ECE358-L2
	"Channel.cpp"
	"Channel.hpp"
	"ChannelBusyStartEvent.cpp"
	"ChannelBusyStartEvent.hpp"
	"ChannelBusyStartEventAcceptor.hpp"
	"ChannelBusyStopEvent.cpp"
	"ChannelBusyStopEvent.hpp"
	"ChannelBusyStopEventAcceptor.hpp"
	"ChannelConnection.cpp"
	"ChannelConnection.hpp"
	"CSMACDNode.cpp"
	"CSMACDNode.hpp"
	"CSMACDNodeFactory.cpp"
	"CSMACDNodeFactory.hpp"
#	"DiscreteEvent.cpp" Must be excluded: see https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp.
	"DiscreteEvent.hpp"
#	"DiscreteEventSimulator.cpp" Must be excluded: see https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp.
	"DiscreteEventSimulator.hpp"
	"ExponentialBackoff.cpp"
	"ExponentialBackoff.hpp"
	"Frame.hpp"
	"Main.cpp"
	"Network.cpp"
	"Network.hpp"
	"NetworkAnalyzer.cpp"
	"NetworkAnalyzer.hpp"
	"NetworkEvent.hpp"
	"NetworkSimulator.hpp"
	"NetworkSimulatorConfigurer.hpp"
	"Node.cpp"
	"Node.hpp"
	"NodeEvent.hpp"
	"NodeFactory.hpp"
	"TransmissionAttemptEvent.cpp"
	"TransmissionAttemptEvent.hpp"
	"TransmissionAttemptEventAcceptor.hpp"
	"TransmissionStopEvent.cpp"
	"TransmissionStopEvent.hpp"
	"TransmissionStopEventAcceptor.hpp"
	"UniformBus.hpp"
	"UniformBusSimulatorConfigurer.cpp"
	"UniformBusSimulatorConfigurer.hpp"
	)

# TODO: Add tests and install targets if needed.
