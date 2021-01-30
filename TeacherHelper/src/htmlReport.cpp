#include "htmlReport.h"

using namespace std;

HTMLReport::HTMLReport(const string &reportFileName) 
    : fileName(reportFileName)
{
}

void HTMLReport::setData(std::shared_ptr<IReportData> data) 
{
    this->data = data;
}

void HTMLReport::setProperties(const std::map<std::string, std::string> &properties) 
{
    
}

void HTMLReport::previewReport() 
{
    
}
