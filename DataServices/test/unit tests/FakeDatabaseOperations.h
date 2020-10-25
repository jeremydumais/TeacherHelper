#pragma once

#include "IDatabaseOperations.h"
#include <string>

class FakeDatabaseOperations : public IDatabaseOperations
{
public:
	int open(const std::string &fileName) override {
		return openReturnValue;
	};

	int openForCreation(const std::string &fileName) override {
		return openForCreationReturnValue;
	};

    int close() override {
		return closeReturnValue;
	};

    void *getConnectionPtr() const override {
		return getConnectionPtrReturnValue;
	};

	int openReturnValue;
	int openForCreationReturnValue;
	int closeReturnValue;
	void *getConnectionPtrReturnValue;
};