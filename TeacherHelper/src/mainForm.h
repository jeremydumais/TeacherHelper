#ifndef MAINFORM_H
#define MAINFORM_H

#include "assessmentController.h"
#include "classController.h"
#include "IDatabaseController.h"
#include "qTableWidgetKeyPressWatcher.h"
#include "schoolController.h"
#include "ui_mainForm.h"
#include "upgradeProgressForm.h"
#include <memory>

class MainForm : public QMainWindow
{
Q_OBJECT

public:
	explicit MainForm(QWidget *parent = 0);
	~MainForm();
	bool event(QEvent *event) override;
    void functionAfterShown();
private:
	Ui::MainForm ui;
	std::unique_ptr<IDatabaseController> databaseController;
	std::unique_ptr<AssessmentController> assessmentController;
	std::unique_ptr<SchoolController> schoolController;
	std::unique_ptr<ClassController> classController;
	std::string userConfigFolder;
	bool functionAfterShownCalled;
	std::string resourcesPath;
	QTableWidgetKeyPressWatcher tableWidgetAssessmentsKeyWatcher;
	std::unique_ptr<UpgradeProgressForm> upgradeProgressForm;
	void fetchResourcesPath();
	void showErrorMessage(const std::string &message,
						  const std::string &internalError = "") const;
	void setAppStylesheet(const std::string &style);
	void loadControllers();
	void refreshTreeViewTestNavigation();
	void reselectTreeViewTestNavigationItem(size_t classId);
	void toggleAssessmentActionsButtons(bool isAssessmentSelected);
	void connectUIActions();
	void prepareListsHeaders();
private slots:
	void action_AddAssessment_Click();
	void action_EditAssessment_Click();
	void action_RemoveAssessment_Click();
	void action_StudentsManagement_Click();
	void action_SchoolsManagement_Click();
	void action_ClassesManagement_Click();
	void action_CitiesManagement_Click();
	void action_TestTypesManagement_Click();
	void action_SubjectsManagement_Click();
	void action_About_Click();
	void action_LightTheme_Click();
	void action_DarkTheme_Click();
	void action_ClassAssessmentsSummaryReport_Click();
	void toolButtonExpandAll_Click();
	void toolButtonCollapseAll_Click();
	void treeWidgetSchoolClassNav_currentItemChanged(QTreeWidgetItem *current);
	void tableWidgetAssessments_selectionChanged(const QItemSelection &selected);
    void tableWidgetAssessments_itemDoubleClicked(QTableWidgetItem *item);
	void tableWidgetAssessments_keyPressEvent(int key, int, int);
};

#endif // MAINFORM_H

