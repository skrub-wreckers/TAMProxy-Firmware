#ifndef UTIL_H
#define UTIL_H

#include <cstdint>
#include <cstddef>
 
namespace tamproxy {

// Adapted from rosettacode.org/wiki/Averages/Simple_moving_average#C.2B.2B
class MovingAverage {
public:
	MovingAverage(uint8_t maxSize);
	virtual ~MovingAverage();
	void add(uint16_t val);
	uint16_t avg() const;
 
private:
	uint8_t size;
	uint16_t * window; // Holds the values to calculate the average of
	uint16_t * head; // Points at the oldest element we've stored.
	uint16_t * tail; // Points at the newest element we've stored.
	uint32_t total; // Cache the total so we don't sum everything each time.
 
	void inc(uint16_t * & p);
	ptrdiff_t windowSize() const;
};

}
#endif