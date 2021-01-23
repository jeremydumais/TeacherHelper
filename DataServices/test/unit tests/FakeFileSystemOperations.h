#pragma once

#include "IFileSystemOperations.h"

class FakeFileSystemOperations : public IFileSystemOperations
{
public:
    const std::string &getLastError() override {
		return "";
	}
	bool fileExists(const std::string &fileName) override {
		return isFileExists;
	}
	bool fileCopy(const std::string &source, const std::string &destination)  override {
		return isFileCopySucceed;
	}

	bool isFileExists;
	bool isFileCopySucceed;
};