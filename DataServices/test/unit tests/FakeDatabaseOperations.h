#pragma once

#include "IDatabaseOperations.h"
#include <string>

class FakeDatabaseOperations : public IDatabaseOperations
{
public:
	int open(const std::string &fileName) override {
		return openReturnValue;
	};

    int close() override {
		return closeReturnValue;
	};

    void *getConnectionPtr() const override {
		return getConnectionPtrReturnValue;
	};

	int openReturnValue;
	int closeReturnValue;
	void *getConnectionPtrReturnValue;
};