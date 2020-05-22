#include "mainForm.h"
#include "schoolManagementForm.h"
#include "studentManagementForm.h"
#include "studentStorage.h"

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

  	try {
		dbConnection->open();
	   	ui.textEdit->setText("Opened database successfully");
	}
	catch(runtime_error &err) {
	   ui.textEdit->setText("Can't open database");
	}

	//action_SchoolsManagement_Click();
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