#include "pe/core/log.hpp"
#include <SDL3/SDL_log.h>

namespace pe::core {

void logMessage(LogLevel level, std::string_view message) {
    SDL_LogPriority priority = SDL_LOG_PRIORITY_INFO;
    switch (level) {
        case LogLevel::Trace: priority = SDL_LOG_PRIORITY_VERBOSE; break;
        case LogLevel::Info:  priority = SDL_LOG_PRIORITY_INFO;    break;
        case LogLevel::Warn:  priority = SDL_LOG_PRIORITY_WARN;    break;
        case LogLevel::Error: priority = SDL_LOG_PRIORITY_ERROR;   break;
    }
    // %.*s prints a length-delimited string — string_view is not null-terminated.
    SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, priority,
                   "%.*s", int(message.size()), message.data());
}

} // namespace pe::core