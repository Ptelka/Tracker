#include "Connection.h"
#include "logger/Logger.h"

namespace db {

Connection::Connection(const std::string &filename)
: connection(open(filename), sqlite3_close) {}

sqlite3 *Connection::open(const std::string &filename) const {
    sqlite3* ptr;

    if (sqlite3_open(filename.c_str(), &ptr) != SQLITE_OK) {
        THROW("Could not connect to database: " << filename);
    }

    LOG_DEBUG("Connected to database: " << filename);

    return ptr;
}

Statement Connection::prepare(const std::string &sql) const {
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(connection.get(), sql.c_str(), sql.size(), &stmt, nullptr) != SQLITE_OK) {
        THROW("Could not create statement: " << sql);
    }

    return Statement(stmt);
}

}
