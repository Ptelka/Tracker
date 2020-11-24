#ifndef TRACKER_STATEMENT_H
#define TRACKER_STATEMENT_H

#include <string>
#include <sqlite3.h>
#include <memory>

namespace db {

class Statement {
public:
    explicit Statement(sqlite3_stmt* stmt);

    Statement& bind(const std::string& name, const std::string& value);
    Statement& bind(const std::string& name, int64_t value);

    void execute();

private:
    int position(const std::string& name) const;
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> statement;
};

}


#endif //TRACKER_STATEMENT_H
