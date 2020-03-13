#pragma once

#include "ChannelBusyStartEventAcceptor.hpp"
#include "NodeEvent.hpp"

/**
 * Indicates an event at a node at which that node should start sensing
 * the channel as being busy.
 */
typedef NodeEvent<ChannelBusyStartEventAcceptor> ChannelBusyStartEvent;