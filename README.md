---
Title: mindbw
Description: MINimal DataBase Wrapper
Author: Deon Poncini

---
mindbw
===============

Developed by Deon Poncini <dex1337@gmail.com>

Description
-----------
A very minimal C++ database wrapper that enables simple database transactions
from applications.

Building
--------
Use the [scripts](http://github.com/DeonPoncini/scripts) build system to build
this library, just add mindbw to your manifest xml file with the appropriate
remote

Usage
-----
Currently mindbw only supports SQLite3 but hopefully will encompass other SQL
type databases in the future (mysql, postgres etc).

Construct a database object by passing a full or relative path to a database
file:

    SQLite3 db("path/to/database.db");

From there, various functions are available:

    db.select("column", "table_name", "predicate", &callback);
    db.insert("table", "values");
    db.update("column", "to_value", "condition");
    db.del("value", "condition");
    db.exec("command", &callback);

Callbacks need to be of type mindbw::DataMapFn, which takes a DataMap (map of
keys to values as strings) and returns void.

To assist in the string constructions, there are a series of wrapper functions
that are used to construct lists, conjunctions and disjunctions.
For example:

    db.select(KeyList({"k1","k2"}), "table", And(Equal("k1","value")),
        Compare("k2",3,Operator::GT), [&](DataMap d) {
               std::cout << "k1: " << data["k1"] << std::endl;
               std::cout << "k2: " << data["k2"] << std::endl;
            });

This selects keys "k1" and "k2" from table "table" where the name of "k1" is
value and the value of "k2" is greater than 3, and prints them out.

The return function gets called once per row, so if there were 10 results in
the table the function would be called 10 times and print 20 rows of key value
pairs.

License
-------
Copyright (c) 2014 Deon Poncini.
See the LICENSE file for license rights and limitations (MIT)
