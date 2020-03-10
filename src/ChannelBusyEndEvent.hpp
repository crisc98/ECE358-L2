#pragma once

#include "NetworkSimulator.hpp"

/**
 * Indicates an event at a node at which that node stops receiving activity
 * from another node. The channel at the node's location does not stop being
 * busy until all signals arriving from other nodes have gone quiet.
 */
class ChannelBusyEndEvent : NetworkEvent
{

};