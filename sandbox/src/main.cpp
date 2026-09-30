#include <pe/core/log.hpp>
#include <pe/core/time.hpp>

#include <SDL3/SDL.h>

int main(int /*argc*/, char** /*argv*/) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        pe::core::logError("SDL_Init failed: {}", SDL_GetError());
        return 1;
    }

    int major = SDL_GetVersion();
    pe::core::logInfo("PixelEngine sandbox starting. SDL version {}", major);

    pe::core::Clock clock;
    for (int i = 0; i < 3; ++i) {
        SDL_Delay(100);
        pe::core::logInfo("frame {} took {:.4f}s", i, clock.tick());
    }

    SDL_Quit();
    return 0;
}
