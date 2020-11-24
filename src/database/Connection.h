#ifndef TRACKER_CONNECTION_H
#define TRACKER_CONNECTION_H

#include "Statement.h"

namespace db {

class Connection {
public:
    explicit Connection(const std::string& filename);

    Statement prepare(const std::string& sql) const;

private:
    sqlite3* open(const std::string& filename) const;
    std::unique_ptr<sqlite3, decltype(&sqlite3_close)> connection;
};

}


#endif //TRACKER_CONNECTION_H
