#ifndef TRACKER_FILESYSTEM_INOTIFY_INOTIFY_H
#define TRACKER_FILESYSTEM_INOTIFY_INOTIFY_H

#include <string>

namespace fs {

struct Inotify {
    explicit Inotify();
    ~Inotify();

    const int descriptor;
};

}

#endif //TRACKER_FILESYSTEM_INOTIFY_INOTIFY_H
