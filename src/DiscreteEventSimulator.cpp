#include "DiscreteEventSimulator.hpp"

/**
 * Returns true if the specified time is between 0 seconds and the current simulation duration.
 */
template <class TTime, class TState>
bool DiscreteEventSimulator<TTime, TState>::isWithinSimulationDuration(Seconds time)
{
	bool result = (time >= 0) && (time <= simulationDuration);
	return result;
}

/**
 * Adds an event to be processed at a particular time with respect to other events,
 * _only_ if it is within the simulation's duration; this ensures that no events
 * occurring _outside_ of the simulation's duration will contribute to the simulation's
 * measured performance statistics.
 *
 * Returns true if the specified event's time is within the simulation time;
 * if this is the case, this class takes _sole responsibility_ over the destruction of
 * the pointer passed in.
 *
 * If false is returned, it is up to the event's creator to delete it, whereby this
 * class _does not_ take responsibility for the destruction of the pointer passed in.
 *
 * See isWithinSimulationDuration().
 */
template <class TTime, class TState>
bool DiscreteEventSimulator<TTime, TState>::addEvent(DiscreteEvent<TTime, TState> *event)
{
	if (isWithinSimulationDuration(event->time))
	{
		eventQueue.push(event);
		return true;
	}
	
	return false;
}

/**
 * Processes all of the events that were registered for this simulation;
 * some events may register further events to be processed in the future.
 */
template <class TTime, class TState>
void DiscreteEventSimulator<TTime, TState>::runSimulation()
{
	while (!eventQueue.empty())
	{
		DiscreteEvent<TTime, TState> *event = eventQueue.top();
		eventQueue.pop();
		event->process(this);
		delete event;
	}
}

/**
 * Removes and deletes all registered events without processing them.
 */
template <class TTime, class TState>
void DiscreteEventSimulator<TTime, TState>::flush()
{
	while (!eventQueue.empty())
	{
		DiscreteEvent<TTime, TState> *event = eventQueue.top();
		eventQueue.pop();
		delete event;
	}
}