#include "Statement.h"
#include "logger/Logger.h"

namespace db {

Statement::Statement(sqlite3_stmt *stmt)
: statement(stmt, sqlite3_finalize) {}

void Statement::execute() {
    if (sqlite3_step(statement.get()) != SQLITE_DONE) {
        THROW("Error while executing statement");
    }
    if (sqlite3_reset(statement.get()) != SQLITE_OK) {
        THROW("Error while resetting statement");
    }
}

Statement &Statement::bind(const std::string& name, const std::string &value) {
    if (sqlite3_bind_text(statement.get(), position(name), value.c_str(), value.size(), nullptr) != SQLITE_OK) {
        THROW("Error while binding value. Name: " << name << " Value: " << value);
    }
    return *this;
}

Statement &Statement::bind(const std::string &name, int64_t value) {
    if (sqlite3_bind_int64(statement.get(), position(name), value) != SQLITE_OK) {
        THROW("Error while binding value. Name: " << name << " Value: " << value);
    }
    return *this;
}

int Statement::position(const std::string &name) const {
    auto pos = sqlite3_bind_parameter_index(statement.get(), name.c_str());
    if (pos == 0) {
        THROW("Could not find parameter in statement. Name: " << name);
    }
    return pos;
}

}
