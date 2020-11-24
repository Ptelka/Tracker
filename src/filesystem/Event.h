#ifndef TRACKER_FILESYSTEM_EVENT_H
#define TRACKER_FILESYSTEM_EVENT_H

#include <string>

namespace fs {

struct Event {
    std::string filename;
    std::string path;
};

}

#endif //TRACKER_FILESYSTEM_EVENT_H
