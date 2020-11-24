#include "DirectoryWatcher.h"

#include "logger/Logger.h"

#include <cstring>
#include <sys/inotify.h>

namespace fs {

DirectoryWatcher::DirectoryWatcher(int descriptor, std::string path, int mask)
: id(inotify_add_watch(descriptor, path.c_str(), mask)), descriptor(descriptor), path(std::move(path)) {
    if (id < 0) {
        THROW("Error while creating watcher '" << this->path << "': " << strerror(errno));
    }

    LOG_DEBUG("Adding watcher: " << this->path);
}

DirectoryWatcher::DirectoryWatcher(DirectoryWatcher&& other) noexcept
: id(other.id), descriptor(other.descriptor), path(std::move(other.path)) {
    other.id = -1;
    other.descriptor = -1;
}

DirectoryWatcher::~DirectoryWatcher() {
    if (id >= 0 && descriptor >= 0) {
        LOG_DEBUG("Removing watcher: " << path);
        inotify_rm_watch(descriptor, id);
        id = -1;
        descriptor = -1;
    }
}

}
