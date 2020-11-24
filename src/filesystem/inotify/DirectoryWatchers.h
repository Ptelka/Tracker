#ifndef TRACKER_FILESYSTEM_INOTIFY_DIRECTORYWATCHERS_H
#define TRACKER_FILESYSTEM_INOTIFY_DIRECTORYWATCHERS_H

#include "Inotify.h"
#include "DirectoryWatcher.h"

#include <unordered_map>

namespace fs {

class DirectoryWatchers {
public:
    DirectoryWatchers(const std::string& path, const Inotify& inotify, int mask);

    void add(const std::string& path);
    void remove(const std::string& path);

    std::optional<std::string> find(int id) const;

private:
    const int mask;
    const Inotify& inotify;

    std::unordered_map<std::string, DirectoryWatcher> watchers;
    std::unordered_map<int, std::string> paths;

    void watch(const std::string& path);
};

}

#endif //TRACKER_FILESYSTEM_INOTIFY_DIRECTORYWATCHERS_H
