#ifndef TRACKER_WORKER_EVENTTYPE_H
#define TRACKER_WORKER_EVENTTYPE_H

enum class EventType {
    FileCreated,
    FileMovedFrom,
    FileMovedTo,
    FileDeleted,
    FileModified,
    ProcessStarted,
    ProcessStopped
};

#endif //TRACKER_WORKER_EVENTTYPE_H
