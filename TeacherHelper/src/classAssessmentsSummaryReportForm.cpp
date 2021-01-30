#include "classAssessmentsSummaryReportForm.h"
#include <fmt/format.h>

using namespace std;

ClassAssessmentsSummaryReport::ClassAssessmentsSummaryReport(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ui(Ui::classAssessmentsSummaryReportFormClass()),
	  databaseController(databaseController),
	  schoolController(databaseController),
	  classController(databaseController),
	  assessmentController(databaseController)
{
	ui.setupUi(this);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &ClassAssessmentsSummaryReport::close);
	connect(ui.comboBoxSchool, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &ClassAssessmentsSummaryReport::comboBoxSchool_CurrentIndexChanged);
	connect(ui.comboBoxClass, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &ClassAssessmentsSummaryReport::comboBoxClass_CurrentIndexChanged);

	ui.tableWidgetAssessments->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(1, new QTableWidgetItem("Weighting %"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(2, new QTableWidgetItem("Date"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(3, new QTableWidgetItem("Name"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(4, new QTableWidgetItem("Test type"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(5, new QTableWidgetItem("Subject"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(6, new QTableWidgetItem("Nb of results"));
	ui.tableWidgetAssessments->setColumnHidden(0, true);
}

void ClassAssessmentsSummaryReport::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/class64.png"));
	schoolController.loadSchools();
	classController.loadClasses();
	refreshSchoolComboBox();
} 

void ClassAssessmentsSummaryReport::refreshSchoolComboBox()
{
	ui.comboBoxSchool->clear();
	ui.comboBoxClass->clear();
	ui.tableWidgetAssessments->model()->removeRows(0, ui.tableWidgetAssessments->rowCount());
	//Add a first empty choice
	ui.comboBoxSchool->addItem("", -1);
	for(const auto &school : schoolController.getSchools()) {
		ui.comboBoxSchool->addItem(fmt::format("{0} ({1})", school.getName(), school.getCity().getName()).c_str(), 
								   static_cast<uint>(school.getId()));
	}
}

void ClassAssessmentsSummaryReport::refreshClassComboBox(const School &school)
{
	ui.comboBoxClass->clear();
	ui.tableWidgetAssessments->model()->removeRows(0, ui.tableWidgetAssessments->rowCount());
	//Add a first empty choice
	ui.comboBoxClass->addItem("", -1);
	for(const auto &itemClass : classController.getClassesBySchool(school)) {
		ui.comboBoxClass->addItem(itemClass.getName().c_str(), 
								static_cast<uint>(itemClass.getId()));
	}
}

void ClassAssessmentsSummaryReport::refreshAssessmentTable(const Class &itemClass) 
{
	ui.tableWidgetAssessments->model()->removeRows(0, ui.tableWidgetAssessments->rowCount());
	//Load the class assessments
	assessmentController.loadAssessmentsByClass(itemClass.getId());
	size_t row {0};
	for(const auto &assessment : assessmentController.getAssessments()) {
		ui.tableWidgetAssessments->insertRow(row);
		ui.tableWidgetAssessments->setItem(row, 0, createNonEditableRow(to_string(assessment.getId())));
		ui.tableWidgetAssessments->setItem(row, 1, createEditableRow(""));
		ui.tableWidgetAssessments->setItem(row, 2, createNonEditableRow(to_simple_string(assessment.getDateTime())));
		ui.tableWidgetAssessments->setItem(row, 3, createNonEditableRow(assessment.getName()));
		ui.tableWidgetAssessments->setItem(row, 4, createNonEditableRow(assessment.getTestType().getName()));
		ui.tableWidgetAssessments->setItem(row, 5, createNonEditableRow(assessment.getSubject().getName()));
		ui.tableWidgetAssessments->setItem(row, 6, createNonEditableRow((to_string(assessment.getResults().size()))));
		row++;
	}
}

QTableWidgetItem* ClassAssessmentsSummaryReport::createNonEditableRow(const std::string &value) 
{
	QTableWidgetItem *item = new QTableWidgetItem(value.c_str());
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	return item;
}

QTableWidgetItem* ClassAssessmentsSummaryReport::createEditableRow(const std::string &value) 
{
	return new QTableWidgetItem(value.c_str());
}

void ClassAssessmentsSummaryReport::comboBoxSchool_CurrentIndexChanged() 
{
	//Find the selected school
	if (ui.comboBoxSchool->currentData().toInt() >= 1) {
		const School* const selectedSchool = schoolController.findSchool(ui.comboBoxSchool->currentData().toUInt());
		if (selectedSchool) {
			refreshClassComboBox(*selectedSchool);
		}
	}
}

void ClassAssessmentsSummaryReport::comboBoxClass_CurrentIndexChanged() 
{
	//Find the selected class
	if (ui.comboBoxClass->currentData().toInt() >= 1) {
		const Class* const selectedClass = classController.findClass(ui.comboBoxClass->currentData().toUInt());
		if (selectedClass) {
			refreshAssessmentTable(*selectedClass);
		}
	}
}