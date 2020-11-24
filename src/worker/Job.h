#ifndef TRACKER__WORKER_JOB_H
#define TRACKER__WORKER_JOB_H

class Job {
public:
    virtual ~Job() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
};

#endif //TRACKER__WORKER_JOB_H
