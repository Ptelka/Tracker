#include "Inotify.h"
#include "logger/Logger.h"

#include <cstring>
#include <sys/inotify.h>
#include <zconf.h>

namespace fs {

Inotify::Inotify()
: descriptor(inotify_init()) {
    if (descriptor < 0) {
        THROW("Error while initialising inotify: " << std::strerror(errno));
    }
}

Inotify::~Inotify() {
    close(descriptor);
}

}