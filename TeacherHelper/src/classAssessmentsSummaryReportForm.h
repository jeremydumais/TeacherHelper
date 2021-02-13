
#ifndef CLASSASSESSMENTSSUMMARYREPORTFORM_H
#define CLASSASSESSMENTSSUMMARYREPORTFORM_H

#include "IDatabaseController.h"
#include "assessmentController.h"
#include "classController.h"
#include "htmlReport.h"
#include "IReportData.h"
#include "schoolController.h"
#include "ui_classAssessmentsSummaryReportForm.h"
#include <list>
#include <map>
#include <memory>
#include <QWebView>
#include <unordered_map>

class ClassAssessmentsSummaryReportForm : public QDialog
{
Q_OBJECT

public:
	ClassAssessmentsSummaryReportForm(QWidget *parent, 
								  const IDatabaseController &databaseController, 
								  const std::string &resourcesPath);
	~ClassAssessmentsSummaryReportForm();
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
	bool validate() const;
	bool processPhase1(const std::list<Student> &students, 
					   const int selectedAssessmentCount,
					   std::unordered_map<size_t, std::map<size_t, float>> &userIdAssessmentIdWeighting,
					   std::vector<size_t> &studentWithDifferentWeighting);
	bool processPhase2(const std::list<Student> &students, 
					   std::unordered_map<size_t, std::map<size_t, float>> &userIdAssessmentIdWeighting,
					   std::vector<size_t> &studentWithDifferentWeighting,
					   std::vector<std::shared_ptr<IReportData>> &reportData);
	float getAssessmentWeighting(const QModelIndex &assessmentRow) const;
	const AssessmentResult *getStudentAssessmentResult(const Assessment &assessment, const Student &student) const;
	const Student *findStudent(const size_t id, const std::list<Student> &students) const;
	void comboBoxSchool_CurrentIndexChanged();
	void comboBoxClass_CurrentIndexChanged();
	void checkBoxIdenticalWeighting_Changed(int state);
	void tableWidgetAssessments_selectionChanged();
	void toggleWeightingCellsEditMode(bool enabled);
	void calculateAutomaticWeighting();
};

#endif // CLASSASSESSMENTSSUMMARYREPORTFORM_H
