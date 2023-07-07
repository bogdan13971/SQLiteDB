#pragma once
#include <exception>
#include <string>

namespace sqlitedb
{

struct SQLiteException : public std::exception
{
	std::string message;

	SQLiteException(const char* message, int error)
		: message{message}
	{
		this->message.append(" , Error Code : ").append(std::to_string(error));
	}

	SQLiteException(const std::string& message, int error)
		: message{ message }
	{
		this->message.append(" , Error Code : ").append(std::to_string(error));
	}

	const char* what() const override
	{
		return message.c_str();
	}
};

} // sqlitedb
