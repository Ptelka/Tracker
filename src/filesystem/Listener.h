#ifndef TRACKER_FILESYSTEM_LISTENER_H
#define TRACKER_FILESYSTEM_LISTENER_H

namespace fs {

struct Event;

class Listener {
public:
    virtual ~Listener() = default;
    virtual void onMoveFrom(const Event&) {};
    virtual void onMoveTo(const Event&) {};
    virtual void onWrite(const Event&) {};
    virtual void onDelete(const Event&) {};
    virtual void onCreate(const Event&) {};
    virtual void onAccess(const Event&) {};
    virtual void onAttributeChange(const Event&) {};
};

}

#endif //TRACKER_FILESYSTEM_LISTENER_H
