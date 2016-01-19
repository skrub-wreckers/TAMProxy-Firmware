#include "Util.h"
#include <cstdint>
#include <cstddef>

namespace tamproxy {
    MovingAverage::MovingAverage(uint8_t maxSize) {
        size = maxSize;
        window = new uint16_t[size];
        head = NULL;
        tail = NULL;
        total = 0;
    }

    // Adds a value to the average, pushing one out if nescessary
    void MovingAverage::add(uint16_t val) {
        // Special case: Initialization
        if (head == NULL) {
            head = window;
            *head = val;
            tail = head;
            inc(tail);
            total = val;
            return;
        }
 
        // Were we already full?
        if (head == tail) {
            // Fix total-cache
            total -= *head;
            // Make room
            inc(head);
        }
 
        // Write the value in the next spot.
        *tail = val;
        inc(tail);
 
        // Update our total-cache
        total += val;
    }

    // Returns the average of the last P elements added to this SMA.
    // If no elements have been added yet, returns 0.0
    uint16_t MovingAverage::avg() const {
        ptrdiff_t size = this->size();
        if (size == 0) {
            return 0; // No entries => 0 average
        }
        return total / (double) size; // Cast to double for floating point arithmetic
    }

    // Bumps the given pointer up by one.
    // Wraps to the start of the array if needed.
    void inc(uint16_t * & p) {
        if (++p >= window + size) {
            p = window;
        }
    }

    // Returns how many numbers we have stored.
    ptrdiff_t windowSize() const {
        if (head == NULL)
            return 0;
        if (head == tail)
            return size;
        return (size + tail - head) % size;
    }
}
