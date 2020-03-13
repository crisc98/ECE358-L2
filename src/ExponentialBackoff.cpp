#include "ExponentialBackoff.hpp"

/**
 * Generates a random bit-delay based on the current number of collisions
 * and then increments the collision counter.
 *
 * Returns -1 if the maximum number of collisions (10) has been exceeded.
 */
Bits ExponentialBackoff::getNextWaitTime()
{
	++collisions;
	if (collisions > 10)
	{
		return -1;
	}

	std::uniform_int_distribution<> getRandomNumber(0, power - 1);

	power *= 2;

	int randomNumber = getRandomNumber(gen);
	Bits waitTime = randomNumber * baseWaitTime;

	return waitTime;
}

/**
 * Returns true if there has at least been one collision.
 */
bool ExponentialBackoff::isBackedOff()
{
	bool isBackedOff = collisions > 0;
	return isBackedOff;
}

/**
 * Resets the collision counter and power.
 */
void ExponentialBackoff::reset()
{
	collisions = 0;
	power = 2;
}