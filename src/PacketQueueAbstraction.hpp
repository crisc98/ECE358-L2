#pragma once

typedef double Seconds;
typedef double CumulativePackets;
typedef long Bits;
typedef long BitsPerSecond;
typedef long Observations;
typedef long Packets;

/**
 * An abstraction of a packet queue's state and statistics regarding its performance.
 */
class PacketQueueAbstraction
{
public:

	/**
	 * The packet queue's size, K, as measured in packets.
	 */
	Packets maxBufferSize = 0;

	/**
	 * The current number of packets stored in the queue.
	 */
	Packets currentBufferSize = 0;

	/**
	 * The cumulative number of packets in the queue across all observations.
	 */
	CumulativePackets cumulativeBufferSize = 0;

	/**
	 * The rate in bits per second at which a packet can be serviced and transmitted.
	 * Used to determine the delay between a packets arrival, the previous packet's
	 * departure, and when the current packet will have finished being serviced.
	 */
	BitsPerSecond transmissionRate = 0;

	Packets numArrivals = 0;
	Packets numDepartures = 0;
	Packets numDropped = 0;
	Observations numIdles = 0;
	Observations numObservations = 0;

	/**
	 * True if the packet queue is currently empty and is not
	 * imminently serving a packet.
	 */
	bool isIdle = true;

	/**
	 * True if the packet queue is full and can accept no more packets.
	 */
	bool isFull = false;

	/**
	 * The time at which the last packet completed being transmitted.
	 */
	Seconds lastDeparture = 0;

	/**
	 * Calculate E[N], the average number of packets in the queue across all observations.
	 */
	double getAverageBufferSize();

	/**
	 * Calculate P_idle, the fraction of all observations over which the queue
	 * was observed to be empty.
	 */
	double getQueueIdleRatio();

	/**
	 * Calculates P_loss, the ratio of the number of dropped packets to the total
	 * number of packets generated.
	 */
	double getPacketLossRatio();

	/**
	 * Resets all of the measured statistics so that a new set of measurements may
	 * be taken for a new set of packet queue events, leaving maxBufferSize and
	 * transmissionRate unchanged; those may be edited manually.
	 *
	 * The maxBufferSize and transmissionRate MUST be set and this method called
	 * before running a new simulation of the packet queue.
	 */
	void reset();
};