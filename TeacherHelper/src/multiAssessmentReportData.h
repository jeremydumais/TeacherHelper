#pragma once

#include "htmlReport.h"
#include <boost/optional.hpp>
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <map>
#include <string>

class MultiAssessmentReportData : public IReportData
{
public:
    MultiAssessmentReportData(const std::string &studentFirstName,
                              const std::string &studentLastName,
                              boost::optional<float> result) 
        : studentFirstName(studentFirstName),
          studentLastName(studentLastName),
          result(result)
    {
    } 

    std::string generateDataRow() const override {
        return fmt::format("<tr><td>{0}</td><td>{1}</td><td>{2}{3}</td></tr>",
                           studentLastName,
                           studentFirstName,
                           result.has_value() ? std::to_string(result.get()) : "N/A",
                           result.has_value() ? "%" : "");
    };
private:
    std::string studentFirstName;
    std::string studentLastName;
    boost::optional<float> result;
};