#pragma once

#include "IStorageUpdateOperation.h"
#include "queryResult.h"
#include <list>
#include <memory>

template <class T>
class IManagementItemStorage
{
public:
    virtual ~IManagementItemStorage() {};
    virtual std::list<T> getAllItems() = 0;
    virtual const std::string &getLastError() const = 0;
    virtual bool insertItem(const T &item) = 0;
    virtual bool updateItem(const T &item) = 0;
    virtual QueryResult deleteItem(size_t id) = 0;
};