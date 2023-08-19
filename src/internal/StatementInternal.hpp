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

public:
	static StatementInternal create(sqlite3* db, const char* query);

public:
	StatementInternal();
	~StatementInternal();
	StatementInternal(const StatementInternal&) = delete;
	StatementInternal& operator=(const StatementInternal&) = delete;
	StatementInternal(StatementInternal&&) noexcept;
	StatementInternal& operator=(StatementInternal&&) noexcept;

	/**
	 * @brief Executes the statement
	 * @return True if can be executed further to retrieve data, False otherwise
	*/
	bool execute();

	void reset();

	void clear();

	void bindType(int index, int value);
	void bindType(int index, double value);
	void bindType(int index, const std::string& value);
	void bindType(int index, std::string&& value);
	void bindType(int index, int64_t value);

	template<class T>
	T retrieveType(int index);

	const char* getQueryString() const;
	int getBindCount() const;
};

}//internal
} //sqlitedb