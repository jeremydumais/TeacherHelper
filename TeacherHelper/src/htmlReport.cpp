#include "htmlReport.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <fstream>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <sstream>

using namespace std;

HTMLReport::HTMLReport(const string &reportFileName, QWebView *webView) 
    : fileName(reportFileName),
      webView(webView),
      renderedReportFileName(""),
      reportContent(""),
      lastError("")
{
}

const std::string& HTMLReport::getReportFileName() const
{
    return fileName;
}

const std::string& HTMLReport::getLastError() const
{
    return lastError;
}

void HTMLReport::setData(const std::vector<std::shared_ptr<IReportData>> &data) 
{
    this->data = data;
}

void HTMLReport::setProperties(const std::map<std::string, std::string> &properties) 
{
    this->properties = properties;
}

bool HTMLReport::previewReport(bool ascendingOrder) 
{
    if (!createTemporaryReportFile()) {
        return false;
    }
    if (!readReportContent()) {
        return false;
    }
    //Apply report header properties
    for(const auto &property : properties) {
        boost::replace_all(reportContent, property.first, property.second);
    }
    //Apply report data
    if (data.size() > 0) {
        size_t index { 0 };
        for(const auto &dataRow : data) {
            string rowValue { dataRow->generateDataRow() };
            if (index < data.size() -1) {
                if (ascendingOrder) {
                    rowValue += "\n<:DATAROW>";
                }
                else {
                    rowValue = "<:DATAROW>\n" + rowValue;
                }
            }
            boost::replace_first(reportContent, "<:DATAROW>", rowValue);
            index++;
        }
    }
    else {
        boost::replace_first(reportContent, "<:DATAROW>", "No data to display");
    }
    if (!saveTemporaryReportFile()) {
        return false;
    }
	webView->setUrl(QUrl(fmt::format("file://{0}", renderedReportFileName).c_str()));
    return true;
}

bool HTMLReport::createTemporaryReportFile() 
{
	string tempFolder = QDir::tempPath().toStdString();
	QFile reportFile(fileName.c_str());
	QFileInfo reportFileInfo(fileName.c_str());
	renderedReportFileName = fmt::format("{0}/{1}", tempFolder, reportFileInfo.fileName().toStdString());
	//If the file already exist remove it
	QFile renderedReportFile(renderedReportFileName.c_str());
	if (renderedReportFile.exists()) {
		if (!renderedReportFile.remove()) {
            lastError = fmt::format("Unable to remove the temporary file {0}", renderedReportFileName);
            return false;
        }
	}
	if (!reportFile.copy(renderedReportFileName.c_str())) {
		lastError = fmt::format("Unable to create the file {0} from {1}", renderedReportFileName, fileName);
		return false;
	}
    return true;
}
bool HTMLReport::readReportContent() 
{
	ifstream fs;
	std::stringstream buffer;
	fs.open(renderedReportFileName);
    if (fs.is_open()) {
	    buffer << fs.rdbuf();
	    reportContent = buffer.str();
    	fs.close();
    }
    else {
        lastError = fmt::format("Unable to open the file {0}", renderedReportFileName);
        return false;
    }
    return true;
}

bool HTMLReport::saveTemporaryReportFile() 
{
	ofstream ofs;
	ofs.open(renderedReportFileName, ofstream::out | ofstream::trunc);
	if (ofs.is_open()) {
        ofs << reportContent;
	    ofs.close();
    }
    else {
        lastError = fmt::format("Unable to save the file {0}", renderedReportFileName);
        return false;
    }
    return true;
}