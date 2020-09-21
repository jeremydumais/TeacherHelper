#include "editAssessmentForm.h"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <fmt/format.h>
#include <QtWidgets/qmessagebox.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

EditAssessmentForm::EditAssessmentForm(QWidget *parent, 
						   const DatabaseConnection &connection,
						   const EditAssessmentActionMode editMode,
						   const Assessment * const  assessmentToEdit)
	: QDialog(parent),
	  ui(Ui::editAssessmentFormClass()),
	  editMode(editMode),
	  controller(connection),
	  testTypeController(connection),
	  subjectController(connection),
	  schoolController(connection),
	  classController(connection),
	  assessmentToEdit(assessmentToEdit)
{
	this->setResult(QDialog::DialogCode::Rejected);
	ui.setupUi(this);
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(pushButtonOK_Click()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(pushButtonCancel_Click()));
	connect(ui.comboBoxSchool, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSchool_CurrentIndexChanged()));
	connect(ui.comboBoxClass, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxClass_CurrentIndexChanged()));

	if (this->editMode == EditAssessmentActionMode::Add) {
		this->setWindowTitle("Create an assessment");
		ui.dateEditAssessmentDate->setDate(QDate::currentDate());
		ui.timeEditAssessmentTime->setTime(QTime(QTime::currentTime().hour(), 0));
	}
	else {
		this->setWindowTitle("Edit assessment");
	}

	ui.tableWidgetResults->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetResults->setHorizontalHeaderItem(1, new QTableWidgetItem("First Name"));
	ui.tableWidgetResults->setHorizontalHeaderItem(2, new QTableWidgetItem("Last Name"));
	ui.tableWidgetResults->setHorizontalHeaderItem(3, new QTableWidgetItem("Result"));
	ui.tableWidgetResults->setHorizontalHeaderItem(4, new QTableWidgetItem("Comments"));
	ui.tableWidgetResults->setColumnHidden(0, true);
}

void EditAssessmentForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	testTypeController.loadTestTypes();
	subjectController.loadSubjects();
	schoolController.loadSchools();
	classController.loadClasses();
	refreshTestTypeComboBox();
	refreshSubjectComboBox();
	refreshSchoolComboBox();

	//Edit mode (display assessment value)
	if (editMode == EditAssessmentActionMode::Modify) {
		ui.lineEditName->setText(assessmentToEdit->getName().c_str());
		//ui.comboBoxTestType->setSele
	}
} 

void EditAssessmentForm::pushButtonOK_Click()
{
	if (validateEntry()) {
		if (editMode == EditAssessmentActionMode::Add) {
			saveNewItem();
		}
		else {
			updateExistingItem();
		}
	}
}

void EditAssessmentForm::saveNewItem() 
{
	
	if (controller.insertAssessment(getAssessmentFromFields())) {
		done(QDialog::DialogCode::Accepted);
	}
	else {
		showError(controller.getLastError());
	}
}

void EditAssessmentForm::updateExistingItem() 
{
	/*auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	//Find the selected city
	size_t currentlyEditedStudentId = row[0].data().toUInt();
	auto editedStudent = controller.findStudent(currentlyEditedStudentId);
	if (editedStudent != nullptr) {
		Student editedStudentClone { *editedStudent };
		editedStudentClone.setFirstName(ui.lineEditFirstname->text().trimmed().toStdString());
		editedStudentClone.setLastName(ui.lineEditLastname->text().trimmed().toStdString());
		editedStudentClone.setComments(ui.plainTextEditComments->toPlainText().trimmed().toStdString());
		if (controller.updateStudent(editedStudentClone)) {
			toggleEditMode(ActionMode::None);
			refreshItemsTable();
		}
		else {
			showError(controller.getLastError());
		}
	}
	else {
		showError("Unable to find the selected student.");
	}*/
}

bool EditAssessmentForm::validateEntry() const
{
	if (ui.lineEditName->text().trimmed().isEmpty()) {
		showError("The name is required!");
		return false;
	}
	if (ui.lineEditName->text().trimmed().length() > 100) {
		showError("The name must not be greater than 100 characters!");
		return false;
	}
	if (ui.comboBoxTestType->currentData().toInt() < 1 || 
		testTypeController.findTestType(ui.comboBoxTestType->currentData().toInt()) == nullptr) {
		showError("The test type is required!");
		return false;
	}
	if (ui.comboBoxSubject->currentData().toInt() < 1 || 
		subjectController.findSubject(ui.comboBoxSubject->currentData().toInt()) == nullptr) {
		showError("The subject is required!");
		return false;
	}
	if (ui.comboBoxSchool->currentData().toInt() < 1 || 
		schoolController.findSchool(ui.comboBoxSchool->currentData().toInt()) == nullptr) {
		showError("The school is required!");
		return false;
	}
	if (ui.comboBoxClass->currentData().toInt() < 1 || 
		classController.findClass(ui.comboBoxClass->currentData().toInt()) == nullptr) {
		showError("The class is required!");
		return false;
	}
	//Check each results
	const auto resultModel = ui.tableWidgetResults->model();
	for(int i=0; i<resultModel->rowCount() ;i++) {
		//Check the validity of the result
		float result { 0.0f };
		if (boost::trim_copy(resultModel->index(i, 3).data().toString().toStdString()) != "") {
			try {
				result = boost::lexical_cast<float>(resultModel->index(i, 3).data().toString().toStdString());
			}
			catch(boost::bad_lexical_cast &) {
				showError(fmt::format("The result of {0} is invalid!", 
					getStudentNameFromTableLine(resultModel, i)));
				return false;
			}
			if (result < 0 || result > 100.0f) {
				showError(fmt::format("The result of {0} must be between 0 and 100.", 
					getStudentNameFromTableLine(resultModel, i)));
				return false;
			}
		}
		//Check the comment length
		if (resultModel->index(i, 4).data().toString().toStdString().length() > 256) {
        	showError(fmt::format("The comment of {0} exceed the 256 chars limit!", 
				getStudentNameFromTableLine(resultModel, i)));
			return false;
		}
	}
	return true;
}

