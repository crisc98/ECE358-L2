#pragma once

#include <random>

typedef long Bits;
typedef long Collisions;

/**
 * Stores the state for performing an exponential backoff.
 */
class ExponentialBackoff
{
private:

	Collisions maxCollisions;
	Collisions collisions;
	Bits baseWaitTime;
	int power;

	// initialize random number generation; see https://en.cppreference.com/w/cpp/numeric/random/exponential_distribution
	std::random_device rd;
	std::mt19937_64 gen;

public:

	ExponentialBackoff() :
		maxCollisions(10),
		collisions(0),
		baseWaitTime(512),
		power(2)
		gen(rd())
	{
	}

	/**
	 * Generates a random bit-delay based on the current number of collisions
	 * and then increments the collision counter.
	 *
	 * Returns -1 if the maximum number of collisions (10) has been exceeded.
	 */
	Bits getNextWaitTime();

	/**
	 * Resets the collision counter and power.
	 */
	void reset();
};