#include "mainForm.h"
#include "aboutBoxForm.h"
#include "configurationManager.h"
#include "cityManagementForm.h"
#include "classManagementForm.h"
#include "schoolManagementForm.h"
#include "studentManagementForm.h"
#include "testTypeManagementForm.h"
#include "subjectManagementForm.h"
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <qfile.h>
#include <qt5/QtWidgets/qmessagebox.h>

using namespace std;
using namespace boost::property_tree;

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
    connect(ui.action_Cities, SIGNAL(triggered()), this, SLOT(action_CitiesManagement_Click()));
    connect(ui.action_TestTypes, SIGNAL(triggered()), this, SLOT(action_TestTypesManagement_Click()));
    connect(ui.action_Subjects, SIGNAL(triggered()), this, SLOT(action_SubjectsManagement_Click()));
    connect(ui.action_About, SIGNAL(triggered()), this, SLOT(action_About_Click()));
    connect(ui.action_LightTheme, SIGNAL(triggered()), this, SLOT(action_LightTheme_Click()));
    connect(ui.action_DarkTheme, SIGNAL(triggered()), this, SLOT(action_DarkTheme_Click()));

	//Check if the configuration file exist
	ConfigurationManager configManager("config.json");
	setAppStylesheet(configManager.getStringValue(ConfigurationManager::THEME_PATH));

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
			}
			catch(runtime_error &err) {
				showErrorMessage("The database cannot be created.",
								 err.what());
				exit(2);
			}
		}
		else {
			exit(0);
		}
	}
  	try {
		dbConnection->open();
	}
	catch(runtime_error &err) {
	   showErrorMessage("Can't open database", err.what());
	   exit(1);
	}
}

MainForm::~MainForm()
{
	delete dbConnection;
}
	
void MainForm::action_StudentsManagement_Click()
{
	StudentManagementForm formStudentManagement(this, *dbConnection);
	formStudentManagement.exec();
}

void MainForm::action_SchoolsManagement_Click()
{
	SchoolManagementForm formSchoolManagement(this, *dbConnection);
	formSchoolManagement.exec();
}

void MainForm::action_ClassesManagement_Click()
{
	ClassManagementForm formClassManagement(this, *dbConnection);
	formClassManagement.exec();
}

void MainForm::action_CitiesManagement_Click()
{
	CityManagementForm formCityManagement(this, *dbConnection);
	formCityManagement.exec();
}

void MainForm::action_TestTypesManagement_Click() 
{
	TestTypeManagementForm formTestTypeManagement(this, *dbConnection);
	formTestTypeManagement.exec();
}

void MainForm::action_SubjectsManagement_Click() 
{
	SubjectManagementForm formSubjectManagement(this, *dbConnection);
	formSubjectManagement.exec();
}

void MainForm::action_About_Click()
{
	AboutBoxForm aboutBoxForm(this);
	aboutBoxForm.exec();
}

void MainForm::action_LightTheme_Click()
{
	ConfigurationManager configManager("config.json");
	configManager.setStringValue(ConfigurationManager::THEME_PATH, "");
	setAppStylesheet(configManager.getStringValue(ConfigurationManager::THEME_PATH));
	if (!configManager.save()) {
		showErrorMessage("An error occurred while saving the configuration file.", 
						 configManager.getLastError());
	}
}

void MainForm::action_DarkTheme_Click()
{
	ConfigurationManager configManager("config.json");
	configManager.setStringValue(ConfigurationManager::THEME_PATH, "Dark");
	setAppStylesheet(configManager.getStringValue(ConfigurationManager::THEME_PATH));
	if (!configManager.save()) {
		showErrorMessage("An error occurred while saving the configuration file.", 
						 configManager.getLastError());
	}
}

void MainForm::showErrorMessage(const string &message,
								const string &internalError) const
{
	QMessageBox msgBox;
	msgBox.setText(message.c_str());
	if (internalError.length() > 0) {
		msgBox.setInformativeText(internalError.c_str());
	}
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setWindowTitle("Error");
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

void MainForm::setAppStylesheet(const std::string &style) 
{
	/*
	The Dark theme comes has been built by Colin Duquesnoy 
	Github page : https://github.com/ColinDuquesnoy
	Project page : https://github.com/ColinDuquesnoy/QDarkStyleSheet
	*/
	ui.action_LightTheme->setChecked(false);
	ui.action_DarkTheme->setChecked(false);
	if (style == "Dark") {
		QFile file("res/darkstyle/darkstyle.qss");
		file.open(QFile::ReadOnly);
		const QString styleSheet = QLatin1String(file.readAll());
		this->setStyleSheet(styleSheet);
		ui.action_DarkTheme->setChecked(true);
	}
	else {
		this->setStyleSheet("");
		ui.action_LightTheme->setChecked(true);
	}
}
