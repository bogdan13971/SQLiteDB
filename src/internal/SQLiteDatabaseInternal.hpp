#pragma once

#include <functional>
#include <vector>

#include "StatementInternal.hpp"

struct sqlite3;

namespace sqlitedb
{
namespace internal
{

class SQLiteDatabaseInternal
{
private:
	sqlite3* db;

public:
	SQLiteDatabaseInternal();
	SQLiteDatabaseInternal(const SQLiteDatabaseInternal&) = delete;
	SQLiteDatabaseInternal& operator=(const SQLiteDatabaseInternal&) = delete;
	SQLiteDatabaseInternal(SQLiteDatabaseInternal&&) noexcept;
	SQLiteDatabaseInternal& operator=(SQLiteDatabaseInternal&&) noexcept;

	void init(const char* path);

	StatementInternal createStatement(const char* query);

	void beginTransaction();

	void endTransaction();

	void executeInsert(StatementInternal& stmt);

	~SQLiteDatabaseInternal();
};

}//internal
}//sqlitedb