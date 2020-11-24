#ifndef TRACKER_WORKER_WORKER_H
#define TRACKER_WORKER_WORKER_H

#include "Job.h"
#include <thread>

class Worker {
public:
    explicit Worker(std::unique_ptr<Job> job);

    void start();
    void stop();

private:
    std::unique_ptr<Job> job;
    std::thread thread;
};


#endif //TRACKER_WORKER_WORKER_H
