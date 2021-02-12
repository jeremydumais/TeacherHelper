
#ifndef CLASSASSESSMENTSSUMMARYREPORTSTUDENTPRECISIONFORM_H
#define CLASSASSESSMENTSSUMMARYREPORTSTUDENTPRECISIONFORM_H

#include "assessment.h"
#include "ui_classAssessmentsSummaryReportStudentPrecisionForm.h"
#include <map>
#include <vector>

class ClassAssessmentsSummaryReportStudentPrecisionForm : public QDialog
{
Q_OBJECT

public:
	ClassAssessmentsSummaryReportStudentPrecisionForm(QWidget *parent, 
													  const std::string &studentName,
								  					  const std::vector<const Assessment *> &assessments);
	void showEvent(QShowEvent *event) override;
	std::map<size_t, float> getWeightingResult() const;
private:
	Ui::classAssessmentsSummaryReportStudentPrecisionFormClass ui;
	const std::string &studentName;
	const std::vector<const Assessment *> &assessments;
	std::map<size_t, float> result;
	void showError(const std::string &message) const;
	void refreshAssessmentTable();
	QTableWidgetItem *createNonEditableRow(const std::string &value);
	QTableWidgetItem *createEditableRow(const std::string &value);
	void pushButtonOK_Clicked();
	bool validate() const;
	float getAssessmentWeighting(const QModelIndex &assessmentRow) const;
	const AssessmentResult *getStudentAssessmentResult(const Assessment &assessment, const Student &student) const;
	void checkBoxIdenticalWeighting_Changed(int state);
	void tableWidgetAssessments_selectionChanged();
	void toggleWeightingCellsEditMode(bool enabled);
	void calculateAutomaticWeighting();
};

#endif // CLASSASSESSMENTSSUMMARYREPORTSTUDENTPRECISIONFORM_H
