#pragma once

#include <string>

class IFileSystemOperations
{
public:
    virtual ~IFileSystemOperations() {};
    virtual const std::string &getLastError() = 0;
    virtual bool fileExists(const std::string &fileName) = 0;
    virtual bool fileCopy(const std::string &source, const std::string &destination) = 0;
    virtual bool fileCopyWithOverwrite(const std::string &source, const std::string &destination) = 0;
    virtual std::string extractFolderPath(const std::string &fullPathWithFileName) const = 0;
};