Assessment EditAssessmentForm::getAssessmentFromFields() const
{
	Assessment assessment(editMode == EditAssessmentActionMode::Modify && assessmentToEdit != nullptr ? assessmentToEdit->getId() : 0,
							ui.lineEditName->text().trimmed().toStdString(),
							*testTypeController.findTestType(ui.comboBoxTestType->currentData().toInt()),
							*subjectController.findSubject(ui.comboBoxSubject->currentData().toInt()),
							*classController.findClass(ui.comboBoxClass->currentData().toInt()),
							getSelectedDateTime());
	const auto resultModel = ui.tableWidgetResults->model();
	for(int i=0; i<resultModel->rowCount() ;i++) {
		string result = boost::trim_copy(resultModel->index(i, 3).data().toString().toStdString());
		if (result != "") {
			assessment.addResult(AssessmentResult(Student(resultModel->index(i, 0).data().toInt(),
													resultModel->index(i, 1).data().toString().toStdString(),
													resultModel->index(i, 2).data().toString().toStdString()),
												boost::lexical_cast<float>(result),
												resultModel->index(i, 4).data().toString().toStdString()));
		}
	}
	return assessment;
}

std::string EditAssessmentForm::getStudentNameFromTableLine(QAbstractItemModel * const model, int rowIndex) const
{
	return fmt::format("{0} {1}", 
		model->index(rowIndex, 1).data().toString().toStdString(),
		model->index(rowIndex, 2).data().toString().toStdString());
}

void EditAssessmentForm::pushButtonCancel_Click()
{
	done(QDialog::DialogCode::Rejected);
}

ptime EditAssessmentForm::getSelectedDateTime() const
{
	auto selectedDate = ui.dateEditAssessmentDate->date();
	auto selectedTime = ui.timeEditAssessmentTime->time();
	return ptime(date(selectedDate.year(), selectedDate.month(), selectedDate.day()),
				time_duration(selectedTime.hour(), selectedTime.minute(), selectedTime.second()));
}

void EditAssessmentForm::refreshTestTypeComboBox()
{
	ui.comboBoxTestType->clear();
	//Add a first empty choice
	ui.comboBoxTestType->addItem("", -1);
	for(const auto &testype : testTypeController.getTestTypes()) {
		ui.comboBoxTestType->addItem(testype.getName().c_str(), static_cast<uint>(testype.getId()));
	}
}

void EditAssessmentForm::refreshSubjectComboBox()
{
	ui.comboBoxSubject->clear();
	//Add a first empty choice
	ui.comboBoxSubject->addItem("", -1);
	for(const auto &subject : subjectController.getSubjects()) {
		ui.comboBoxSubject->addItem(subject.getName().c_str(), static_cast<uint>(subject.getId()));
	}
}

void EditAssessmentForm::refreshSchoolComboBox()
{
	ui.comboBoxSchool->clear();
	//Add a first empty choice
	ui.comboBoxSchool->addItem("", -1);
	for(const auto &school : schoolController.getSchools()) {
		ui.comboBoxSchool->addItem(school.getName().c_str(), static_cast<uint>(school.getId()));
	}
	//Clear the class ComboBox list
	ui.comboBoxClass->clear();
}

void EditAssessmentForm::refreshClassComboBox(const School &school) 
{
	ui.comboBoxClass->clear();
	//Add a first empty choice
	ui.comboBoxClass->addItem("", -1);
	for(const auto &itemClass : classController.getClassesBySchool(school)) {
		ui.comboBoxClass->addItem(itemClass.getName().c_str(), static_cast<uint>(itemClass.getId()));
	}
}

void EditAssessmentForm::comboBoxSchool_CurrentIndexChanged() 
{
	//Find the selected school
	if (ui.comboBoxSchool->currentData().toInt() >= 1) {
		const School* const selectedSchool = schoolController.findSchool(ui.comboBoxSchool->currentData().toUInt());
		if (selectedSchool) {
			refreshClassComboBox(*selectedSchool);
		}
	}
}

void EditAssessmentForm::comboBoxClass_CurrentIndexChanged() 
{
	if (ui.comboBoxClass->currentData().toInt() >= 1) {
		const Class* const selectedClass = classController.findClass(ui.comboBoxClass->currentData().toUInt());
		if (selectedClass) {
			refreshStudentList(*selectedClass);
		}
	}
}

void EditAssessmentForm::refreshStudentList(const Class &aClass) 
{
	ui.tableWidgetResults->model()->removeRows(0, ui.tableWidgetResults->rowCount());
	size_t row {0};
	for(const auto &student : aClass.getMembers()) {
		ui.tableWidgetResults->insertRow(row);
		ui.tableWidgetResults->setItem(row, 0, new QTableWidgetItem(to_string(student.getId()).c_str()));
		auto nonEditableFirstName = new QTableWidgetItem(student.getFirstName().c_str());
		nonEditableFirstName->setFlags(nonEditableFirstName->flags() & ~Qt::ItemIsEditable); 
		ui.tableWidgetResults->setItem(row, 1, nonEditableFirstName);
		auto nonEditableLastName = new QTableWidgetItem(student.getLastName().c_str());
		nonEditableLastName->setFlags(nonEditableLastName->flags() & ~Qt::ItemIsEditable); 
		ui.tableWidgetResults->setItem(row, 2, nonEditableLastName);
		row++;
	}
}

void EditAssessmentForm::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

