
#ifndef CLASSASSESSMENTSSUMMARYREPORTFORM_H
#define CLASSASSESSMENTSSUMMARYREPORTFORM_H

#include "IDatabaseController.h"
#include "assessmentController.h"
#include "classController.h"
#include "schoolController.h"
#include "ui_classAssessmentsSummaryReportForm.h"

class ClassAssessmentsSummaryReport : public QDialog
{
Q_OBJECT

public:
	ClassAssessmentsSummaryReport(QWidget *parent, const IDatabaseController &databaseController);
	~ClassAssessmentsSummaryReport() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::classAssessmentsSummaryReportFormClass ui;
    const IDatabaseController &databaseController;
	SchoolController schoolController;
	ClassController classController;
	AssessmentController assessmentController;
	void refreshSchoolComboBox();
	void refreshClassComboBox(const School &school);
	void refreshAssessmentTable(const Class &itemClass);
	QTableWidgetItem *createNonEditableRow(const std::string &value);
	QTableWidgetItem *createEditableRow(const std::string &value);
	void comboBoxSchool_CurrentIndexChanged();
	void comboBoxClass_CurrentIndexChanged();
};

#endif // CLASSASSESSMENTSSUMMARYREPORTFORM_H
