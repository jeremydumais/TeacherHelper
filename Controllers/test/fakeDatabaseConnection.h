#pragma once

#include "IDatabaseConnection.h"
#include <string>

class FakeDatabaseConnection : public IDatabaseConnection
{
public:
    FakeDatabaseConnection()
		: dbName("dbtest"),
		  isDBOpened(false) {}
	const std::string &getDbName() const override { return dbName; };
    void *getConnectionPtr() const override { return nullptr; };
    void open() override {};
	bool isOpened() const override { return isDBOpened; }
    void close() override {};
	std::string dbName;
	bool isDBOpened;
};