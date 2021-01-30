#pragma once

#include "IReportData.h"
#include <string>

struct MultiAssessmentReportData : IReportData
{
    
    std::string IReportData::generateDataRow() override {
    }
}