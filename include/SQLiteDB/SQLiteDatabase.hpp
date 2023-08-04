#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <array>
#include <cassert>
#include <cstring>

#include "Statement.hpp"

namespace sqlitedb
{
	namespace internal
	{
		class SQLiteDatabaseInternal;
	}

	/**
	 * @brief Simple wrapper over sqlite3 db
	*/
	class SQLiteDatabase
	{
	private:
		std::unique_ptr<internal::SQLiteDatabaseInternal> pimpl;

	public:
		SQLiteDatabase();
		SQLiteDatabase(const SQLiteDatabase&) = delete;
		SQLiteDatabase& operator=(const SQLiteDatabase&) = delete;
		SQLiteDatabase(SQLiteDatabase&&) noexcept;
		SQLiteDatabase& operator=(SQLiteDatabase&&) noexcept;

		/**
		 * @brief Initialize database from path
		 * @throws SQLiteException on error
		*/
		void init(const char* path);

		/**
		 * @brief Creates a Statement
		 * @param query SQLite query used for the statement
		 * @throws StatementException on error 
		*/
		Statement createStatement(const char* query);

		/**
		 * @brief Starts a SQL transaction
		*/
		void beginTransaction();

		/**
		 * @brief Ends a SQL transaction
		*/
		void endTransaction();

		/**
		 * @brief Executes an insert statement in a transaction
		 * @param stmt Statement created using an INSERT query and binded 
		 * @note Statement is cleread after
		*/
		void executeInsert(Statement& stmt);

		/**
		 * @brief Executes a select statement in a transaction
		 * @tparam T Type of object to be retrieved
		 * @param stmt Statement created using a SELECT query
		 * @param builder Function that creates an object T from a statement
		 * @return a vector containing all results of the select
		*/
		template<class... Args>
		std::vector<std::tuple<Args...>> executeSelect(Statement& stmt)
		{
			std::vector<std::tuple<Args...>> result;

			beginTransaction();

			while (stmt.execute())
			{
				result.push_back(stmt.retrieve<Args...>());
			}
			stmt.reset();
			stmt.clear();

			endTransaction();

			return result;
		}

		/**
		 * @brief Batch inserts values in a transaction
		 * @param stmt Statement created using an INSERT query
		 * @param entries Entries to be added
		*/
		void executeBatchInsert(Statement& stmt, unsigned short entries);

		~SQLiteDatabase();
	};

}//sqlitedb