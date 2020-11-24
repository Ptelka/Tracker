#include "MonitorProcesses.h"

#include "filesystem/Event.h"
#include "process/Pid.h"
#include "logger/Logger.h"

#include <thread>
#include <filesystem>

using std::filesystem::path;
using std::filesystem::directory_entry;
using std::filesystem::perms;
using std::filesystem::recursive_directory_iterator;

namespace {

auto INSERT = "INSERT INTO EVENT (file, timestamp, event_type) VALUES (@file, @timestamp, @type);";

bool isExecutable(const path& path) {
    auto file = directory_entry(path);
    auto perms = file.status().permissions();

    return file.is_regular_file() &&
           ((perms & perms::group_exec) != perms::none ||
           (perms & perms::owner_exec) != perms::none ||
           (perms & perms::others_exec) != perms::none);
}

}

MonitorProcesses::MonitorProcesses(const db::Connection &connection, std::string path)
: running(true), statement(connection.prepare(INSERT)), path(std::move(path)) {
    LOG_INFO("Starting monitoring " << this->path << " for process activity");
    findExecutables();
}

void MonitorProcesses::findExecutables() {
    for (const auto& file: recursive_directory_iterator(path)) {
        if (isExecutable(file)) {
            LOG_DEBUG("Found new executable: " << file.path());
            executables.push_back({file.path(), false});
        }
    }
}

void MonitorProcesses::start() {
    while (isWorking()) {
        updateExecutablesState();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void MonitorProcesses::updateExecutablesState() {
    for (auto& executable: executables) {
        auto state = process::pid(executable.filename).has_value();

        if (state != executable.running) {
            if (state) {
                processHasBeenStarted(executable.filename);
            } else {
                processHasBeenStopped(executable.filename);
            }

            executable.running = state;
        }
    }
}

void MonitorProcesses::stop() {
    std::lock_guard<std::mutex> guard(mutex);
    running = false;
}

void MonitorProcesses::processHasBeenStarted(const std::string& filename) {
    LOG_INFO("Process has been started: " << filename);
    updateDatabase(filename, EventType::ProcessStarted);
}

void MonitorProcesses::processHasBeenStopped(const std::string& filename) {
    LOG_INFO("Process has been stopped: " << filename);
    updateDatabase(filename, EventType::ProcessStopped);
}

void MonitorProcesses::updateDatabase(const std::string &filename, EventType type) {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now).count();

    statement.bind("@file", filename)
             .bind("@timestamp", seconds)
             .bind("@type", static_cast<int64_t>(type))
             .execute();
}

bool MonitorProcesses::isWorking() {
    std::lock_guard<std::mutex> guard(mutex);
    return running;
}
