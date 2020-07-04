#include "mainForm.h"
#include "schoolManagementForm.h"
#include "studentManagementForm.h"
#include "classManagementForm.h"
#include "studentStorage.h"
#include <boost/filesystem.hpp>
#include <qt5/QtWidgets/qmessagebox.h>

using namespace std;

MainForm::MainForm(QWidget *parent)
	: QMainWindow(parent),
	  ui(Ui::MainForm()),
	  dbConnection(new DatabaseConnection("teacherdb"))
	  
{
	ui.setupUi(this);
	this->showMaximized();
    connect(ui.action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.action_Students, SIGNAL(triggered()), this, SLOT(action_StudentsManagement_Click()));
    connect(ui.action_Schools, SIGNAL(triggered()), this, SLOT(action_SchoolsManagement_Click()));
    connect(ui.action_Classes, SIGNAL(triggered()), this, SLOT(action_ClassesManagement_Click()));

	//Check if the database exist. If not, ask for creation.
	if (!boost::filesystem::exists("teacherdb")) {
		QMessageBox msgBox;
		msgBox.setText("The database doesn't seem to exist.");
		msgBox.setInformativeText("Do you want to create it?");
		msgBox.setWindowTitle("Confirmation");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);

		if (msgBox.exec() == QMessageBox::Yes) {
			try {
				dbConnection->create();
				ui.textEdit->setText("Database created successfully");
			}
			catch(runtime_error &err) {
				ui.textEdit->setText("Can't create database");
			}
		}
		else {
			exit(0);
		}
	}
  	try {
		dbConnection->open();
	   	ui.textEdit->setText("Opened database successfully");
	}
	catch(runtime_error &err) {
	   ui.textEdit->setText("Can't open database");
	}

	action_ClassesManagement_Click();
}

MainForm::~MainForm()
{
	delete dbConnection;
}
	
void MainForm::action_StudentsManagement_Click()
{
	StudentManagementForm formStudentManagement(this);
	formStudentManagement.setDatabaseConnection(*dbConnection);
	formStudentManagement.exec();
	//exit(0);
}

void MainForm::action_SchoolsManagement_Click()
{
	SchoolManagementForm formSchoolManagement(this);
	formSchoolManagement.setDatabaseConnection(*dbConnection);
	formSchoolManagement.exec();
	//exit(0);
}

void MainForm::action_ClassesManagement_Click()
{
	ClassManagementForm formClassManagement(this);
	formClassManagement.setDatabaseConnection(*dbConnection);
	formClassManagement.exec();
	exit(0);
}