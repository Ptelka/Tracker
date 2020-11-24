#ifndef TRACKER_WORKER_MONITORPROCESSES_H
#define TRACKER_WORKER_MONITORPROCESSES_H

#include "Job.h"
#include "EventType.h"

#include "database/Connection.h"
#include "database/Statement.h"

#include <mutex>
#include <list>

class MonitorProcesses: public Job {
public:
    MonitorProcesses(const db::Connection& connection, std::string path);

    void start() override;
    void stop() override;

private:
    struct Executable {
        std::string filename;
        bool running;
    };

    void updateExecutablesState();
    void processHasBeenStarted(const std::string& filename);
    void processHasBeenStopped(const std::string& filename);
    void updateDatabase(const std::string &filename, EventType type);
    void findExecutables();

    bool isWorking();

    bool running;

    std::list<Executable> executables;
    db::Statement statement;
    std::string path;
    std::mutex mutex;
};

#endif //TRACKER_WORKER_MONITORPROCESSES_H
