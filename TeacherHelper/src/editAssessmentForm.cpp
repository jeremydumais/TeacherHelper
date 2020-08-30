#include "editAssessmentForm.h"
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

EditAssessmentForm::EditAssessmentForm(QWidget *parent, 
						   const DatabaseConnection &connection,
						   const EditAssessmentActionMode editMode)
	: QDialog(parent),
	  ui(Ui::editAssessmentFormClass()),
	  editMode(editMode),
	  controller(connection),
	  testTypeController(connection),
	  subjectController(connection),
	  schoolController(connection),
	  classController(connection)
{
	this->setResult(QDialog::DialogCode::Rejected);
	ui.setupUi(this);
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(pushButtonOK_Click()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(pushButtonCancel_Click()));
	connect(ui.comboBoxSchool, SIGNAL(currentIndexChanged(int)), this, SLOT(comboBoxSchool_CurrentIndexChanged()));

	if (this->editMode == EditAssessmentActionMode::Add) {
		this->setWindowTitle("Create an assessment");
	}
	else {
		this->setWindowTitle("Edit assessment");
	}
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
	if (controller.insertAssessment(Assessment(ui.lineEditName->text().trimmed().toStdString(),
											*testTypeController.findTestType(ui.comboBoxTestType->currentData().toInt()),
											*subjectController.findSubject(ui.comboBoxSubject->currentData().toInt()),
											*classController.findClass(ui.comboBoxClass->currentData().toInt()),
											getSelectedDateTime()))) {
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

void EditAssessmentForm::pushButtonCancel_Click()
{
	done(QDialog::DialogCode::Rejected);
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
	return true;
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

void EditAssessmentForm::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

