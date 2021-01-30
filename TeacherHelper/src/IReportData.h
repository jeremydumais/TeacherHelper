#pragma once

#include <string>

class IReportData
{
public:
    virtual ~IReportData() {};
    virtual std::string generateDataRow() = 0;
};