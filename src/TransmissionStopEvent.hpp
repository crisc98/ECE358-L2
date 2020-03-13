#pragma once

#include "NodeEvent.hpp"
#include "TransmissionStopEventAcceptor.hpp"

/**
 * Represents the event at which a node should stop of finish transmitting
 * the current frame.
 */
typedef NodeEvent<TransmissionStopEventAcceptor> TransmissionStopEvent;