#include "InotifyWatcher.h"
#include "filesystem/Listener.h"

#include "logger/Logger.h"

#include <cstring>
#include <utility>
#include <zconf.h>

#include <iostream>

namespace {

const auto MASK = IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM | IN_DELETE_SELF | IN_MOVE_SELF | IN_ACCESS;

}

namespace fs {

InotifyWatcher::InotifyWatcher(std::string path)
: working(true), watchers(path, inotify, MASK), path(std::move(path)) {

}

void InotifyWatcher::addListener(Listener *listener) {
    listeners.push_back(listener);
}

void InotifyWatcher::removeListener(Listener *listener) {
    listeners.remove(listener);
}

const std::string &InotifyWatcher::getPath() const {
    return path;
}

void InotifyWatcher::start() {
    while (isWorking()) {
        if (select()) {
            read();
        }
    }
}

bool InotifyWatcher::select() {
    fd_set descriptors;

    FD_ZERO(&descriptors);
    FD_SET(inotify.descriptor, &descriptors);

    timeval timeout {1, 0};

    return ::select(inotify.descriptor + 1, &descriptors, nullptr, nullptr, &timeout);
}

void InotifyWatcher::read() {
    auto bytes = read(buffer.data(), sizeof(buffer));

    auto i = 0l;
    while (i < bytes) {
        auto event = reinterpret_cast<inotify_event*>(buffer.data() + i);

        auto id = watchers.find(event->wd);
        if (id) {
            process({event->len ? event->name : "", *id}, event->mask);
        }

        i += sizeof(inotify_event) + event->len;
    }
}

ssize_t InotifyWatcher::read(void *buff, std::size_t bytes) const {
    auto len = ::read(inotify.descriptor, buff, bytes);

    if (len >= 0) {
        return len;
    }

    if (errno == EAGAIN || errno == EINTR) {
        return 0;
    }

    THROW("Read error: " << std::strerror(errno));
}

void InotifyWatcher::process(const Event& event, int type) {
    if (type & IN_CREATE) {
        notify(event, &Listener::onCreate);
    }
    if (type & IN_DELETE) {
        notify(event, &Listener::onDelete);
    }
    if (type & IN_MOVED_TO) {
        notify(event, &Listener::onMoveTo);
    }
    if (type & IN_MOVE_SELF) {
        notify(event, &Listener::onMoveFrom);
    }
    if (type & IN_DELETE_SELF) {
        notify(event, &Listener::onDelete);
    }
    if (type & IN_MOVED_FROM) {
        notify(event, &Listener::onMoveFrom);
    }
    if (type & IN_MODIFY) {
        notify(event, &Listener::onWrite);
    }
    if (type & IN_ACCESS) {
        notify(event, &Listener::onAccess);
    }
    if (type & IN_ATTRIB) {
        notify(event, &Listener::onAttributeChange);
    }

    if (type & IN_ISDIR) {
        if (type & IN_CREATE || type & IN_MOVED_TO) {
            watchers.add(event.path + "/" + event.filename);
        }
        if (type & IN_DELETE || type & IN_MOVED_FROM || type & IN_DELETE_SELF || type & IN_MOVE_SELF) {
            watchers.remove(event.path + "/" + event.filename);
        }
    }
}

void InotifyWatcher::notify(const Event& event, ListenerMethod method) const {
    for (auto listener: listeners) {
        (listener->*method)(event);
    }
}

void InotifyWatcher::stop() {
    std::lock_guard<std::mutex> guard(mutex);
    working = false;
}

bool InotifyWatcher::isWorking() {
    std::lock_guard<std::mutex> guard(mutex);
    return working;
}



}