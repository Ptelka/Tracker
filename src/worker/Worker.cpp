#include "Worker.h"

#include "logger/Logger.h"

Worker::Worker(std::unique_ptr<Job> job)
: job(std::move(job)) { }

void Worker::start() {
    thread = std::thread([&](){
        try {
            job->start();
        } catch (const std::exception& exception) {
            LOG_INFO("Job executed with error: " << exception.what());
        } catch (...) {
            LOG_INFO("Job executed with unknown error");
        }
    });
}

void Worker::stop() {
    job->stop();
    thread.join();
}
