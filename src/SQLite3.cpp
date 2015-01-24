#include <mindbw/SQLite3.h>

#include <sqlite3.h>

#include <stdexcept>

#include <kizhi/Log.h>
#include <zephyr/to_string.h>

namespace mindbw
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
        case Operator::LET: return "=";
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

std::string Let(const std::string& key, const std::string& value)
{
    return Compare(key, value, Operator::LET);
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

SQLite3::SQLite3(const std::string& name) :
    SQLite3(name.c_str())
{
}

SQLite3::SQLite3(const char* name)
{
    if (sqlite3_open(name,&mDB)) {
        KIZHI_FATAL_T("SQLite3") << "Cannot open database: " << name
            << " : " << sqlite3_errmsg(mDB);
        throw std::runtime_error(std::string("Can't open database ") + name);
    }
    KIZHI_TRACE_T("SQLite3") << "Opening database " << mDB;
}

SQLite3::~SQLite3()
{
    KIZHI_TRACE_T("SQLite3") << "Closing database " << mDB;
    auto rc = sqlite3_close(mDB);
    if(rc == SQLITE_BUSY) {
        KIZHI_ERROR_T("SQLite3") << "Database closing failed! " << mDB;
        sqlite3_stmt* s;
        while((s = sqlite3_next_stmt(mDB, nullptr)) != nullptr) {
            sqlite3_finalize(s);
        }
        rc = sqlite3_close(mDB);
        if (rc != SQLITE_OK) {
            KIZHI_FATAL_T("SQLite3") << "Unable to finalize database! " << mDB;
        }
    }

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
    return zephyr::to_string(sqlite3_last_insert_rowid(mDB));
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
    KIZHI_TRACE_T("SQLite3") << statement;
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
                    auto col = sqlite3_column_name(s, i);
                    auto d = data ? data : "NULL";
                    KIZHI_TRACE_T("SQLite3") << col << "->" << d;
                    ret.insert({col,d});
                }
                f(ret);
            } else {
                break;
            }
        }
        // cleanup
        if (s != nullptr) {
            if (sqlite3_finalize(s) != SQLITE_OK) {
                KIZHI_ERROR_T("SQLite3") << "Finalize failed: "
                    << sqlite3_errmsg(mDB);
            }
        }
    } else {
        KIZHI_ERROR_T("SQLite3") << "Prepare failed: " << sqlite3_errmsg(mDB);
        throw std::runtime_error(std::string("prepare failed: ") +
                sqlite3_errmsg(mDB));
    }
}

}
