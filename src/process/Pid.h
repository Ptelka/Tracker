#ifndef TRACKER_PROCESS_PID_H
#define TRACKER_PROCESS_PID_H

#include <string>
#include <optional>

namespace process {

std::optional<int> pid(const std::string& filename);

}

#endif //TRACKER_PROCESS_PID_H
