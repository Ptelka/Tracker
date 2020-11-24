#include "MonitorFiles.h"

#include "logger/Logger.h"
#include "filesystem/Event.h"

namespace {

const auto INSERT = "INSERT INTO EVENT (file, timestamp, event_type) VALUES (@file, @timestamp, @type);";

}

MonitorFiles::MonitorFiles(const db::Connection &connection, std::unique_ptr<fs::Watcher> watcher)
: statement(connection.prepare(INSERT)), watcher(std::move(watcher)) {
    LOG_INFO("Starting monitoring path '" << this->watcher->getPath() << "' for file changes");
    this->watcher->addListener(this);
}

void MonitorFiles::start() {
    watcher->start();
}

void MonitorFiles::stop() {
    watcher->stop();
}

void MonitorFiles::onMoveFrom(const fs::Event &event) {
    LOG_INFO("File moved from " << event.path << "/" << event.filename);
    process(event, EventType::FileMovedFrom);
}

void MonitorFiles::onMoveTo(const fs::Event &event) {
    LOG_INFO("File moved to " << event.path << "/" << event.filename);
    process(event, EventType::FileMovedTo);}

void MonitorFiles::onWrite(const fs::Event &event) {
    LOG_INFO("File modified " << event.path << "/" << event.filename);
    process(event, EventType::FileModified);
}

void MonitorFiles::onDelete(const fs::Event &event) {
    LOG_INFO("File deleted " << event.path << "/" << event.filename);
    process(event, EventType::FileDeleted);
}

void MonitorFiles::onCreate(const fs::Event &event) {
    LOG_INFO("File created " << event.path << "/" << event.filename);
    process(event, EventType::FileCreated);
}

void MonitorFiles::process(const fs::Event &event, EventType type) {
    auto now = std::chrono::system_clock::now().time_since_epoch();
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now).count();

    statement.bind("@file", path(event))
             .bind("@timestamp", seconds)
             .bind("@type", static_cast<int64_t>(type))
             .execute();
}

std::string MonitorFiles::path(const fs::Event &event) const {
    return event.path + "/" + event.filename;
}