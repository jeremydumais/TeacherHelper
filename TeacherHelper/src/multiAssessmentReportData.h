#pragma once

#include "htmlReport.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <map>
#include <string>

class MultiAssessmentReportData : public IReportData
{
public:
    MultiAssessmentReportData(const std::string &studentFirstName,
                              const std::string &studentLastName) 
        : studentFirstName(studentFirstName),
          studentLastName(studentLastName)
    {
    } 

    /*void generateHeader(const std::map<std::string, std::string> &properties) override {
        for(const auto &property : properties) {
            boost::replace_all(content, property.first, property.second);
        }
    };*/

    std::string generateDataRow() const override {
        return fmt::format("<tr><td>{0}</td><td>{1}</td><td></td></tr>",
                           studentLastName,
                           studentFirstName);
    };
private:
    std::string studentFirstName;
    std::string studentLastName;
};