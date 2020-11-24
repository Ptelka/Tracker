#include "logger/Logger.h"
#include "database/Connection.h"

#include "filesystem/inotify/InotifyWatcher.h"

#include "worker/Worker.h"
#include "worker/MonitorFiles.h"
#include "worker/MonitorProcesses.h"

#include <rapidjson/document.h>
#include <csignal>
#include <iostream>
#include <list>

rapidjson::Document parse(int argc, char** argv);
void initLogs(const rapidjson::Document& document);
std::list<Worker> createWorkers(const rapidjson::Document& config, const db::Connection& connection);

bool working = true;

void handler(int) {
    working = false;
}

int main(int argc, char** argv) {
    signal(SIGQUIT, handler);
    signal(SIGINT, handler);
    signal(SIGTERM, handler);

    auto config = parse(argc, argv);

    initLogs(config);
    db::Connection connection(config["database"].GetString());
    auto workers = createWorkers(config, connection);

    std::cout << "Starting tracker" << std::endl;

    for (auto& worker: workers) {
        worker.start();
    }

    while (working) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "Stopping tracker" << std::endl;

    for (auto& worker: workers) {
        worker.stop();
    }

    return 0;
}

rapidjson::Document parse(int argc, char** argv) {
    if (argc < 2) {
        std::cout << "Usage: Tracker config_file" << std::endl;;
        exit(1);
    }

    std::ifstream stream(argv[1]);

    if (!stream) {
        std::cout << "Error while opening file " << argv[1] << std::endl;
    }

    auto config = std::string(
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>()
    );

    rapidjson::Document document;
    document.Parse(config.c_str());
    return document;
}

void initLogs(const rapidjson::Document& document) {
    auto file = document["log_file"].GetString();
    auto level = document["log_level"].GetInt();

    if (level < 0 || level > 2) {
        std::cout << "log_level allowed values are 0, 1, 2" << std::endl;
        exit(1);
    }

    log::Logger::getInstance().init(std::ofstream(file), static_cast<log::Level>(level));
}

std::list<Worker> createWorkers(const rapidjson::Document& config, const db::Connection& connection) {
    std::list<Worker> workers;

    for (const auto& entry: config["directories"].GetArray()) {
        auto path = entry["path"].GetString();
        auto tracking = entry["tracking"].GetArray();

        if (tracking.Size() > 2) {
            std::cout << "maximum allowed size of tracking array is 2" << std::endl;
            exit(1);
        }

        for (const auto& method: tracking) {
            auto name = std::string(method.GetString());
            if (name == "time") {
                workers.emplace_back(std::make_unique<MonitorProcesses>(connection, path));
            } else if (name == "files") {
                auto watcher = std::make_unique<fs::InotifyWatcher>(path);
                workers.emplace_back(std::make_unique<MonitorFiles>(connection, std::move(watcher)));
            }
        }
    }

    return workers;
}