
#ifndef CLASSASSESSMENTSSUMMARYREPORTFORM_H
#define CLASSASSESSMENTSSUMMARYREPORTFORM_H

#include "IDatabaseController.h"
#include "assessmentController.h"
#include "classController.h"
#include "htmlReport.h"
#include "IReportData.h"
#include "schoolController.h"
#include "ui_classAssessmentsSummaryReportForm.h"
#include <memory>
#include <QWebView>

class ClassAssessmentsSummaryReport : public QDialog
{
Q_OBJECT

public:
	ClassAssessmentsSummaryReport(QWidget *parent, 
								  const IDatabaseController &databaseController, 
								  const std::string &resourcesPath);
	~ClassAssessmentsSummaryReport();
	void showEvent(QShowEvent *event) override;
private:
	Ui::classAssessmentsSummaryReportFormClass ui;
    const IDatabaseController &databaseController;
	SchoolController schoolController;
	ClassController classController;
	AssessmentController assessmentController;
	std::string resourcesPath;
	std::string renderedReportFileName;
	QWebView *webView;
	std::shared_ptr<IReportData> reportData;
	void showError(const std::string &message) const;
	void refreshSchoolComboBox();
	void refreshClassComboBox(const School &school);
	void refreshAssessmentTable(const Class &itemClass);
	QTableWidgetItem *createNonEditableRow(const std::string &value);
	QTableWidgetItem *createEditableRow(const std::string &value);
	void pushButtonShowReport_Clicked();
	void comboBoxSchool_CurrentIndexChanged();
	void comboBoxClass_CurrentIndexChanged();
	void checkBoxIdenticalWeighting_Changed(int state);
	void tableWidgetAssessments_selectionChanged();
	void toggleWeightingCellsEditMode(bool enabled);
	void calculateAutomaticWeighting();
	void generateReport(HTMLReport &report);
	void createTemporaryReportFile(HTMLReport &report);
	std::string readTemporaryReportFile();
	void saveTemporaryReportFile(const std::string &content);
};

#endif // CLASSASSESSMENTSSUMMARYREPORTFORM_H
