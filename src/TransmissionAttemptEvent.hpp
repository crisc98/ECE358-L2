#pragma once

#include "NodeEvent.hpp"
#include "TransmissionAttemptEventAcceptor.hpp"

/**
 * Represents the event of a node attempting to transmit a frame on the channel
 * or determining whether it can do so.
 */
typedef NodeEvent<TransmissionAttemptEventAcceptor> TransmissionAttemptEvent;