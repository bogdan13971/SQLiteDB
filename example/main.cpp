#include <SQLiteDB/SQLiteDatabase.hpp>
#include <SQLiteDB/Statement.hpp>
#include <SQLiteDB/SQLiteException.hpp>
#include <string>
#include <iostream>
#include "SQLiteDB/types.hpp"

int main()
{
	using namespace sqlitedb;

	int i = 32;

	std::string lval = "aaaa";
	std::string& lvalRef = lval;

	auto a = make_entity(i, 43, "asa vreau eu");

	//using ENTRY = std::tuple<std::string, int, double>;

	//auto buff = createBulkInsertQuery<4>("INSERT INTO table VALUES(@name, @id,")

	//SQLiteDatabase db;
	//try {
	//	db.init("../../example/test.db");

	//	Statement drop_stmt = db.createStatement("DELETE FROM test_table;");
	//	Statement insert_stmt = db.createStatement("INSERT INTO test_table VALUES(@name, @id, @price);");
	//	Statement select_stmt = db.createStatement("SELECT * FROM test_table LIMIT @offset, @count;");

	//	drop_stmt.execute();
	//	drop_stmt.reset();

	//	auto insertWithStmt = [&]() {
	//		insert_stmt.bind("Sample name", 170, 12.24);
	//		insert_stmt.execute();
	//		insert_stmt.reset();
	//		insert_stmt.clear();

	//		std::string a = "Other name";
	//		insert_stmt.bind(a, 200, -0.42);
	//		insert_stmt.execute();
	//		insert_stmt.reset();
	//		insert_stmt.clear();
	//	};

	//	//wrapper, also uses transaction
	//	auto insertWIthDB = [&]() {
	//		insert_stmt.bind("Sample again", 142, -0.32);
	//		db.executeInsert(insert_stmt);

	//		insert_stmt.bind("Other sample again", 28, -23.32);
	//		db.executeInsert(insert_stmt);
	//	};

	//	insertWithStmt();
	//	std::cout << "Inserted with stmt\n";

	//	insertWIthDB();
	//	std::cout << "Inserted with db\n";

	//	auto print = [](const ENTRY& one)
	//	{
	//		std::cout << "Retrieved: " <<
	//			std::get<0>(one) << ", " <<
	//			std::get<1>(one) << ", " <<
	//			std::get<2>(one) << "\n";
	//	};

	//	auto selectWithStmt = [&]() {
	//		select_stmt.bind(0, 1);
	//		select_stmt.execute();
	//		auto one = select_stmt.retrieve<std::string, int, double>();
	//		print(one);
	//		select_stmt.reset();
	//		select_stmt.clear();

	//		select_stmt.bind(1, 1);
	//		select_stmt.execute();
	//		one = select_stmt.retrieve<std::string, int, double>();
	//		print(one);
	//		select_stmt.reset();
	//		select_stmt.clear();
	//	};

	//	auto selectWithDB = [&]() {
	//		select_stmt.bind(0, 10);
	//		auto all = db.executeSelect<std::string, int, double>(select_stmt);

	//		return all;
	//	};

	//	selectWithStmt();

	//	std::cout << "Printin all\n";
	//	auto all = selectWithDB();
	//	for (const auto& one : all)
	//	{
	//		print(one);
	//	}
	//}
	//catch (const SQLiteException& e)
	//{
	//	std::cout << e.what() << "\n";
	//}
	//catch (const std::exception& e)
	//{
	//	std::cout << e.what() << "\n";
	//}

	return 0;
}