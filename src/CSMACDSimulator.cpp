#include "CSMACDSimulator.hpp"

/**
 * Transforms the network's nodes list into a min heap.
 */
void sortNodes()
{
	std::make_heap(network->nodes.begin(), network->nodes.end(), Node::Comparator());
}

/**
 * Obtains the pointer to the node at the top of the min heap; this denotes the
 * current sender node, namely that whose frame at the top of its frame queue
 * has the earliest arrival time or scheduled transmission time out of that of
 * all the other nodes.
 */
Node* CSMACDSimulator::getSenderNode()
{
	Node* senderNode = network->nodes.front();
	return senderNode;
}

/**
 * After the frame at the top of the sender node's frame queue has been successfully
 * transmitted without collision, or if it has been dropped, it is removed from the
 * queue such that that node's position in the min heap of nodes must be updated.
 * That way, the next sender node to attempt to transmit a frame can be properly
 * determined.
 */
void CSMACDSimulator::popSenderFrame()
{
	Node* senderNode = getSenderNode();
	std::pop_heap(network->nodes.begin(), network->nodes.end(), Node::Comparator());
	network->nodes.pop_back();
	senderNode->popFrame();
	network->nodes.push_back(senderNode);
}

/**
 * Returns true if the specified time is between 0 seconds and the current simulation duration.
 */
bool CSMACDSimulator::isWithinSimulationDuration(Seconds time)
{
	bool result = (time >= 0) && (time <= simulationDuration);
	return result;
}

/**
 * Adds an event to be processed at a particular time with respect to other events.
 *
 * Returns true if the specified event's time is within the simulation time;
 * if this is the case, This class takes _sole responsibility_ over the destruction of
 * the pointer passed in.
 *
 * If false is returned, it is up to the event's creator to delete it, whereby this
 * class _does not_ take responsibility for the destruction of the pointer passed in.
 */
/*
bool CSMACDSimulator::addEvent(PacketQueueEvent *event)
{
	if (isWithinSimulationDuration(event->time))
	{
		eventQueue.push(event);
		return true;
	}
	
	return false;
}
*/

/**
 * Processes all of the events that were registered for this simulation;
 * some events may register further events to be processed in the future.
 */
void CSMACDSimulator::runSimulation()
{
	sortNodes();

	while (true)
	{
		Node *senderNode = getSenderNode();

		// perform carrier sensing for each non-transmitting node
		for (Node* node : network->nodes)
		{

		}
	}

	/*
	while (!eventQueue.empty())
	{
		PacketQueueEvent *event = eventQueue.top();
		eventQueue.pop();
		event->process(this);
		delete event;
	}
	*/
}

/**
 * Removes and deletes all registered events without processing them.
 */
void CSMACDSimulator::flush()
{
	while (!eventQueue.empty())
	{
		PacketQueueEvent *event = eventQueue.top();
		eventQueue.pop();
		delete event;
	}
}