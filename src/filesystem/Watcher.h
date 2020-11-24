#ifndef TRACKER_FILESYSTEM_WATCHER_H
#define TRACKER_FILESYSTEM_WATCHER_H

namespace fs {

class Listener;

class Watcher {
public:
    virtual ~Watcher() = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void addListener(Listener* listener) = 0;
    virtual void removeListener(Listener* listener) = 0;
    virtual const std::string& getPath() const = 0;
};

}


#endif //TRACKER_FILESYSTEM_WATCHER_H
