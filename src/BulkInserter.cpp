#include "BulkInserter.hpp"
#include "SQLiteDatabase.hpp"
#include "Statement.hpp"

using namespace sqlitedb;

BulkInserter::BulkInserter(SQLiteDatabase& db, Statement& stmt)
	: db{ db },
	stmt{ stmt }
{}

void BulkInserter::execute()
{
	stmt.execute();
	stmt.reset();
	stmt.clear();
}

void BulkInserter::start()
{
	db.beginTransaction();
}

void BulkInserter::commit()
{
	db.endTransaction();
}
