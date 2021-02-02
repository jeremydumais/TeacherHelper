#pragma once

#include <map>
#include <string>

class IReportData
{
public:
    virtual ~IReportData() {};
    virtual std::string generateDataRow() const = 0;
};