#pragma once

#include <string>
#include <memory>
#include "types.hpp"

namespace sqlitedb
{

class SQLiteDatabase;
class Statement;

class BulkInserter
{
private:
	SQLiteDatabase& db;
	Statement& stmt;

private:
	void execute();

public:
	BulkInserter(SQLiteDatabase& db, Statement& stmt);

	void start();

	template<class ...Ts>
	void add(Ts&&... values)
	{
		stmt.bind(std::forward<Ts>(values)...);
		execute();
	}

	void commit();
};

} //sqlitedb