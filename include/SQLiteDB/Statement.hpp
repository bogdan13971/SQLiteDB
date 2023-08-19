#pragma once

#include <string>
#include <memory>
#include "types.hpp"

namespace sqlitedb
{
namespace internal
{
class StatementInternal;
}

/**
	* @brief Simple wrapper over sqlite prepared statement
	* @info Created only by a SQLiteDatabase object
*/
class Statement
{
private:
	Statement(internal::StatementInternal&&);
	std::unique_ptr<internal::StatementInternal> pimpl;

public:
	Statement(const Statement&) = delete;
	Statement& operator=(const Statement&) = delete;
	Statement(Statement&&) noexcept;
	Statement& operator=(Statement&&) noexcept;

	/**
	 * @brief Bind values to the query
	 * @note Values number should match to the ones in the query
	 * @throws StatementException if binding fails
	*/
	template<class... Args>
	void bind(Args&&... args)
	{
		bindHelper(*this, 1, std::forward<Args>(args)...);
	}

	void bindType(int index, int value);
	void bindType(int index, double value);
	void bindType(int index, const std::string& value);
	void bindType(int index, std::string&& value);
	void bindType(int index, int64_t value);

	/**
	 * @brief Executes the prepared statement
	 * @return True if any data is returned, False otherwise
	 * @throws StatementException if can't execute query
	*/
	bool execute();

	/**
	 * @brief Resets statement to be rebinded
	*/
	void reset();

	/**
	 * @brief Retrieve a tuple of values from a statement
	 * @note Can be called as long as execute return true
	 * @warn Calling this when execute threw is undefined behaviour
	*/
	template<class... Args>
	std::tuple<Args...> retrieve()
	{
		return retrieveHelper<Args...>(0);
	}

	/**
	 * @brief Clear bindings for statement
	*/
	void clear();

	~Statement();

	friend class SQLiteDatabase;

private:
	template<class T>
	T retrieveType(int index);

	template<class T>
	std::tuple<T> retrieveHelper(int index)
	{
		return std::make_tuple(retrieveType<T>(index));
	}

	template<class First, class Second, class... Rest>
	std::tuple<First, Second, Rest...> retrieveHelper(int index)
	{
		auto head = std::make_tuple(retrieveType<First>(index));
		auto tail = retrieveHelper<Second, Rest...>(index + 1);
		return std::tuple_cat(head, tail);
	}
};

} //sqlitedb