#include "pe/core/time.hpp"
#include <SDL3/SDL_timer.h>

namespace pe::core {

Clock::Clock()
    : frequency_(SDL_GetPerformanceFrequency())
    , start_(SDL_GetPerformanceCounter())
    , last_(start_) {}

float Clock::tick() {
    const uint64_t now   = SDL_GetPerformanceCounter();
    const uint64_t delta = now - last_;
    last_ = now;
    return float(double(delta) / double(frequency_));
}

double Clock::totalSeconds() const {
    return double(last_ - start_) / double(frequency_);
}

} // namespace pe::core