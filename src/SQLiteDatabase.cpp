#include "SQLiteDatabase.hpp"

#include "internal/SQLiteDatabaseInternal.hpp"

using namespace sqlitedb;
using namespace sqlitedb::internal;

SQLiteDatabase::SQLiteDatabase()
{
	pimpl = std::make_unique<SQLiteDatabaseInternal>();
}

SQLiteDatabase::SQLiteDatabase(SQLiteDatabase&& other) noexcept
{
	pimpl = std::move(other.pimpl);
}

SQLiteDatabase& SQLiteDatabase::operator=(SQLiteDatabase&& other) noexcept
{
	pimpl = std::move(other.pimpl);

	return *this;
}

void SQLiteDatabase::init(const char* path)
{
	pimpl->init(path);
}

Statement SQLiteDatabase::createStatement(const char* query)
{
	return pimpl->createStatement(query);
}

void SQLiteDatabase::beginTransaction()
{
	pimpl->beginTransaction();
}

void SQLiteDatabase::endTransaction()
{
	pimpl->endTransaction();
}

void SQLiteDatabase::executeInsert(Statement& stmt)
{
	pimpl->executeInsert(*(stmt.pimpl));
}

SQLiteDatabase::~SQLiteDatabase()
{
}