#include <string>

#include <sqlite3.h>

#include "StatementInternal.hpp"
#include "SQLiteException.hpp"

using namespace sqlitedb;
using namespace sqlitedb::internal;
using sqlitedb::SQLiteException;

StatementInternal::StatementInternal()
	: stmt{nullptr}
{
}

StatementInternal StatementInternal::create(sqlite3* db, const char* query)
{
	if (db == nullptr)
	{
		throw SQLiteException("Can't create statement on null database");
	}

	if (query == nullptr)
	{
		throw SQLiteException("Query can't be null");
	}

	StatementInternal statementInternal;

	int ec = sqlite3_prepare_v2(db, query, strlen(query), &(statementInternal.stmt), nullptr);
	if (ec != SQLITE_OK)
	{
		const char* m = sqlite3_errmsg(db);
		throw SQLiteException("Prepare statement failed: " + std::string(m), ec);
	}

	return statementInternal;
}

StatementInternal::~StatementInternal()
{
	sqlite3_finalize(stmt);
}

StatementInternal::StatementInternal(StatementInternal&& other) noexcept
{
	stmt = other.stmt;
	clear();
	other.stmt = nullptr;
}

StatementInternal& StatementInternal::operator=(StatementInternal&& other) noexcept
{
	stmt = other.stmt;
	clear();
	other.stmt = nullptr;

	return *this;
}

void StatementInternal::bindType(int index, int value)
{
	int ec = sqlite3_bind_int(stmt, index, value);
	if (ec != SQLITE_OK)
	{
		throw SQLiteException("StatementInternal bind int error ", ec);
	}
}

void StatementInternal::bindType(int index, double value)
{
	int ec = sqlite3_bind_double(stmt, index, value);
	if (ec != SQLITE_OK)
	{
		throw SQLiteException("StatementInternal bind double error ", ec);
	}
}

void StatementInternal::bindType(int index, const std::string& value)
{
	int ec = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
	if (ec != SQLITE_OK)
	{
		throw SQLiteException("StatementInternal bind string error ", ec);
	}
}

void StatementInternal::bindType(int index, std::string&& value)
{
	int ec = sqlite3_bind_text(stmt, index, value.c_str(), -1, SQLITE_TRANSIENT);
	if (ec != SQLITE_OK)
	{
		throw SQLiteException("StatementInternal bind string error ", ec);
	}
}

bool StatementInternal::execute()
{
	int ec = sqlite3_step(stmt);

	if (ec == SQLITE_DONE)
	{
		return false;
	}
	else if (ec == SQLITE_ROW)
	{
		return true;
	}
	else
	{
		throw SQLiteException("StatementInternal step no handle", ec);
	}
}

void StatementInternal::reset()
{
	sqlite3_reset(stmt);
}

template<>
int StatementInternal::retrieveType(int index)
{
	return sqlite3_column_int(stmt, index);
}

template<>
double StatementInternal::retrieveType(int index)
{
	return sqlite3_column_double(stmt, index);
}

template<>
std::string StatementInternal::retrieveType(int index)
{
	const char* retrieved = reinterpret_cast<const char*>(sqlite3_column_text(stmt, index));
	return std::string(retrieved);
}

void StatementInternal::clear()
{
	sqlite3_clear_bindings(stmt);
}

const char* StatementInternal::getQueryString() const
{
	return sqlite3_sql(stmt);
}

int StatementInternal::getBindCount() const
{
	return sqlite3_bind_parameter_count(stmt);
}