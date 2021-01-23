#pragma once

#include "IDatabaseConnection.h"
#include "IFileSystemOperations.h"
#include <boost/signals2.hpp>
#include <string>

class IDatabaseManagementOperations
{
public:
    virtual ~IDatabaseManagementOperations() {};
    virtual const std::string &getLastError() const = 0;
    virtual bool exist(const std::string &databaseName) const = 0;
    virtual bool create(const std::string &databaseName) = 0;
    virtual bool upgrade(IDatabaseConnection &connection) = 0;
    boost::signals2::signal<void(size_t progress, const std::string &message)> onUpgradeProgress;
};