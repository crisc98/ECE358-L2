#pragma once

#include "NetworkSimulator.hpp"

/**
 * Represents an event that occurs somewhere in a network and performs
 * an operation on that network's state.
 */
typedef DiscreteEvent<Seconds, NetworkSimulator> NetworkEvent;