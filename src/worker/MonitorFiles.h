#ifndef TRACKER_WORKER_MONITORFILES_H
#define TRACKER_WORKER_MONITORFILES_H

#include "Job.h"
#include "EventType.h"

#include "database/Connection.h"
#include "database/Statement.h"

#include "filesystem/Listener.h"
#include "filesystem/Watcher.h"

class MonitorFiles: public Job, public fs::Listener {
public:
    MonitorFiles(const db::Connection& connection, std::unique_ptr<fs::Watcher> watcher);

    void start() override;
    void stop() override;

private:
    void onMoveFrom(const fs::Event& event) override;
    void onMoveTo(const fs::Event& event) override;
    void onWrite(const fs::Event& event) override;
    void onDelete(const fs::Event& event) override;
    void onCreate(const fs::Event& event) override;

    void process(const fs::Event& event, EventType type);
    std::string path(const fs::Event& event) const;

    db::Statement statement;
    std::unique_ptr<fs::Watcher> watcher;
};

#endif //TRACKER_WORKER_MONITORFILES_H
