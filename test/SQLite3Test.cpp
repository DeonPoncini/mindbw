#define BOOST_TEST_MODULE SQLite3
#include <boost/test/unit_test.hpp>
#include <mindbw/SQLite3.h>

struct SQLite3_Fixture
{
    SQLite3_Fixture() :
        sqlite3("test/test.db")
    {
    }

    mindbw::SQLite3 sqlite3;

};

BOOST_FIXTURE_TEST_SUITE(SQLite3, SQLite3_Fixture)

BOOST_AUTO_TEST_CASE(Insert)
{
    sqlite3.insert("test", mindbw::ValueList({"TestName", "500"}));
    auto selCount = 0;
    sqlite3.select(mindbw::KeyList({"name", "value"}), "test",
            mindbw::Equal("name","TestName"),
            [&](mindbw::DataMap data) {
                selCount++;
                BOOST_CHECK_EQUAL(data["name"], "TestName");
                BOOST_CHECK_EQUAL(data["value"], "500");
            });
    BOOST_CHECK_EQUAL(selCount, 1);
}

BOOST_AUTO_TEST_CASE(Update)
{
    sqlite3.update("test", "value = 200", mindbw::Equal("name", "TestName"));
    auto selCount = 0;
    sqlite3.select(mindbw::KeyList({"name", "value"}), "test",
            mindbw::Equal("name","TestName"),
            [&](mindbw::DataMap data) {
                selCount++;
                BOOST_CHECK_EQUAL(data["name"], "TestName");
                BOOST_CHECK_EQUAL(data["value"], "200");
            });
    BOOST_CHECK_EQUAL(selCount, 1);
}

BOOST_AUTO_TEST_CASE(Delete)
{
    sqlite3.del("test", mindbw::Equal("name", "TestName"));
    auto selCount = 0;
    sqlite3.select(mindbw::KeyList({"name", "value"}), "test",
            mindbw::Equal("name","TestName"),
            [&](mindbw::DataMap data) {
                selCount++;
            });
    BOOST_CHECK_EQUAL(selCount, 0);
}

BOOST_AUTO_TEST_SUITE_END()
