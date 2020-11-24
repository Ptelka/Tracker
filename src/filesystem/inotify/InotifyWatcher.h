#ifndef TRACKER_FILESYSTEM_INOTIFY_INOTIFYWATCHER_H
#define TRACKER_FILESYSTEM_INOTIFY_INOTIFYWATCHER_H

#include "DirectoryWatchers.h"
#include "Inotify.h"

#include "filesystem/Event.h"
#include "filesystem/Watcher.h"

#include <string>
#include <list>
#include <mutex>
#include <sys/inotify.h>

#define ALIGN(name) __attribute__ ((aligned(__alignof__(name))))

namespace fs {

class Listener;

class InotifyWatcher: public Watcher {
public:
    explicit InotifyWatcher(std::string path);

    void start() override;
    void stop() override;

    void addListener(Listener* listener) override;
    void removeListener(Listener* listener) override;
    const std::string& getPath() const override;

private:
    using ListenerMethod = void(Listener::*)(const Event&);

    bool select();
    void read();
    ssize_t read(void* buff, std::size_t bytes) const;
    void process(const Event& event, int type);
    void notify(const Event& event, ListenerMethod method) const;
    bool isWorking();

    bool working;

    std::list<Listener*> listeners;
    Inotify inotify;
    DirectoryWatchers watchers;
    std::string path;
    std::mutex mutex;
    std::array<char, 4096> ALIGN(inotify_event) buffer {};
};

}


#endif //TRACKER_FILESYSTEM_INOTIFY_INOTIFYWATCHER_H
