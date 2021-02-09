#include "classAssessmentsSummaryReportForm.h"
#include "multiAssessmentReportData.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <fstream>
#include <QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <sstream>

using namespace std;

ClassAssessmentsSummaryReport::ClassAssessmentsSummaryReport(QWidget *parent, 
															 const IDatabaseController &databaseController,
															 const std::string &resourcesPath)
	: QDialog(parent),
	  ui(Ui::classAssessmentsSummaryReportFormClass()),
	  databaseController(databaseController),
	  schoolController(databaseController),
	  classController(databaseController),
	  assessmentController(databaseController),
	  resourcesPath(resourcesPath),
	  webView(new QWebView(this))
{
	ui.setupUi(this);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &ClassAssessmentsSummaryReport::close);
	connect(ui.pushButtonShowReport, &QPushButton::clicked, this, &ClassAssessmentsSummaryReport::pushButtonShowReport_Clicked);
	connect(ui.comboBoxSchool, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &ClassAssessmentsSummaryReport::comboBoxSchool_CurrentIndexChanged);
	connect(ui.comboBoxClass, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &ClassAssessmentsSummaryReport::comboBoxClass_CurrentIndexChanged);
	connect(ui.checkBoxIdenticalWeighting, &QCheckBox::stateChanged, this, &ClassAssessmentsSummaryReport::checkBoxIdenticalWeighting_Changed);
	connect(ui.tableWidgetAssessments->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassAssessmentsSummaryReport::tableWidgetAssessments_selectionChanged);
	ui.tableWidgetAssessments->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(1, new QTableWidgetItem("Weighting %"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(2, new QTableWidgetItem("Date"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(3, new QTableWidgetItem("Name"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(4, new QTableWidgetItem("Test type"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(5, new QTableWidgetItem("Subject"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(6, new QTableWidgetItem("Nb of results"));
	ui.tableWidgetAssessments->setColumnHidden(0, true);
}

ClassAssessmentsSummaryReport::~ClassAssessmentsSummaryReport()
{
	delete webView;
}

void ClassAssessmentsSummaryReport::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/class64.png"));
	schoolController.loadSchools();
	classController.loadClasses();
	refreshSchoolComboBox();

	ui.comboBoxSchool->setCurrentIndex(1);
	ui.comboBoxClass->setCurrentIndex(2);
} 

void ClassAssessmentsSummaryReport::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
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
	toggleWeightingCellsEditMode(!ui.checkBoxIdenticalWeighting->isChecked());
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

void ClassAssessmentsSummaryReport::pushButtonShowReport_Clicked() 
{
	int selectedRowsCount { ui.tableWidgetAssessments->selectionModel()->selectedRows().count() };
	if (selectedRowsCount > 0 && validate()) {
		const School* const selectedSchool = schoolController.findSchool(ui.comboBoxSchool->currentData().toUInt());
		const Class* const selectedClass = classController.findClass(ui.comboBoxClass->currentData().toUInt());

		HTMLReport report(fmt::format("{0}/reports/MultiAssessmentSummary.html", resourcesPath), webView);
		string schoolAndClass { fmt::format("{0}, {1}", 
											selectedSchool->getName(),
											selectedClass->getName()) };
		string allAssessments { "" };
		for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
			allAssessments += fmt::format("<li>{0} ({1})</li>", 
										  assessmentRow.sibling(assessmentRow.row(), 3).data().toString().toStdString(),
										  assessmentRow.sibling(assessmentRow.row(), 2).data().toString().toStdString());
		}
		report.setProperties({ { "<:SCHOOLANDCLASS>", schoolAndClass },
							   { "<:ASSESSMENTSINCLUDED>", allAssessments } });
		//Load report data
		vector<shared_ptr<IReportData>> reportData;
		//Load assessments results
		assessmentController.loadAssessmentsByClass(selectedClass->getId());
		//Phase 1: Load all students and check if each of them have a result in the selected assessment
		//If not them let the user choose another weighting
		map<size_t, map<size_t, float>> userIdAssessmentIdWeighting;
		for(const auto &student : selectedClass->getMembers()) {
			int nbOfResultMissing { 0 };
			vector<size_t> assessmentWithoutResult;
			for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
				uint assessmentId { assessmentRow.sibling(assessmentRow.row(), 0).data().toUInt() };
				const auto *assessment { assessmentController.findAssessment(assessmentId) };
				if (assessment != nullptr) {
					const AssessmentResult *result = getStudentAssessmentResult(*assessment, student);
					if (result == nullptr) {
						nbOfResultMissing++;
						assessmentWithoutResult.push_back(assessment->getId());
					}
				}
				else {
					showError(fmt::format("Unable to find the assessment with id {0}", assessmentId));
				}
			}
			//If number of result missing > 0 and < total assessment selected then ask the user to select a different weighting
			if (nbOfResultMissing > 0) {
				if (nbOfResultMissing == selectedRowsCount) {
					//Add the student with no result at all
					userIdAssessmentIdWeighting.emplace(student.getId(), map<size_t, float>());
				}
				else {
					//Ask for different weighting

					//Add the student with is customized weighting
				}
			}
			else {
				//Add the student with is standard weighting
				map<size_t, float> assessmentWeighting;
				for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
					uint assessmentId { assessmentRow.sibling(assessmentRow.row(), 0).data().toUInt() };
					string weightingStr { assessmentRow.sibling(assessmentRow.row(), 1).data().toString().toStdString() };
					replace(weightingStr.begin(), weightingStr.end(), ',', '.');
					float weighting { boost::lexical_cast<float>(weightingStr) };
					assessmentWeighting.emplace(assessmentId, weighting);
				}
				userIdAssessmentIdWeighting.emplace(student.getId(), assessmentWeighting);
			}
		}
		//Phase2 :  and calculate the average

		
		reportData.push_back(make_shared<MultiAssessmentReportData>("Jed", "Dum"));
		report.setData(reportData);
		if (!report.previewReport()) {
			showError(report.getLastError());
			return;
		}
		QCoreApplication::processEvents();
		QPrinter *printer = new QPrinter();
		printer->setPageSize(QPrinter::Letter);
		printer->setPageOrientation(QPageLayout::Portrait);
		printer->setPageMargins(15.00, 10.00, 7.00, 15.00, QPrinter::Millimeter);
		printer->setResolution(QPrinter::HighResolution);
		printer->setFullPage(true);
		QPrintPreviewDialog *preview = new QPrintPreviewDialog(printer, this);
		preview->setWindowState(Qt::WindowMaximized);
		connect(preview, SIGNAL(paintRequested(QPrinter*)), webView, SLOT(print(QPrinter*)));
		preview->exec();
	}
	else {
		showError("No assessment selected.");
	}
}

bool ClassAssessmentsSummaryReport::validate() const
{
	//Check if the weighting equals to 100

	/*for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
		uint assessmentId { assessmentRow.sibling(assessmentRow.row(), 0).data().toUInt() };
		string weightingStr { assessmentRow.sibling(assessmentRow.row(), 1).data().toString().toStdString() };
		replace(weightingStr.begin(), weightingStr.end(), ',', '.');
		float weighting { boost::lexical_cast<float>(weightingStr) };
		assessmentWeighting.emplace(assessmentId, weighting);
	}*/
	return true;
}

float ClassAssessmentsSummaryReport::getAssessmentWeighting() const
{
	return -1.0f;
}

const AssessmentResult *ClassAssessmentsSummaryReport::getStudentAssessmentResult(const Assessment &assessment, const Student &student) const
{
	for(const auto &assessmentResult: assessment.getResults()) {
		if (assessmentResult.getStudent() == student) {
			return &assessmentResult;
		}
	}	
	return nullptr;
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

void ClassAssessmentsSummaryReport::tableWidgetAssessments_selectionChanged()
{
	if (ui.checkBoxIdenticalWeighting->checkState() == Qt::Checked) {
		calculateAutomaticWeighting();
	}
}

void ClassAssessmentsSummaryReport::checkBoxIdenticalWeighting_Changed(int state) 
{
	toggleWeightingCellsEditMode(state != Qt::Checked);
	if (state == Qt::Checked) {
		calculateAutomaticWeighting();
	}
}

void ClassAssessmentsSummaryReport::toggleWeightingCellsEditMode(bool enabled) 
{
	for(int i = 0; i < ui.tableWidgetAssessments->rowCount(); i++) {
		QTableWidgetItem *item = ui.tableWidgetAssessments->item(i, 1);
		if (enabled) {
			item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable);
		}
		else {
			item->setFlags(Qt::ItemIsSelectable);
		}
	}	
}

void ClassAssessmentsSummaryReport::calculateAutomaticWeighting() 
{
	//Calculate equal weighting for each selected assessment
	float assessmentWeighting { 100.0f };
	int selectedRowsCount { ui.tableWidgetAssessments->selectionModel()->selectedRows().count() };
	if (selectedRowsCount > 0) {
		assessmentWeighting = 100.0f / static_cast<float>(selectedRowsCount);
	}
	for(int i = 0; i < ui.tableWidgetAssessments->rowCount(); i++) {
		QTableWidgetItem *item = ui.tableWidgetAssessments->item(i, 1);
		if (item->isSelected()) {
			item->setText(to_string(assessmentWeighting).c_str());
		}
		else {
			item->setText("");
		}
	}
}
