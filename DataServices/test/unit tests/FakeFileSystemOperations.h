#pragma once

#include "IFileSystemOperations.h"

class FakeFileSystemOperations : public IFileSystemOperations
{
public:
	bool fileExists(const std::string &fileName) override {
		return isFileExists;
	}
	bool isFileExists;
};