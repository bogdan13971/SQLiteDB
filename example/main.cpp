#include <SQLiteDB/SQLiteDatabase.hpp>
#include <SQLiteDB/Statement.hpp>
#include <SQLiteDB/SQLiteException.hpp>
#include <SQLiteDB/BulkInserter.hpp>
#include <string>
#include <iostream>
#include "SQLiteDB/types.hpp"

int main()
{
	using namespace sqlitedb;

	using ENTRY = std::tuple<std::string, int, double>;

	SQLiteDatabase db;
	try {
		db.init("../../example/test.db");

		Statement drop_stmt = db.createStatement("DELETE FROM test_table;");
		Statement insert_stmt = db.createStatement("INSERT INTO test_table VALUES(@name, @id, @price);");
		Statement select_stmt = db.createStatement("SELECT * FROM test_table LIMIT @offset, @count;");

		drop_stmt.execute();
		drop_stmt.reset();

		auto bulkInsert = [&]() {
			auto data = { std::make_tuple("name1", 12, 12.54),
										std::make_tuple("name2", 13, 13.56),
										std::make_tuple("name3", 14, 14.87) };

			auto bulkInserter = db.createBulkInserter(insert_stmt);
			bulkInserter.start();
			for (const auto& element : data)
			{
				bulkInserter.add(std::get<0>(element), std::get<1>(element), std::get<2>(element));
			}
			bulkInserter.commit();
		};

		auto insertWithStmt = [&]() {
			insert_stmt.bind("Sample name", 170, 12.24);
			insert_stmt.execute();
			insert_stmt.reset();
			insert_stmt.clear();

			std::string a = "Other name";
			insert_stmt.bind(a, 200, -0.42);
			insert_stmt.execute();
			insert_stmt.reset();
			insert_stmt.clear();
		};

		//wrapper, also uses transaction
		auto insertWIthDB = [&]() {
			insert_stmt.bind("Sample again", 142, -0.32);
			db.executeInsert(insert_stmt);

			insert_stmt.bind("Other sample again", 28, -23.32);
			db.executeInsert(insert_stmt);
		};

		insertWithStmt();
		std::cout << "Inserted with stmt\n";

		bulkInsert();
		std::cout << "Bulk inserted \n";

		insertWIthDB();
		std::cout << "Inserted with db\n";

		auto print = [](const ENTRY& one)
		{
			std::cout << "Retrieved: " <<
				std::get<0>(one) << ", " <<
				std::get<1>(one) << ", " <<
				std::get<2>(one) << "\n";
		};

		struct Obj
		{
			std::string field1;
			int field2;
			double field3;
		};

		std::function<Obj(const ENTRY&)> func = [](const auto& result)
		{
			return Obj{ std::get<0>(result), std::get<1>(result), std::get<2>(result) };
		};

		auto selectWithStmt = [&]() {
			select_stmt.bind(0, 1);
			select_stmt.execute();
			auto one = select_stmt.retrieve<std::string, int, double>();
			print(one);
			select_stmt.reset();
			select_stmt.clear();

			select_stmt.bind(1, 1);
			select_stmt.execute();
			one = select_stmt.retrieve<std::string, int, double>();
			print(one);
			select_stmt.reset();
			select_stmt.clear();

			select_stmt.bind(1, 1);
			select_stmt.execute();
			auto obj = select_stmt.retrieve(func);
			
			std::cout << obj.field1 << " " << obj.field2 << " " << obj.field3 << "\n";

			select_stmt.reset();
			select_stmt.clear();
		};

		auto selectWithDB = [&]() {
			select_stmt.bind(0, 10);
			auto all = db.executeSelect<std::string, int, double>(select_stmt);

			return all;
		};

		auto selectWithMapper = [&]() {
			select_stmt.bind(0, 10);
			auto all = db.executeSelect(select_stmt, func);

			return all;
		};

		selectWithStmt();

		std::cout << "Printin all\n";
		auto all = selectWithDB();
		for (const auto& one : all)
		{
			print(one);
		}

		auto some = selectWithMapper();
	}
	catch (const SQLiteException& e)
	{
		std::cout << e.what() << "\n";
	}
	catch (const std::exception& e)
	{
		std::cout << e.what() << "\n";
	}

	return 0;
}