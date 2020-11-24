#include "Pid.h"

#include <filesystem>

using std::filesystem::directory_iterator;
using std::filesystem::exists;
using std::filesystem::read_symlink;

namespace process {

std::optional<int> pid(const std::string& filename) {
    for (const auto& pid: directory_iterator("/proc")) {
        try {
            auto symlink = pid.path().string() + "/exe";

            if (exists(symlink) && filename == read_symlink(symlink)) {
                return std::stoi(pid.path().filename());
            }
        } catch (const std::exception& exception) {
            // no permission to read symlink
        }
    }

    return {};
}

}