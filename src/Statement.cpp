#include "Statement.hpp"
#include "internal/StatementInternal.hpp"
#include "SQLiteException.hpp"

using namespace sqlitedb;
using namespace sqlitedb::internal;
using sqlitedb::SQLiteException;

Statement::Statement(StatementInternal&& internal)
{
	pimpl = std::make_unique<StatementInternal>(std::move(internal));
}

Statement::~Statement()
{
}

Statement::Statement(Statement&& other) noexcept
{
	pimpl = std::move(other.pimpl);
}

Statement& Statement::operator=(Statement&& other) noexcept
{
	pimpl = std::move(other.pimpl);
	clear();

	return *this;
}

bool Statement::execute()
{
	return pimpl->execute();
}

void Statement::reset()
{
	pimpl->reset();
}

void Statement::clear()
{
	pimpl->clear();
}

void Statement::bindType(int index, int value)
{
	pimpl->bindType(index, value);
}

void Statement::bindType(int index, double value)
{
	pimpl->bindType(index, value);
}

void Statement::bindType(int index, const std::string& value)
{
	pimpl->bindType(index, value);
}

void Statement::bindType(int index, std::string&& value)
{
	pimpl->bindType(index, std::move(value));
}

template<>
int Statement::retrieveType(int index)
{
	return pimpl->retrieveType<int>(index);
}

template<>
double Statement::retrieveType(int index)
{
	return pimpl->retrieveType<double>(index);
}

template<>
std::string Statement::retrieveType(int index)
{
	return pimpl->retrieveType<std::string>(index);
}

