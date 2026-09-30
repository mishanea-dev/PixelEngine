#pragma once
#include <cstdint>

namespace pe::core {

// Monotonic wall-clock timer. Never use system time for frame timing —
// it can jump backwards when the OS syncs the clock.
class Clock {
public:
    Clock();
    // Call once per frame. Returns seconds since the previous call.
    float tick();
    double totalSeconds() const;

private:
    uint64_t frequency_ = 0;
    uint64_t start_     = 0;
    uint64_t last_      = 0;
};

} // namespace pe::core