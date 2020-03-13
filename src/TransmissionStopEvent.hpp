#pragma once

#include "NodeEvent.hpp"
#include "TransmissionStopEventAcceptor.hpp"

/**
 * Represents the event at which a node has completely finished transmitting
 * its current frame.
 */
typedef NodeEvent<TransmissionStopEventAcceptor> TransmissionStopEvent;