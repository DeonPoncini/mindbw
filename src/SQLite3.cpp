#include "SQLite3.h"

#include <stdexcept>

namespace db
{

std::string operator_to_string(Operator op)
{
    switch (op) {
        case Operator::LT: return "<";
        case Operator::LTE: return "<=";
        case Operator::GT: return ">";
        case Operator::GTE: return ">=";
        case Operator::EQ: return "=";
        case Operator::NEQ: return "!=";
    }
}

std::string Esc(const std::string& s)
{
    auto query = sqlite3_mprintf("'%q'",s.c_str());
    auto ret = std::string(query);
    sqlite3_free(query);
    return ret;
}

std::string Compare(const std::string& key, const std::string& value,
        Operator op)
{
    return key + operator_to_string(op) + Esc(value);
}

std::string Equal(const std::string& key, const std::string& value)
{
    return Compare(key, value, Operator::EQ);
}

std::string Like(const std::string& key, const std::string& value)
{
    return key + " LIKE " + Esc(std::string("%") + value + "%");
}

std::string Null()
{
    return "NULL";
}

std::string True()
{
    return "1";
}

std::string False()
{
    return "0";
}

std::string All()
{
    return "*";
}

std::string ValueList(const std::vector<std::string>& list)
{
    std::string s;
    if (list.empty()) {
        return s;
    }

    s += Null();
    for (auto&& l : list) {
        s += "," + Esc(l);
    }
    return s;
}

std::string KeyList(const std::vector<std::string>& list)
{
    std::string s;
    if (list.empty()) {
        return s;
    }

    s += Null();
    for (auto&& l : list) {
        s += "," + l;
    }
    return s;
}

std::string Unique(const std::string& s)
{
    return std::string("DISTINCT ") + s;
}

std::string And(const std::vector<std::string>& list)
{
    if (list.empty()) {
        return "1";
    }
    std::string where = "(";
    where += list[0];
    for (auto i = 1; i < list.size(); ++i) {
        where += " AND " + list[i];
    }
    where += ")";
    return where;
}

std::string Or(const std::vector<std::string>& list)
{
    if (list.empty()) {
        return "1";
    }
    std::string where = "(";
    where += list[0];
    for (auto i = 1; i < list.size(); ++i) {
        where += " OR " + list[i];
    }
    where += ")";
    return where;
}

SQLite3::SQLite3(const char* name)
{
    if (sqlite3_open(name,&mDB)) {
        throw std::runtime_error(std::string("Can't open database ") + name);
    }
}

SQLite3::~SQLite3()
{
    sqlite3_close(mDB);
}

void SQLite3::select(const std::string& select, const std::string& from,
            const std::string& where, const DataMapFn& f)
{
    std::string sql("SELECT ");
    sql += select + " FROM " + from + " WHERE " + where;
    exec(sql,f);
}

std::string SQLite3::insert(const std::string& insert, const std::string& values)
{
    std::string sql("INSERT INTO ");
    sql += insert + " VALUES(" + values + ")";
    exec(sql);
    return std::to_string(sqlite3_last_insert_rowid(mDB));
}

void SQLite3::update(const std::string& update, const std::string& set,
        const std::string& where)
{
    std::string sql("UPDATE ");
    sql += update + " SET " + set + " WHERE " + where;
    exec(sql);
}

void SQLite3::del(const std::string& del, const std::string& where)
{
    std::string sql("DELETE FROM ");
    sql += del + " WHERE " + where;
    exec(sql);
}

void SQLite3::exec(const std::string& statement, const DataMapFn& f)
{
    sqlite3_stmt* s;
    if (sqlite3_prepare_v2(mDB,statement.c_str(),-1,&s,nullptr) == SQLITE_OK) {
        auto cols = sqlite3_column_count(s);
        while (true) {
            auto result = sqlite3_step(s);

            if (result == SQLITE_ROW) {
                DataMap ret;
                for (auto i = 0; i < cols; ++i) {
                    auto* data =
                        reinterpret_cast<const char*>(sqlite3_column_text(s,i));
                    ret.insert({sqlite3_column_name(s,i),data ? data : "NULL"});
                }
                f(ret);
            } else {
                break;
            }
        }
    } else {
        throw std::runtime_error(sqlite3_errmsg(mDB));
    }
}

}
