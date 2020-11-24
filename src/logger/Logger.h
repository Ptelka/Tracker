#ifndef TRACKER_LOGGER_LOGGER_H
#define TRACKER_LOGGER_LOGGER_H

#include <fstream>
#include <sstream>
#include <mutex>

namespace log {

enum class Level {
    DISABLED,
    INFO,
    DEBUG
};

class Logger {
public:
    void init(std::ofstream stream, Level level);
    void log(const std::string& message);

    Level getLogLevel();

    static Logger& getInstance();

private:
    Logger() = default;

    std::mutex mutex;
    std::ofstream stream;
    Level level = Level::DISABLED;
};

}

#define LOG(level, message)                                                  \
if (level <= log::Logger::getInstance().getLogLevel()) {                     \
    std::stringstream  __stream;                                             \
    __stream << __FILE__ << ":" << __LINE__ << " " << message << std::endl;  \
    log::Logger::getInstance().log(__stream.str());                          \
} static_assert(true, "enforce semicolon")

#define LOG_INFO(message) LOG(log::Level::INFO, message)
#define LOG_DEBUG(message) LOG(log::Level::DEBUG, message)

#define THROW(message) {                                                               \
    LOG_INFO(message);                                                                 \
    std::stringstream  __exception_stream;                                             \
    __exception_stream << __FILE__ << ":" << __LINE__ << " " << message << std::endl;  \
    throw std::runtime_error(__exception_stream.str());                                \
} static_assert(true, "enforce semicolon")

#endif //TRACKER_LOGGER_LOGGER_H
