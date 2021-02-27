#pragma once

#include "IReportData.h"
#include <map>
#include <memory>
#include <QWebEngineView>
#include <string>
#include <vector>

class HTMLReport
{
public:
    HTMLReport(const std::string &reportFileNam, QWebEngineView *webView);
    const std::string &getReportFileName() const;
    const std::string &getLastError() const;
    void setData(const std::vector<std::shared_ptr<IReportData>> &data);
    void setProperties(const std::map<std::string, std::string> &properties);
    bool previewReport(bool ascendingOrder = true);
private:
    std::string fileName;
    QWebEngineView *webView;
    std::string renderedReportFileName;
    std::string reportContent;
    std::string lastError;
    std::map<std::string, std::string> properties;
    std::vector<std::shared_ptr<IReportData>> data;
    bool createTemporaryReportFile();
    bool readReportContent();
    bool saveTemporaryReportFile();
};