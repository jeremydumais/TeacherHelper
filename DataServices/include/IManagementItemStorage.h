#pragma once

#include <list>

template <class T>
class IManagementItemStorage
{
public:
    virtual ~IManagementItemStorage() {};
    virtual std::list<T> getAllItems() = 0;
    virtual const std::string &getLastError() const = 0;
    virtual bool insertItem(const T &city) = 0;
    virtual bool updateItem(const T &city) = 0;
    virtual bool deleteItem(size_t id) = 0;
};