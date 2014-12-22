#ifndef MINDBW_SQLITE3_H
#define MINDBW_SQLITE3_H

#include <sqlite3.h>

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace mindbw
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

std::string Compare(const std::string& key, const std::string& value,
        Operator op);
std::string Equal(const std::string& key, const std::string& value);
std::string Like(const std::string& key, const std::string& value);
std::string Null();
std::string True();
std::string False();
std::string All();
std::string ValueList(const std::vector<std::string>& list);
std::string KeyList(const std::vector<std::string>& list);
std::string Unique(const std::string& s);
std::string And(const std::vector<std::string>& list);
std::string Or(const std::vector<std::string>& list);

typedef std::unordered_map<std::string, std::string> DataMap;
typedef std::function<void(DataMap)> DataMapFn;

class SQLite3
{
public:
    explicit SQLite3(const std::string& name);
    explicit SQLite3(const char* name);
    SQLite3(const SQLite3&) = delete;
    SQLite3(SQLite3&&) = delete;
    SQLite3& operator=(const SQLite3&) = delete;
    SQLite3& operator=(SQLite3&&) = delete;
    ~SQLite3();

    void select(const std::string& select, const std::string& from,
            const std::string& where, const DataMapFn& f);
    std::string insert(const std::string& insert, const std::string& values);
    void update(const std::string& update, const std::string& set,
            const std::string& where);
    void del(const std::string& del, const std::string& where);

    void exec(const std::string& statement,
            const DataMapFn& f = [](DataMap){});

private:
    sqlite3* mDB;
};
}

#endif
