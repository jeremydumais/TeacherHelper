#include "classAssessmentsSummaryReportStudentPrecisionForm.h"
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <QMessageBox>
#include <sstream>

using namespace std;

ClassAssessmentsSummaryReportStudentPrecisionForm::ClassAssessmentsSummaryReportStudentPrecisionForm(QWidget *parent, 
															 const std::string &studentName,
															 const std::vector<const Assessment *> &assessments)
	: QDialog(parent),
	  ui(Ui::classAssessmentsSummaryReportStudentPrecisionFormClass()),
	  studentName(studentName),
	  assessments(assessments)
{
	ui.setupUi(this);
	connect(ui.pushButtonCancel, &QPushButton::clicked, this, &ClassAssessmentsSummaryReportStudentPrecisionForm::reject);
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &ClassAssessmentsSummaryReportStudentPrecisionForm::pushButtonOK_Clicked);
	connect(ui.checkBoxIdenticalWeighting, &QCheckBox::stateChanged, this, &ClassAssessmentsSummaryReportStudentPrecisionForm::checkBoxIdenticalWeighting_Changed);
	connect(ui.tableWidgetAssessments->selectionModel(), &QItemSelectionModel::selectionChanged, this, &ClassAssessmentsSummaryReportStudentPrecisionForm::tableWidgetAssessments_selectionChanged);
	ui.tableWidgetAssessments->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(1, new QTableWidgetItem("Weighting %"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(2, new QTableWidgetItem("Date"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(3, new QTableWidgetItem("Name"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(4, new QTableWidgetItem("Test type"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(5, new QTableWidgetItem("Subject"));
	ui.tableWidgetAssessments->setHorizontalHeaderItem(6, new QTableWidgetItem("Nb of results"));
	ui.tableWidgetAssessments->setColumnHidden(0, true);
	ui.labelStudentName->setText(studentName.c_str());
}

void ClassAssessmentsSummaryReportStudentPrecisionForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/class64.png"));
	refreshAssessmentTable();
}

map<size_t, float> ClassAssessmentsSummaryReportStudentPrecisionForm::getWeightingResult() const
{
	return result;
} 

void ClassAssessmentsSummaryReportStudentPrecisionForm::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

void ClassAssessmentsSummaryReportStudentPrecisionForm::refreshAssessmentTable() 
{
	ui.tableWidgetAssessments->model()->removeRows(0, ui.tableWidgetAssessments->rowCount());
	size_t row {0};
	for(const auto &assessment : assessments) {
		ui.tableWidgetAssessments->insertRow(row);
		ui.tableWidgetAssessments->setItem(row, 0, createNonEditableRow(to_string(assessment->getId())));
		ui.tableWidgetAssessments->setItem(row, 1, createEditableRow(""));
		ui.tableWidgetAssessments->setItem(row, 2, createNonEditableRow(to_simple_string(assessment->getDateTime())));
		ui.tableWidgetAssessments->setItem(row, 3, createNonEditableRow(assessment->getName()));
		ui.tableWidgetAssessments->setItem(row, 4, createNonEditableRow(assessment->getTestType().getName()));
		ui.tableWidgetAssessments->setItem(row, 5, createNonEditableRow(assessment->getSubject().getName()));
		ui.tableWidgetAssessments->setItem(row, 6, createNonEditableRow((to_string(assessment->getResults().size()))));
		row++;
	}
	toggleWeightingCellsEditMode(!ui.checkBoxIdenticalWeighting->isChecked());
}

QTableWidgetItem* ClassAssessmentsSummaryReportStudentPrecisionForm::createNonEditableRow(const std::string &value) 
{
	QTableWidgetItem *item = new QTableWidgetItem(value.c_str());
	item->setFlags(item->flags() & ~Qt::ItemIsEditable);
	return item;
}

QTableWidgetItem* ClassAssessmentsSummaryReportStudentPrecisionForm::createEditableRow(const std::string &value) 
{
	return new QTableWidgetItem(value.c_str());
}

void ClassAssessmentsSummaryReportStudentPrecisionForm::pushButtonOK_Clicked() 
{
	int selectedRowsCount { ui.tableWidgetAssessments->selectionModel()->selectedRows().count() };
	if (selectedRowsCount > 0) {
		if (validate()) {
			result.clear();
			for(const auto &assessmentRow : ui.tableWidgetAssessments->selectionModel()->selectedRows()) {
				uint assessmentId { assessmentRow.sibling(assessmentRow.row(), 0).data().toUInt() };
				float weighting { getAssessmentWeighting(assessmentRow) };
				result.insert( { assessmentId, weighting } );
			}
			accept();
		}
	}
	else {
		showError("No assessment selected.");
	}
}

bool ClassAssessmentsSummaryReportStudentPrecisionForm::validate() const
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

float ClassAssessmentsSummaryReportStudentPrecisionForm::getAssessmentWeighting(const QModelIndex &assessmentRow) const
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

const AssessmentResult *ClassAssessmentsSummaryReportStudentPrecisionForm::getStudentAssessmentResult(const Assessment &assessment, const Student &student) const
{
	for(const auto &assessmentResult: assessment.getResults()) {
		if (assessmentResult.getStudent() == student) {
			return &assessmentResult;
		}
	}	
	return nullptr;
}

void ClassAssessmentsSummaryReportStudentPrecisionForm::tableWidgetAssessments_selectionChanged()
{
	if (ui.checkBoxIdenticalWeighting->checkState() == Qt::Checked) {
		calculateAutomaticWeighting();
	}
}

void ClassAssessmentsSummaryReportStudentPrecisionForm::checkBoxIdenticalWeighting_Changed(int state) 
{
	toggleWeightingCellsEditMode(state != Qt::Checked);
	if (state == Qt::Checked) {
		calculateAutomaticWeighting();
	}
}

void ClassAssessmentsSummaryReportStudentPrecisionForm::toggleWeightingCellsEditMode(bool enabled) 
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

void ClassAssessmentsSummaryReportStudentPrecisionForm::calculateAutomaticWeighting() 
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
