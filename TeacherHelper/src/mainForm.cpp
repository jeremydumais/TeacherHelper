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
	connect(ui.action_Quit, &QAction::triggered, this, &MainForm::close);
	connect(ui.action_AddAssessment, &QAction::triggered, this, &MainForm::action_AddAssessment_Click);
	connect(ui.action_EditAssessment, &QAction::triggered, this, &MainForm::action_EditAssessment_Click);
	connect(ui.action_RemoveAssessment, &QAction::triggered, this, &MainForm::action_RemoveAssessment_Click);
	connect(ui.action_Students, &QAction::triggered, this, &MainForm::action_StudentsManagement_Click);
	connect(ui.action_Schools, &QAction::triggered, this, &MainForm::action_SchoolsManagement_Click);
	connect(ui.action_Classes, &QAction::triggered, this, &MainForm::action_ClassesManagement_Click);
	connect(ui.action_Cities, &QAction::triggered, this, &MainForm::action_CitiesManagement_Click);
	connect(ui.action_TestTypes, &QAction::triggered, this, &MainForm::action_TestTypesManagement_Click);
	connect(ui.action_Subjects, &QAction::triggered, this, &MainForm::action_SubjectsManagement_Click);
	connect(ui.action_About, &QAction::triggered, this, &MainForm::action_About_Click);
	connect(ui.action_LightTheme, &QAction::triggered, this, &MainForm::action_LightTheme_Click);
	connect(ui.action_DarkTheme, &QAction::triggered, this, &MainForm::action_DarkTheme_Click);
	connect(ui.toolButtonExpandAll, &QToolButton::clicked, this, &MainForm::toolButtonExpandAll_Click);
	connect(ui.toolButtonCollapseAll, &QToolButton::clicked, this, &MainForm::toolButtonCollapseAll_Click);
	connect(ui.treeWidgetSchoolClassNav, &QTreeWidget::currentItemChanged, this, &MainForm::treeWidgetSchoolClassNav_currentItemChanged);
	connect(ui.tableWidgetAssessments->selectionModel(), 
		&QItemSelectionModel::selectionChanged, 
		this,
  		&MainForm::tableWidgetAssessments_selectionChanged);
	connect(ui.tableWidgetAssessments, 
		&QTableWidget::itemDoubleClicked,
		this,
		&MainForm::tableWidgetAssessments_itemDoubleClicked);
	tableWidgetAssessmentsKeyWatcher.installOn(ui.tableWidgetAssessments);
	connect(&tableWidgetAssessmentsKeyWatcher, &QTableWidgetKeyPressWatcher::keyPressed, this, &MainForm::tableWidgetAssessments_keyPressEvent);

	ui.tableWidgetAssessments->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(1, new QTableWidgetItem("Date"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(2, new QTableWidgetItem("Name"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(3, new QTableWidgetItem("Test type"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(4, new QTableWidgetItem("Subject"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(5, new QTableWidgetItem("Nb of results"));
	ui.tableWidgetAssessments->setColumnHidden(0, true);

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

	//Get database version
	

	loadControllers();
	refreshTreeViewTestNavigation();
	toggleAssessmentActionsButtons(false);
}

MainForm::~MainForm()
{
	delete dbConnection;
}

void MainForm::functionAfterShown()
{
	setWindowIcon(QIcon(":/global/TeacherHelper Icon256.png"));
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
		refreshTreeViewTestNavigation();
	}
}

void MainForm::action_EditAssessment_Click() 
{
	//Get the selected assessment
	auto selectedAssessmentRow = ui.tableWidgetAssessments->selectionModel()->selectedIndexes();
	if (!selectedAssessmentRow.empty()) {
		//Find selected assessment
		auto assessmentPtr = assessmentController->findAssessment(selectedAssessmentRow[0].data().toUInt());
		if (assessmentPtr != nullptr) {
			EditAssessmentForm formEditAssessment(this, *dbConnection, 
												EditAssessmentActionMode::Modify, 
												assessmentPtr);
			if (formEditAssessment.exec() == QDialog::DialogCode::Accepted) {
				//Refresh the navigation and test list
				size_t selectedClassId = assessmentPtr->getClass().getId();
				refreshTreeViewTestNavigation();
				//Reposition to previously selected class
				reselectTreeViewTestNavigationItem(selectedClassId);
			}
		}
	}
}

void MainForm::action_RemoveAssessment_Click() 
{
	//Get the selected assessment
	auto selectedAssessmentRow = ui.tableWidgetAssessments->selectionModel()->selectedIndexes();
	if (!selectedAssessmentRow.empty()) {
		//Find selected assessment
		auto assessmentPtr = assessmentController->findAssessment(selectedAssessmentRow[0].data().toUInt());
		if (assessmentPtr != nullptr) {
			QMessageBox msgBox;
			msgBox.setText(fmt::format("Are you sure you want to delete the assessment {0}?", assessmentPtr->getName()).c_str());
			msgBox.setWindowTitle("Confirmation");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			if (msgBox.exec() == QMessageBox::Yes) {
				if (!assessmentController->deleteAssessment(assessmentPtr->getId())) {
					showErrorMessage("Unable to remove the assessment. ", assessmentController->getLastError());
					return;
				}
				//Refresh the navigation and test list
				size_t selectedClassId = assessmentPtr->getClass().getId();
				refreshTreeViewTestNavigation();
				//Reposition to previously selected class
				reselectTreeViewTestNavigationItem(selectedClassId);
			}
		}
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
	assessmentController = make_unique<AssessmentController>(*dbConnection);
	if (!assessmentController) {
		showErrorMessage("Can't create the assessment controller", "");
	   	exit(1);
	}

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
	ui.tableWidgetAssessments->model()->removeRows(0, ui.tableWidgetAssessments->rowCount());
}

void MainForm::reselectTreeViewTestNavigationItem(size_t classId) 
{
	QTreeWidgetItemIterator iter(ui.treeWidgetSchoolClassNav);
	while(*iter)
	{
		if ((*iter) && (*iter)->parent() != nullptr && (*iter)->data(1, 0).toUInt() == classId) {
			(*iter)->setSelected(true);
			(*iter)->parent()->setExpanded(true);
			treeWidgetSchoolClassNav_currentItemChanged((*iter));
		}
		iter++;
	}
}

void MainForm::toolButtonExpandAll_Click() 
{
	ui.treeWidgetSchoolClassNav->expandAll();
}

void MainForm::toolButtonCollapseAll_Click() 
{
	ui.treeWidgetSchoolClassNav->collapseAll();
}

void MainForm::treeWidgetSchoolClassNav_currentItemChanged(QTreeWidgetItem *current) 
{
	ui.tableWidgetAssessments->model()->removeRows(0, ui.tableWidgetAssessments->rowCount());
	toggleAssessmentActionsButtons(false);

	if (current != nullptr && current->parent() != nullptr) {
		//Retreive selected class id
		size_t classId = current->data(1, 0).toUInt();
		//Load the class assessments
		assessmentController->loadAssessmentsByClass(classId);
		size_t row {0};
		for(const auto &assessment : assessmentController->getAssessments()) {
			ui.tableWidgetAssessments->insertRow(row);
			ui.tableWidgetAssessments->setItem(row, 0, new QTableWidgetItem(to_string(assessment.getId()).c_str()));
			ui.tableWidgetAssessments->setItem(row, 1, new QTableWidgetItem(to_simple_string(assessment.getDateTime()).c_str()));
			ui.tableWidgetAssessments->setItem(row, 2, new QTableWidgetItem(assessment.getName().c_str()));
			ui.tableWidgetAssessments->setItem(row, 3, new QTableWidgetItem(assessment.getTestType().getName().c_str()));
			ui.tableWidgetAssessments->setItem(row, 4, new QTableWidgetItem(assessment.getSubject().getName().c_str()));
			ui.tableWidgetAssessments->setItem(row, 5, new QTableWidgetItem((to_string(assessment.getResults().size())).c_str()));
			row++;
		}
	}
}

void MainForm::tableWidgetAssessments_selectionChanged(const QItemSelection &selected) 
{
	toggleAssessmentActionsButtons(selected.size() == 1);
}

void MainForm::toggleAssessmentActionsButtons(bool isAssessmentSelected) 
{
	ui.action_EditAssessment->setEnabled(isAssessmentSelected);
	ui.action_RemoveAssessment->setEnabled(isAssessmentSelected);
}

void MainForm::tableWidgetAssessments_itemDoubleClicked(QTableWidgetItem *item) 
{
	if (item) {
		action_EditAssessment_Click();
	}
}

void MainForm::tableWidgetAssessments_keyPressEvent(int key, int, int) 
{
	if (key == Qt::Key_Delete) {
		action_RemoveAssessment_Click();
	}
}
