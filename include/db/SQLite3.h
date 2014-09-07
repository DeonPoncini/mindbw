#ifndef DB_SQLITE3_H
#define DB_SQLITE3_H

#include <sqlite3.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace db
{

enum class Operator
{
    LT,
    LTE,
    GT,
    GTE,
    EQ,
    NEQ,
};

std::string operator_to_string(Operator op);

std::string DBEsc(const std::string& s);
std::string DBPair(const std::string& key, const std::string& value);
std::string DBFuzzyPair(const std::string& key, const std::string& value);
std::string DBAnd(const std::vector<std::string>& list);
std::string DBOr(const std::vector<std::string>& list);
std::string DBNull();
std::string DBList(const std::vector<std::string>& list);
std::string DBKeyList(const std::vector<std::string>& list);
std::string DBUnique(const std::string& s);
std::string DBTrue();
std::string DBFalse();
std::string DBAll();

class SQLite3
{
public:
    typedef std::unordered_map<std::string, std::string> DataMap;
    typedef std::function<void(DataMap)> DataMapFn;

    explicit SQLite3(const char* name);
    SQLite3(const SQLite3&) = delete;
    SQLite3(SQLite3&&) = delete;
    SQLite3& operator=(const SQLite3&) = delete;
    SQLite3& operator=(SQLite3&&) = delete;
    ~SQLite3();

    void select(const std::string& select, const std::string& from,
            const std::string& where, DataMapFn f);
    std::string insert(const std::string& insert, const std::string& values);
    void update(const std::string& update, const std::string& set,
            const std::string& where);
    void del(const std::string& del, const std::string& where);

    void exec(const std::string& statement, DataMapFn f);

private:
    sqlite3* mDB;
};
}

#endif
