#ifndef TRACKER_FILESYSTEM_INOTIFY_DIRECTORYWATCHER_H
#define TRACKER_FILESYSTEM_INOTIFY_DIRECTORYWATCHER_H

#include <string>

namespace fs {

struct DirectoryWatcher {
    DirectoryWatcher(DirectoryWatcher&& other) noexcept;
    DirectoryWatcher(int descriptor, std::string path, int mask);

    ~DirectoryWatcher();

    int id;
    int descriptor;
    std::string path;
};

}

#endif //TRACKER_FILESYSTEM_INOTIFY_DIRECTORYWATCHER_H
