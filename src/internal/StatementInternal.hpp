#pragma once

#include <string>

struct sqlite3;
struct sqlite3_stmt;

namespace sqlitedb
{
namespace internal
{

class StatementInternal
{
private:
	sqlite3_stmt* stmt;
	StatementInternal();

	void create(sqlite3* db, const char* query);

public:
	StatementInternal(const StatementInternal&) = delete;
	StatementInternal& operator=(const StatementInternal&) = delete;
	StatementInternal(StatementInternal&&) noexcept;
	StatementInternal& operator=(StatementInternal&&) noexcept;

	bool execute();

	void reset();

	void clear();

	~StatementInternal();

	friend class SQLiteDatabaseInternal;

	void bindType(int index, int value);
	void bindType(int index, double value);
	void bindType(int index, const std::string& value);

	template<class T>
	T retrieveType(int index);
};

}//internal
} //sqlitedb