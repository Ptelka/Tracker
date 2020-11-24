#include "Logger.h"

namespace log {

void Logger::init(std::ofstream stream, Level level) {
    std::lock_guard<std::mutex> guard(mutex);

    if (!stream) {
        throw std::runtime_error("Initialising logger with invalid ofstream");
    }

    this->stream = std::move(stream);
    this->level = level;
}

void Logger::log(const std::string &message) {
    std::lock_guard<std::mutex> guard(mutex);

    if (stream) {
        stream << message;
        stream.flush();
    }
}

Level Logger::getLogLevel() {
    std::lock_guard<std::mutex> guard(mutex);
    return level;
}

Logger &Logger::getInstance() {
    static Logger instance;
    return instance;
}

}