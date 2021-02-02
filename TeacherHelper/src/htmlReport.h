#pragma once

#include "IReportData.h"
#include <map>
#include <memory>
#include <QWebView>
#include <string>
#include <vector>

class HTMLReport
{
public:
    HTMLReport(const std::string &reportFileNam, QWebView *webView);
    const std::string &getReportFileName() const;
    const std::string &getLastError() const;
    void setData(const std::vector<std::shared_ptr<IReportData>> &data);
    void setProperties(const std::map<std::string, std::string> &properties);
    bool previewReport();
private:
    std::string fileName;
    QWebView *webView;
    std::string renderedReportFileName;
    std::string reportContent;
    std::string lastError;
    std::map<std::string, std::string> properties;
    std::vector<std::shared_ptr<IReportData>> data;
    bool createTemporaryReportFile();
    bool readReportContent();
    bool saveTemporaryReportFile();
};