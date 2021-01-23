#pragma once

#include "IFileSystemOperations.h"

class FakeFileSystemOperations : public IFileSystemOperations
{
public:
    const std::string &getLastError() const override {
		return lastError;
	}

	bool fileExists(const std::string &fileName) override {
		return isFileExists;
	}

	bool fileCopy(const std::string &source, const std::string &destination) override {
		return isFileCopySucceed;
	}

	bool fileCopyWithOverwrite(const std::string &source, const std::string &destination) override {
		return isFileCopyWithOverwriteSucceed;
	}

    std::string extractFolderPath(const std::string &fullPathWithFileName) const override {
		return extractedFolderPath;
	}

	std::string lastError { "" };
	std::string extractedFolderPath { "" };
	bool isFileExists;
	bool isFileCopySucceed;
	bool isFileCopyWithOverwriteSucceed;
};