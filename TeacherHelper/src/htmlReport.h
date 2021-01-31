#pragma once

#include "IReportData.h"
#include <map>
#include <memory>
#include <string>

class HTMLReport
{
public:
    HTMLReport(const std::string &reportFileName);
    const std::string &getReportFileName() const;
    void setData(std::shared_ptr<IReportData> data);
    void setProperties(const std::map<std::string, std::string> &properties);
    void previewReport();
private:
    std::string fileName;
    std::map<std::string, std::string> properties;
    std::shared_ptr<IReportData> data;
};