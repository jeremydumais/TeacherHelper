#include "classAssessmentsSummaryReportForm.h"
#include "classAssessmentsSummaryReportStudentPrecisionForm.h"
#include "multiAssessmentReportData.h"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <boost/none.hpp>
#include <fmt/format.h>
#include <fstream>
#include <QMessageBox>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <sstream>

using namespace std;

ClassAssessmentsSummaryReportForm::ClassAssessmentsSummaryReportForm(QWidget *parent, 
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
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &ClassAssessmentsSummaryReportForm::close);
	connect(ui.pushButtonShowReport, &QPushButton::clicked, this, &ClassAssessmentsSummaryReportForm::pushButtonShowReport_Clicked);
	connect(ui.comboBoxSchool, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &ClassAssessmentsSummaryReportForm::comboBoxSchool_CurrentIndexChanged);
	connect(ui.comboBoxClass, static_cast<void (QComboBox::*)(int index)>(&QComboBox::currentIndexChanged), this, &ClassAssessmentsSummaryReportForm::comboBoxClass_CurrentIndexChanged);
	connect(ui.checkBoxIdenticalWeighting, &QCheckBox::stateChanged, this, &ClassAssessmentsSummaryReportForm::checkBoxIdenticalWeighting_Changed);
	connect(ui.tableWidgetAssessments->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassAssessmentsSummaryReportForm::tableWidgetAssessments_selectionChanged);
	ui.tableWidgetAssessments->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(1, new QTableWidgetItem("Weighting %"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(2, new QTableWidgetItem("Date"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(3, new QTableWidgetItem("Name"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(4, new QTableWidgetItem("Test type"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(5, new QTableWidgetItem("Subject"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(6, new QTableWidgetItem("Nb of results"));
	ui.tableWidgetAssessments->setColumnHidden(0, true);
}

ClassAssessmentsSummaryReportForm::~ClassAssessmentsSummaryReportForm()
{
	delete webView;
}

void ClassAssessmentsSummaryReportForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/class64.png"));
	schoolController.loadSchools();
	classController.loadClasses();
	refreshSchoolComboBox();

	ui.comboBoxSchool->setCurrentIndex(1);
	ui.comboBoxClass->setCurrentIndex(2);
} 

void ClassAssessmentsSummaryReportForm::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

void ClassAssessmentsSummaryReportForm::refreshSchoolComboBox()
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

void ClassAssessmentsSummaryReportForm::refreshClassComboBox(const School &school)
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

void ClassAssessmentsSummaryReportForm::refreshAssessmentTable(const Class &itemClass) 
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

QTableWidgetItem* ClassAssessmentsSummaryReportForm::createNonEditableRow(const std::string &value) 
{
	QTableWidgetItem *item = new QTableWidgetItem(value.c_str());
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	return item;
}

QTableWidgetItem* ClassAssessmentsSummaryReportForm::createEditableRow(const std::string &value) 
{
	return new QTableWidgetItem(value.c_str());
}

void ClassAssessmentsSummaryReportForm::pushButtonShowReport_Clicked() 
{
	int selectedRowsCount { ui.tableWidgetAssessments->selectionModel()->selectedRows().count() };
	if (selectedRowsCount > 0) {
		if (validate()) {
			const School* const selectedSchool = schoolController.findSchool(ui.comboBoxSchool->currentData().toUInt());
			const Class* const selectedClass = classController.findClass(ui.comboBoxClass->currentData().toUInt());

			HTMLReport report(fmt::format("{0}/reports/MultiAssessmentSummary.html", resourcesPath), webView);
			string schoolAndClass { fmt::format("{0}, {1}", 
												selectedSchool->getName(),
												selectedClass->getName()) };
			string allAssessments { "" };
			for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
				allAssessments += fmt::format("<li>{0} ({1}) -> Weighting: {2}%</li>", 
											assessmentRow.sibling(assessmentRow.row(), 3).data().toString().toStdString(),
											assessmentRow.sibling(assessmentRow.row(), 2).data().toString().toStdString(),
											getAssessmentWeighting(assessmentRow));
			}
			report.setProperties({ { "<:SCHOOLANDCLASS>", schoolAndClass },
								{ "<:ASSESSMENTSINCLUDED>", allAssessments } });
			//Load report data
			vector<shared_ptr<IReportData>> reportData;
			//Load assessments results
			assessmentController.loadAssessmentsByClass(selectedClass->getId());
			//Process phase 1
			unordered_map<size_t, map<size_t, float>> userIdAssessmentIdWeighting;
			vector<size_t> studentWithDifferentWeighting;
			if (!processPhase1(selectedClass->getMembers(), 
							   selectedRowsCount, 
							   userIdAssessmentIdWeighting, 
							   studentWithDifferentWeighting)) {
				return;
			}
			//Process phase 2
			if (!processPhase2(selectedClass->getMembers(),
							   userIdAssessmentIdWeighting,
							   studentWithDifferentWeighting,
							   reportData)) {
				return;
			}

			report.setData(reportData);
			if (!report.previewReport(false)) {
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
	}
	else {
		showError("No assessment selected.");
	}
}

bool ClassAssessmentsSummaryReportForm::validate() const
{
	float total { 0.0f };
	//Check if all weighting are valid
	for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
		float weighting { getAssessmentWeighting(assessmentRow) };
		if (weighting == -1.0f) {
			showError(fmt::format("Invalid weighting for the assessment {0}", 
								  assessmentRow.sibling(assessmentRow.row(), 3).data().toString().toStdString()));
			return false;
		}
		else {
			if (weighting <= 0.0f || weighting > 100.0f) {
				showError(fmt::format("Weighting for the assessment {0} must be between 1 and 100", 
									  assessmentRow.sibling(assessmentRow.row(), 3).data().toString().toStdString()));
				return false;
			}
			total += weighting;
		}
	}
	//Check if the weighting equals to 100
	if (total != 100) {
		showError("The total of weighting must be 100");
		return false;
	}
	return true;
}

//Phase 1: Load all students and check if each of them have a result in the selected assessment
//If not them let the user choose another weighting
bool ClassAssessmentsSummaryReportForm::processPhase1(const list<Student> &students,
													  const int selectedAssessmentCount,
													  unordered_map<size_t, map<size_t, float>> &userIdAssessmentIdWeighting,
													  vector<size_t> &studentWithDifferentWeighting)
{
	for(const auto &student : students) {
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
				return false;
			}
		}
		//If number of result missing > 0 and < total assessment selected then ask the user to select a different weighting
		if (nbOfResultMissing > 0) {
			if (nbOfResultMissing == selectedAssessmentCount) {
				//Add the student with no result at all
				userIdAssessmentIdWeighting.insert(make_pair(student.getId(), map<size_t, float>()));
			}
			else {
				//Ask for different weighting
				vector<const Assessment *> assessmentsToGetWeighting;
				for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
					uint assessmentId { assessmentRow.sibling(assessmentRow.row(), 0).data().toUInt() };
					const auto *assessment { assessmentController.findAssessment(assessmentId) };
					if (std::find(assessmentWithoutResult.begin(), 
									assessmentWithoutResult.end(), 
									assessmentId) == assessmentWithoutResult.end()) {
						assessmentsToGetWeighting.push_back(assessment);
					}
				}
				ClassAssessmentsSummaryReportStudentPrecisionForm precisionForm(this, student.getFullName(), assessmentsToGetWeighting);
				if (precisionForm.exec()) {
					//Add the student with it's customized weighting
					userIdAssessmentIdWeighting.insert(make_pair(student.getId(), precisionForm.getWeightingResult()));
					studentWithDifferentWeighting.push_back(student.getId());
				}
				else {
					return false;
				}
			}
		}
		else {
			//Add the student with is standard weighting
			map<size_t, float> assessmentWeighting;
			for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
				uint assessmentId { assessmentRow.sibling(assessmentRow.row(), 0).data().toUInt() };
				float weighting { getAssessmentWeighting(assessmentRow) };
				assessmentWeighting.emplace(assessmentId, weighting);
			}
			userIdAssessmentIdWeighting.insert(make_pair(student.getId(), assessmentWeighting));
		}
	}
	return true;
}
			
//Phase 2: Calculate the average from the weighting assigned
bool ClassAssessmentsSummaryReportForm::processPhase2(const list<Student> &students, 
					   								  unordered_map<size_t, map<size_t, float>> &userIdAssessmentIdWeighting,
					   								  vector<size_t> &studentWithDifferentWeighting,
													  vector<shared_ptr<IReportData>> &reportData) 
{
	for(const auto &userIdAssessmentId : userIdAssessmentIdWeighting) {
		const Student *student { findStudent(userIdAssessmentId.first, students) };
		if (student == nullptr) {
			showError(fmt::format("Unable to find student with id {0}", userIdAssessmentId.first));
			return false;
		}
		float average { 0.0f };
		if (!userIdAssessmentId.second.empty()) {
			for(const auto &assessmentIdAndWeighting : userIdAssessmentId.second) {
				const auto *assessment { assessmentController.findAssessment(assessmentIdAndWeighting.first) };
				if (assessment != nullptr) {
					//Get student assessment result and calculate on the selected weighting
					auto studentResult { assessment->getStudentResult(*student) };
					if (studentResult.has_value()) {
						average += (studentResult->getResult() * assessmentIdAndWeighting.second) / assessment->getMaxScore();
					}
					else {
						showError(fmt::format("Unable to find assessment result for student {0} on assessment id {1}", 
											student->getFullName(),
											assessmentIdAndWeighting.first));
						return false;
					}
				}
				else {
					showError(fmt::format("Unable to find assessment with id {0}", assessmentIdAndWeighting.first));
					return false;
				}
			}
			bool studentHasDifferentWeighting { std::find(studentWithDifferentWeighting.begin(),
														  studentWithDifferentWeighting.end(),
														  student->getId()) != studentWithDifferentWeighting.end()};
			reportData.push_back(make_shared<MultiAssessmentReportData>(student->getFirstName(), 
																		student->getLastName() + (studentHasDifferentWeighting ? "*" : ""), 
																		average));
		}
		else {
			//No result at all for the student
			reportData.push_back(make_shared<MultiAssessmentReportData>(student->getFirstName(), 
																		student->getLastName(), 
																		boost::none));
		}
	}
	return true;
}

float ClassAssessmentsSummaryReportForm::getAssessmentWeighting(const QModelIndex &assessmentRow) const
{
	float retVal;
	string weightingStr { assessmentRow.sibling(assessmentRow.row(), 1).data().toString().toStdString() };
	replace(weightingStr.begin(), weightingStr.end(), ',', '.');
	try {
		retVal = boost::lexical_cast<float>(weightingStr);
	}
	catch(boost::bad_lexical_cast &err) {
		retVal = -1.0f;
	}
	return retVal;
}

const AssessmentResult *ClassAssessmentsSummaryReportForm::getStudentAssessmentResult(const Assessment &assessment, const Student &student) const
{
	for(const auto &assessmentResult: assessment.getResults()) {
		if (assessmentResult.getStudent() == student) {
			return &assessmentResult;
		}
	}	
	return nullptr;
}

const Student* ClassAssessmentsSummaryReportForm::findStudent(const size_t id, const list<Student> &students) const
{
	for(const auto &student : students) {
		if (student.getId() == id) {
			return &student;
		}
	}
	return nullptr;
}

void ClassAssessmentsSummaryReportForm::comboBoxSchool_CurrentIndexChanged() 
{
	//Find the selected school
	if (ui.comboBoxSchool->currentData().toInt() >= 1) {
		const School* const selectedSchool = schoolController.findSchool(ui.comboBoxSchool->currentData().toUInt());
		if (selectedSchool) {
			refreshClassComboBox(*selectedSchool);
		}
	}
}

void ClassAssessmentsSummaryReportForm::comboBoxClass_CurrentIndexChanged() 
{
	//Find the selected class
	if (ui.comboBoxClass->currentData().toInt() >= 1) {
		const Class* const selectedClass = classController.findClass(ui.comboBoxClass->currentData().toUInt());
		if (selectedClass) {
			refreshAssessmentTable(*selectedClass);
		}
	}
}

void ClassAssessmentsSummaryReportForm::tableWidgetAssessments_selectionChanged()
{
	if (ui.checkBoxIdenticalWeighting->checkState() == Qt::Checked) {
		calculateAutomaticWeighting();
	}
}

void ClassAssessmentsSummaryReportForm::checkBoxIdenticalWeighting_Changed(int state) 
{
	toggleWeightingCellsEditMode(state != Qt::Checked);
	if (state == Qt::Checked) {
		calculateAutomaticWeighting();
	}
}

void ClassAssessmentsSummaryReportForm::toggleWeightingCellsEditMode(bool enabled) 
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

void ClassAssessmentsSummaryReportForm::calculateAutomaticWeighting() 
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
