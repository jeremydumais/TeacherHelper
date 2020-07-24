#ifndef MAINFORM_H
#define MAINFORM_H

#include "databaseConnection.h"
#include "ui_mainForm.h"

class MainForm : public QMainWindow
{
Q_OBJECT

public:
	explicit MainForm(QWidget *parent = 0);
	~MainForm();
private:
	Ui::MainForm ui;
	DatabaseConnection *dbConnection;
	void showErrorMessage(const std::string &message,
						  const std::string &internalError) const;
	void setAppStylesheet(const std::string &style);
private slots:
	void action_StudentsManagement_Click();
	void action_SchoolsManagement_Click();
	void action_ClassesManagement_Click();
	void action_CitiesManagement_Click();
	void action_TestTypesManagement_Click();
	void action_SubjectsManagement_Click();
	void action_About_Click();
	void action_LightTheme_Click();
	void action_DarkTheme_Click();
};

#endif // MAINFORM_H

