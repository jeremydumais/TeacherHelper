#pragma once

#include <string>

class IFileSystemOperations
{
public:
    virtual ~IFileSystemOperations() {};
    virtual bool fileExists(const std::string &fileName) = 0;
};