#include "mainForm.h"
#include "aboutBoxForm.h"
#include "configurationManager.h"
#include "specialFolders.h"
#include "editAssessmentForm.h"
#include "cityManagementForm.h"
#include "classManagementForm.h"
#include "schoolManagementForm.h"
#include "studentManagementForm.h"
#include "testTypeManagementForm.h"
#include "subjectManagementForm.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <fmt/format.h>
#include <QtCore/qfile.h>
#include <QtWidgets/qmessagebox.h>
#include <qtimer.h>
#ifdef _WIN32
	#include <windows.h>
#endif

using namespace std;
using namespace boost::property_tree;

MainForm::MainForm(QWidget *parent)
	: QMainWindow(parent),
	  ui(Ui::MainForm()),
	  dbConnection(nullptr),
	  functionAfterShownCalled(false)
{
	ui.setupUi(this);
	this->showMaximized();
    connect(ui.action_Quit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui.action_AddAssessment, SIGNAL(triggered()), this, SLOT(action_AddAssessment_Click()));
    connect(ui.action_Students, SIGNAL(triggered()), this, SLOT(action_StudentsManagement_Click()));
    connect(ui.action_Schools, SIGNAL(triggered()), this, SLOT(action_SchoolsManagement_Click()));
    connect(ui.action_Classes, SIGNAL(triggered()), this, SLOT(action_ClassesManagement_Click()));
    connect(ui.action_Cities, SIGNAL(triggered()), this, SLOT(action_CitiesManagement_Click()));
    connect(ui.action_TestTypes, SIGNAL(triggered()), this, SLOT(action_TestTypesManagement_Click()));
    connect(ui.action_Subjects, SIGNAL(triggered()), this, SLOT(action_SubjectsManagement_Click()));
    connect(ui.action_About, SIGNAL(triggered()), this, SLOT(action_About_Click()));
    connect(ui.action_LightTheme, SIGNAL(triggered()), this, SLOT(action_LightTheme_Click()));
    connect(ui.action_DarkTheme, SIGNAL(triggered()), this, SLOT(action_DarkTheme_Click()));
    connect(ui.toolButtonExpandAll, SIGNAL(clicked()), this, SLOT(toolButtonExpandAll_Click()));
    connect(ui.toolButtonCollapseAll, SIGNAL(clicked()), this, SLOT(toolButtonCollapseAll_Click()));

	//Check if the user configuration folder exist
	userConfigFolder = SpecialFolders::getUserConfigDirectory();
	if (!boost::filesystem::exists(userConfigFolder)) {
		if (!boost::filesystem::create_directory(userConfigFolder)) {
			showErrorMessage(fmt::format("Unable to create the folder {0}", userConfigFolder), "");
			exit(1);
		}
	}

	//Check if the configuration file exist
	ConfigurationManager configManager(userConfigFolder + "config.json");
	setAppStylesheet(configManager.getStringValue(ConfigurationManager::THEME_PATH));

	//Check if the database exist. If not, ask for creation.
	dbConnection = new DatabaseConnection(userConfigFolder + "teacherdb");
	if (!boost::filesystem::exists(userConfigFolder + "teacherdb")) {
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

	loadControllers();
	refreshTreeViewTestNavigation();
}

MainForm::~MainForm()
{
	delete dbConnection;
}

void MainForm::functionAfterShown()
{
	const int WINDOWWIDTH {this->size().width()};
	ui.splitter->setSizes({static_cast<int>(WINDOWWIDTH*0.2f), static_cast<int>(WINDOWWIDTH*0.8f)});
}

bool MainForm::event(QEvent *event)
{
    const bool ret_val = QMainWindow::event(event);
    if(!functionAfterShownCalled && event->type() == QEvent::Paint)
    {
        functionAfterShownCalled = true;
        functionAfterShown();
    }
    return ret_val;
}

void MainForm::action_AddAssessment_Click() 
{
	EditAssessmentForm formEditAssessment(this, *dbConnection, EditAssessmentActionMode::Add);
	if (formEditAssessment.exec() == QDialog::DialogCode::Accepted) {
		//Refresh the navigation and test list
	}
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
	if (formSchoolManagement.getDataHasChanged()) {
		schoolController->loadSchools();
		classController->loadClasses();
		refreshTreeViewTestNavigation();
	}
}

void MainForm::action_ClassesManagement_Click()
{
	ClassManagementForm formClassManagement(this, *dbConnection);
	formClassManagement.exec();
	if (formClassManagement.getDataHasChanged()) {
		schoolController->loadSchools();
		classController->loadClasses();
		refreshTreeViewTestNavigation();
	}
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
	ConfigurationManager configManager(userConfigFolder + "config.json");
	configManager.setStringValue(ConfigurationManager::THEME_PATH, "");
	setAppStylesheet(configManager.getStringValue(ConfigurationManager::THEME_PATH));
	if (!configManager.save()) {
		showErrorMessage("An error occurred while saving the configuration file.", 
						 configManager.getLastError());
	}
}

void MainForm::action_DarkTheme_Click()
{
	ConfigurationManager configManager(userConfigFolder + "config.json");
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
		#ifdef _WIN32
			char exePath[MAX_PATH]; 
			// When NULL is passed to GetModuleHandle, the handle of the exe itself is returned
			HMODULE hModule = GetModuleHandle(nullptr);
			if (hModule != nullptr)
			{
				// Use GetModuleFileName() with module handle to get the path
				GetModuleFileName(hModule, exePath, (sizeof(exePath))); 
			}
			boost::filesystem::path path(exePath);
			string stylePath { fmt::format("{0}\\res\\", path.parent_path().string())};
		#else
			string stylePath = "/usr/local/share/TeacherHelperApp/res/";
		#endif
		QFile file(fmt::format("{0}darkstyle.qss", stylePath).c_str());
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

void MainForm::loadControllers() 
{
	schoolController = make_unique<SchoolController>(*dbConnection);
	if (schoolController) {
		schoolController->loadSchools();
	}
	else {
		showErrorMessage("Can't create the School controller", "");
	   	exit(1);
	}

	classController = make_unique<ClassController>(*dbConnection);
	if (classController) {
		classController->loadClasses();
	}
	else {
		showErrorMessage("Can't create the Class controller", "");
	   	exit(1);
	}
}

void MainForm::refreshTreeViewTestNavigation() 
{
	ui.treeWidgetSchoolClassNav->clear();
	QList<QTreeWidgetItem *> items;
	for (const auto &itemSchool : schoolController->getSchools()) {
		auto newSchoolItem = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("%1").arg(itemSchool.getName().c_str())));
		auto classesFiltered = classController->getClassesBySchool(itemSchool);
		for (const auto &itemClass : classesFiltered) {
			auto newClassItem = new QTreeWidgetItem(static_cast<QTreeWidget *>(nullptr), QStringList(QString("%1").arg(itemClass.getName().c_str())));
			QVariant idVariant;
			idVariant.setValue<size_t>(itemClass.getId());
			newClassItem->setData(1, 0, idVariant);
			newSchoolItem->addChild(newClassItem);
		}
		items.append(newSchoolItem);

	}
	ui.treeWidgetSchoolClassNav->setColumnHidden(1, true);
	ui.treeWidgetSchoolClassNav->insertTopLevelItems(0, items);
	ui.tableWidgetAssessments->clear();
}

void MainForm::toolButtonExpandAll_Click() 
{
	ui.treeWidgetSchoolClassNav->expandAll();
}

void MainForm::toolButtonCollapseAll_Click() 
{
	ui.treeWidgetSchoolClassNav->collapseAll();
}
