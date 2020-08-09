
#ifndef EDITASSESSMENTFORM_H
#define EDITASSESSMENTFORM_H

#include "databaseConnection.h"
#include "classController.h"
#include "schoolController.h"
#include "subjectController.h"
#include "testTypeController.h"
#include "ui_editAssessmentForm.h"

enum class EditAssessmentActionMode {
	Add,
	Modify
};

class EditAssessmentForm : public QDialog
{
Q_OBJECT

public:
	EditAssessmentForm(QWidget *parent, 
				 const DatabaseConnection &connection, 
				 const EditAssessmentActionMode editMode);
	~EditAssessmentForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::editAssessmentFormClass ui;
	EditAssessmentActionMode editMode;
	TestTypeController testTypeController;
	SubjectController subjectController;
	SchoolController schoolController;
	ClassController classController;
	void saveNewItem();
	void updateExistingItem();
	bool validateEntry() const;
	void refreshTestTypeComboBox();
	void refreshSubjectComboBox();
	void refreshSchoolComboBox();
	void refreshClassComboBox(const School &school);
	void showError(const std::string &message) const;
private slots:
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void comboBoxSchool_CurrentIndexChanged(int index);
};

#endif // EDITASSESSMENTFORM_H
