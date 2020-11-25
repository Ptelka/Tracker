#include "DirectoryWatchers.h"

#include "logger/Logger.h"

#include <filesystem>

using std::filesystem::is_directory;
using std::filesystem::recursive_directory_iterator;

namespace fs {

DirectoryWatchers::DirectoryWatchers(const std::string& path, const Inotify &inotify, int mask)
: mask(mask), inotify(inotify) {
    add(path);
}

void DirectoryWatchers::add(const std::string &path) {
    if (!is_directory(path)) {
        THROW("File '" << path << "' is not a directory");
    }

    watch(path);

    for (const auto& file: recursive_directory_iterator(path)) {
        if (file.is_directory()) {
            watch(file.path());
        }
    }
}

void DirectoryWatchers::watch(const std::string &path) {
    DirectoryWatcher watch(inotify.descriptor, path, mask);
    paths.emplace(watch.id, watch.path);
    watchers.emplace(watch.path, std::move(watch));
}

void DirectoryWatchers::remove(const std::string& path) {
    auto watch = watchers.find(path);
    paths.erase(watch->second.id);
    watchers.erase(watch);
}

std::optional<std::string> DirectoryWatchers::find(int id) const {
    auto path = paths.find(id);

    if (path == paths.end()) {
        return {};
    }

    return {path->second};
}

}