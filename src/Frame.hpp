#pragma once

typedef double Bits;
typedef double Seconds;

/**
 * Represents a single frame to be transmitted from a node at a particular time.
 */
class Frame
{
public:

	/**
	 * The time at which the frame has finished being received by its respective node
	 * and stored into its frame queue.
	 */
	Seconds arrivalTime;
	
	/**
	 * The frame's length in bits.
	 */
	Bits length;

	Frame(
		Seconds timeCreated,
		Bits length
	) :
		arrivalTime(timeCreated),
		length(length)
	{
	}
};