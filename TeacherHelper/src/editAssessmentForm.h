
#ifndef EDITASSESSMENTFORM_H
#define EDITASSESSMENTFORM_H

#include "databaseConnection.h"
#include "classController.h"
#include "assessmentController.h"
#include "schoolController.h"
#include "subjectController.h"
#include "testTypeController.h"
#include "qTableWidgetKeyPressWatcher.h"
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
				 const EditAssessmentActionMode editMode,
				 const Assessment * const assessmentToEdit = nullptr);
	~EditAssessmentForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::editAssessmentFormClass ui;
	EditAssessmentActionMode editMode;
	AssessmentController controller;
	TestTypeController testTypeController;
	SubjectController subjectController;
	SchoolController schoolController;
	ClassController classController;
	const Assessment * const assessmentToEdit;
	QTableWidgetKeyPressWatcher tableWidgetResultsKeyWatcher;
	void prepareFormWithEditingValues();
	bool selectItemInComboBox(QComboBox *comboBox, size_t id);
	void saveNewItem();
	void updateExistingItem();
	bool validateEntry() const;
	Assessment getAssessmentFromFields() const;
	std::string getStudentNameFromTableLine(QAbstractItemModel * const model, int rowIndex) const;
	void refreshTestTypeComboBox();
	void refreshSubjectComboBox();
	void refreshSchoolComboBox();
	void refreshClassComboBox(const School &school);
	void refreshStudentList(const Class &aClass);
	void showError(const std::string &message) const;
private slots:
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void comboBoxSchool_CurrentIndexChanged();
	void comboBoxClass_CurrentIndexChanged();
	void tableWidgetResults_keyPressEvent(int key, int row, int column);
};

#endif // EDITASSESSMENTFORM_H
