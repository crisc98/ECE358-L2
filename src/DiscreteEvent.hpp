#pragma once

/**
 * Represents a discrete event occuring at a particular point in time that will
 * cause a state change on a particular state at the time it gets processed.
 */
template <class TTime, class TState>
class DiscreteEvent
{
protected:

	/**
	 * The implementation of the process() method that will be skipped if cancelled = true.
	 */
	virtual void processImplementation(TState *state) = 0;

public:

	/**
	 * The time at which this discrete event should be processed.
	 */
	TTime time;

	/**
	 * True if this event has been cancelled, meaning that this event once it is
	 * about to be processed should not execute the contents of its contents.
	 * This allows one to save on the performance overhead of completely removing
	 * the event from the priority queue (provided that is even possible before that
	 * element has reached the top).
	 */
	bool cancelled;

	/**
	 * Creates a discrete event to be processed at the specified time.
	 */
	DiscreteEvent(TTime time) : time(time), cancelled(false)
	{
	}

	/**
	 * Processes the discrete event, allowing it to act upon the specific state.
	 * If cancelled = true, then this method will do nothing.
	 */
	void process(TState *state);
	
	/**
	 * Compares pointers to discrete events based on their time of occurrence.
	 * Namely, in a priority_queue, this comparator will always place the
	 * _earliest_ event at the top of the heap.
	 */
	class Comparator
	{
	public:

		bool operator()(DiscreteEvent *lhs, DiscreteEvent *rhs)
		{
			bool result = lhs->time > rhs->time;
			return result;
		}
	};
};

/**
 * The following is to fix a link error.
 * See https://www.codeproject.com/Articles/48575/How-to-Define-a-Template-Class-in-a-h-File-and-Imp.
 */
#include "DiscreteEvent.cpp"