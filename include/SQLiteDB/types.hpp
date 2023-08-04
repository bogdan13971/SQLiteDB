#pragma once

#include <type_traits>
#include <string>
#include <tuple>

namespace sqlitedb
{

/**
 * SQLite basic types are INTEGER, REAL and TEXT
 * They correspond to int, double and string
*/

/**
 * @brief A sqlite entity is a tuple of variadic size than
 * Can be binded to a prepared statement
 * @note Strings are stored in the entity as references (lvalue or rvalue)
 * Create entities using the make_entity helper function
*/
template<typename ...T>
using sqlite_entity = std::tuple<T...>;

template<typename T>
struct SQLiteType : std::false_type
{};

template<>
struct SQLiteType<int> : std::true_type
{};

template<>
struct SQLiteType<double> : std::true_type
{};

template<>
struct SQLiteType<std::string> : std::true_type
{};

template<>
struct SQLiteType<const char*> : std::true_type
{};

template<size_t N>
struct SQLiteType<const char(&)[N]> : std::true_type
{};

template<class T>
constexpr bool is_sqlite_type()
{ 
	return SQLiteType<T>::value ||
		SQLiteType<std::remove_const_t<std::remove_reference_t<T>>>::value;
};

template<class T>
struct StringType
{
	using type = typename std::is_same<std::string, std::remove_reference_t<T>>::type;
};

template<size_t N>
struct StringType<const char(&)[N]> : std::true_type
{};

template<typename T>
decltype(auto) make_entity_single(T&& value, std::false_type)
{
	return std::make_tuple<T>(std::forward<T>(value));
}

//std::tuple<std::string&&> make_entity_single(const char* value, std::false_type)
//{
//	return std::make_tuple<std::string&&>(value);
//}

template<typename T>
decltype(auto) make_entity_single(T&& value, std::true_type)
{
	return std::forward_as_tuple<T>(std::forward<T>(value));
}

template<typename T>
decltype(auto) make_entity_dispatcher(T&& value)
{
	return make_entity_single(std::forward<T>(value), StringType<T>::type());
}

template<typename First>
decltype(auto) make_entity(First&& first)
{
	return make_entity_dispatcher(std::forward<First>(first));
}

/**
* @brief Create a entity from a list of values
* @note String arguments will be passed and stored as references (lvalues or rvalues)
*/
template<typename First, typename ...Rest>
decltype(auto) make_entity(First&& first, Rest&& ...rest)
{
	static_assert(SQLiteType<std::remove_reference_t<First>>::value, "Type is not int, double or string");
	return std::tuple_cat(
		make_entity_dispatcher(std::forward<First>(first)),
		make_entity(std::forward<Rest>(rest)...)
	);
}

template<class T, class Stmt>
void bindSingle(Stmt& stmt, int index, T&& value, std::false_type)
{
	stmt.bindType(index, value);
}

template<class T, class Stmt>
void bindSingle(Stmt& stmt, int index, T&& value, std::true_type)
{
	stmt.bindType(index, std::forward<T>(value));
}

template<class T, class Stmt>
void bindSingleDispatch(Stmt& stmt, int index, T&& value)
{
	bindSingle(stmt, index, std::forward<T>(value), StringType<T>::type());
}

template<class First, class Stmt>
void bindHelper(Stmt& stmt, int index, First&& value)
{
	static_assert(is_sqlite_type<First>(), "Type is not int, double or string");
	bindSingleDispatch(stmt, index, std::forward<First>(value));
}

template<class First, class... Rest, class Stmt>
void bindHelper(Stmt& stmt, int index, First&& value, Rest&&... rest)
{
	static_assert(is_sqlite_type<First>(), "Type is not int, double or string");
	bindSingleDispatch(stmt, index, std::forward<First>(value));
	bindHelper(stmt, index + 1, std::forward<Rest>(rest)...);
}

}