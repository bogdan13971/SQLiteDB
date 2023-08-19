# SQLite DB
Simple C++ wrapper over sqlite3.

#### Components
The project contains a wrapper over an sqlite database and another one for an sqlite prepared statement. Each statement can be executed standalone or by the database inside a transacion. 

## Usage
Creating or opening an existing database. Any sqlite error codes are also wrapped in an SQLiteException.
```cpp
SQLiteDatabase db;
try {
    db.init("test.db");
}
catch (const SQLiteException& e)
{
    std::cout << e.what() << "\n";
}
```
Creating a statement can only be done by the database. The statement is created using an sql string with named arguments format. 
A statement can bind to any number of parameters but only for types int, double, string and int64.
Executing a statement by the database ensures that the operation is inside a transaction and the statement is cleaned up after.
```cpp
Statement insert_stmt = db.createStatement("INSERT INTO test_table VALUES(@name, @id, @price);");
insert_stmt.bind("Sample name", 170, 12.24);
insert_stmt.execute();
insert_stmt.reset();
insert_stmt.clear();

insert_stmt.bind("Sample again", 142, -0.32);
db.executeInsert(insert_stmt);
```
Bulk insertion is done via a BulkInserter created by the database from an insert statement. Data is aggregated inside a transaction until the BulkInserter commits.
```cpp
auto data = { std::make_tuple("name1", 12, 12.54),
              std::make_tuple("name2", 13, 13.56),
              std::make_tuple("name3", 14, 14.87) };

auto bulkInserter = db.createBulkInserter(insert_stmt);
bulkInserter.start();
for (const auto& element : data)
{
    bulkInserter.add(std::get<0>(element), std::get<1>(element), std::get<2>(element));
}
bulkInserter.commit();
```
Getting an element from a select statement can be done either from executing the statement standalone or by using the database. The only difference is that executing the statement gives the first element and data has to be continously polled from the statement. The database version returns a vector containing all the elements of the query. The type of the retrieved data must be specified explicitly.
```cpp
Statement select_stmt = db.createStatement("SELECT * FROM test_table");
auto one = select_stmt.retrieve<std::string, int, double>();
print(one);

auto all = db.executeSelect<std::string, int, double>(select_stmt);
for (const auto& one : all)
{
    print(one);
}
```
A more detailed example in *example/main.cpp*.