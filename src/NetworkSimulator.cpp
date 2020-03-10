#include "NetworkSimulator.hpp"

/**
 * Transforms the network's nodes list into a min heap.
 */
void sortNodes()
{
	std::make_heap(network->nodes.begin(), network->nodes.end(), Node::Comparator());
}

/**
 * Obtains the pointer to the node at the top of the min heap; this denotes the
 * current sender node, namely that whose frame at the front of its frame queue
 * has the earliest arrival time or scheduled transmission time out of that of
 * all the other nodes.
 */
Node* NetworkSimulator::getSenderNode()
{
	Node* senderNode = network->nodes.front();
	return senderNode;
}

/**
 * After the frame at the front of the sender node's frame queue has been successfully
 * transmitted without collision, or if it has been dropped, it is removed from the
 * queue such that that node's position in the min heap of nodes must be updated.
 * That way, the next sender node to attempt to transmit a frame can be properly
 * determined.
 * 
 * If the node has no more frames, it is removed from the queue entirely.
 * This method is partly responsible for disposing of the elements of the node min heap.
 */
void NetworkSimulator::popSenderFrame(bool transmittedOrDropped)
{
	Node* senderNode = getSenderNode();

	std::pop_heap(network->nodes.begin(), network->nodes.end(), Node::Comparator());
	network->nodes.pop_back();

	if(transmittedOrDropped == TRANSMITTED)	senderNode->transmitFrame();
	else if (transmittedOrDropped == DROPPED) senderNode->dropFrame();
	
	if (senderNode->hasFrames())
	{
		network->nodes.push_back(senderNode);
		std::push_heap(network->nodes.begin(), network->nodes.end(), Node::Comparator());
	}
	else
	{
		delete senderNode;
	}
}

bool NetworkSimulator::attemptTransmission()
{
	Node *senderNode = getSenderNode();

	// perform carrier sensing for each non-transmitting node
	for (Node* node : network->nodes)
	{
		if (node != senderNode)
		{
			Seconds propagationDelay = network->getShortestPropagationDelayBetween(senderNode, node);
			Seconds channelBusyStart = senderNode->projectedTimeOfTransmission + propagationDelay;
			Seconds transmissionDelay = network->getTransmissionDelayOf(node);
			Seconds channelBusyEnd = channelBusyStart + transmissionDelay;

			bool isBusyWhileSensing =
				(node->projectedTimeOfTransmission >= channelBusyStart) &&
				(node->projectedTimeOfTransmission <= channelBusyEnd);

			if (isBusyWhileSensing)
			{
				node->updateProjectedTimeOfTransmission()
			}
		}
	}
}