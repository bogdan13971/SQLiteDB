#include <sqlite3.h>

#include "SQLiteDatabaseInternal.hpp"
#include "SQLiteException.hpp"

using namespace sqlitedb::internal;
using sqlitedb::SQLiteException;

SQLiteDatabaseInternal::SQLiteDatabaseInternal()
	:db{ nullptr }
{}

SQLiteDatabaseInternal::SQLiteDatabaseInternal(SQLiteDatabaseInternal&& other) noexcept
{
	db = other.db;
	other.db = nullptr;
}

SQLiteDatabaseInternal& SQLiteDatabaseInternal::operator=(SQLiteDatabaseInternal&& other) noexcept
{
	db = other.db;
	other.db = nullptr;

	return *this;
}

void SQLiteDatabaseInternal::init(const char* path)
{
	int ec = sqlite3_open(path, &db);
	if (ec != SQLITE_OK)                             
	{							             
		const char* m = sqlite3_errmsg(db);	 
		throw SQLiteException(m, ec);        
	}
}

StatementInternal SQLiteDatabaseInternal::createStatement(const char* query)
{
	StatementInternal stmt;
	stmt.create(db, query);

	return stmt;
}

void SQLiteDatabaseInternal::beginTransaction()
{
	int ec = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
	if (ec != SQLITE_OK)
	{
		const char* m = sqlite3_errmsg(db);
		throw SQLiteException(m, ec);
	}
}

void SQLiteDatabaseInternal::endTransaction()
{
	int ec = sqlite3_exec(db, "END TRANSACTION", nullptr, nullptr, nullptr);
	if (ec != SQLITE_OK)
	{
		const char* m = sqlite3_errmsg(db);
		throw SQLiteException(m, ec);
	}
}

void SQLiteDatabaseInternal::executeInsert(StatementInternal& stmt)
{
	beginTransaction();

	stmt.execute();
	stmt.reset();
	stmt.clear();

	endTransaction();
}

SQLiteDatabaseInternal::~SQLiteDatabaseInternal()
{
	sqlite3_close(db);
	db = nullptr;
}