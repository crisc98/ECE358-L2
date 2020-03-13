#include "DiscreteEvent.hpp"

/**
 * Processes the discrete event, allowing it to act upon the specific state.
 * If cancelled = true, then this method will do nothing.
 * 
 * This is an implementation of the Template Method Pattern; see
 * https://en.wikipedia.org/wiki/Template_method_pattern.
 */
template <class TTime, class TState>
void DiscreteEvent<TTime, TState>::process(TState *state)
{
	if (!cancelled) processImplementation(state);
}