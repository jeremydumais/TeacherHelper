#ifndef MAINFORM_H
#define MAINFORM_H

#include "databaseConnection.h"
#include "ui_mainForm.h"
#include "schoolController.h"
#include "classController.h"
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
	DatabaseConnection *dbConnection;
	std::unique_ptr<SchoolController> schoolController;
	std::unique_ptr<ClassController> classController;
	std::string userConfigFolder;
	bool functionAfterShownCalled;
	void showErrorMessage(const std::string &message,
						  const std::string &internalError) const;
	void setAppStylesheet(const std::string &style);
	void loadControllers();
	void refreshTreeViewTestNavigation();
private slots:
	void action_AddAssessment_Click();
	void action_StudentsManagement_Click();
	void action_SchoolsManagement_Click();
	void action_ClassesManagement_Click();
	void action_CitiesManagement_Click();
	void action_TestTypesManagement_Click();
	void action_SubjectsManagement_Click();
	void action_About_Click();
	void action_LightTheme_Click();
	void action_DarkTheme_Click();
	void toolButtonExpandAll_Click();
	void toolButtonCollapseAll_Click();
};

#endif // MAINFORM_H

