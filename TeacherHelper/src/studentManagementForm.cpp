#include "studentManagementForm.h"
#include <fmt/format.h>
#include <qt5/QtWidgets/qmessagebox.h>
#include <qt5/QtGui/QKeyEvent>

using namespace std;

StudentManagementForm::StudentManagementForm(QWidget *parent, const DatabaseConnection &connection)
	: QDialog(parent),
	  ManagementFormBase(connection),
	  ui(Ui::studentManagementFormClass()),
	  controller(connection)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButtonAdd, SIGNAL(clicked()), this, SLOT(pushButtonAdd_Click()));
	connect(ui.pushButtonModify, SIGNAL(clicked()), this, SLOT(pushButtonModify_Click()));
	connect(ui.pushButtonDelete, SIGNAL(clicked()), this, SLOT(pushButtonDelete_Click()));
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(pushButtonOK_Click()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(pushButtonCancel_Click()));

	ui.tableWidgeItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgeItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Firstname"));
	ui.tableWidgeItems->setHorizontalHeaderItem(2, new QTableWidgetItem("Lastname"));
	ui.tableWidgeItems->setColumnHidden(0, true);
	connect(ui.tableWidgeItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
	connect(ui.tableWidgeItems, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonModify_Click()));
}

StudentManagementForm::~StudentManagementForm()
{
}

void StudentManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	controller.loadStudents();
    refreshItemsTable();
} 

void StudentManagementForm::refreshItemsTable()
{
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &student : controller.getStudents()) {
		ui.tableWidgeItems->insertRow(row);
		ui.tableWidgeItems->setItem(row, 0, new QTableWidgetItem(to_string(student.getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 1, new QTableWidgetItem(student.getFirstName().c_str()));
		ui.tableWidgeItems->setItem(row, 2, new QTableWidgetItem(student.getLastName().c_str()));
		row++;
    }
	toggleTableControls(false);
}

void StudentManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void StudentManagementForm::toggleEditMode(ActionMode mode)
{
	this->mode = mode;
	bool editMode = (mode ==  ActionMode::Add || mode == ActionMode::Modify);
	ui.frameDetails->setEnabled(editMode);
	ui.tableWidgeItems->setEnabled(!editMode);
	ui.frameActionButtons->setEnabled(!editMode);
	if(!editMode) {
		ui.lineEditFirstname->clear();
		ui.lineEditLastname->clear();
		ui.plainTextEditComments->clear();
	} 
	else {
		ui.lineEditFirstname->setFocus();
		ui.pushButtonOK->setDefault(true);
	}
}

void StudentManagementForm::itemsTableSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{	
	toggleTableControls(selected.size() == 1);
}

void StudentManagementForm::pushButtonAdd_Click()
{
	ui.lineEditFirstname->clear();
	ui.lineEditLastname->clear();
	ui.plainTextEditComments->clear();
	toggleEditMode(ActionMode::Add);
}

void StudentManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected student
		auto editedStudent = controller.findStudent(row[0].data().toUInt());
		if (editedStudent != nullptr) {
			ui.lineEditFirstname->setText(editedStudent->getFirstName().c_str());
			ui.lineEditLastname->setText(editedStudent->getLastName().c_str());
			ui.plainTextEditComments->setPlainText(editedStudent->getComments().c_str());
			toggleEditMode(ActionMode::Modify);
		}
		else {
			showError("Unable to find the student city.");
		}
	}
}

void StudentManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected city
		auto editedStudent = controller.findStudent(row[0].data().toUInt());
		if (editedStudent) {
			string message = fmt::format("Are you sure you want to delete the student {0} {1}?", 
										editedStudent->getFirstName(), 
										editedStudent->getLastName());
			msgBox.setText(message.c_str());
			msgBox.setWindowTitle("Confirmation");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);

			if (msgBox.exec() == QMessageBox::Yes) {
				if (controller.deleteStudent(editedStudent->getId())) {
					refreshItemsTable();
				}
				else {
					showError(controller.getLastError());
				}
			}
		}
		else {
			showError("Unable to find the selected student.");
		}
	}
}

void StudentManagementForm::pushButtonOK_Click()
{
	if (validateEntry()) {
		if (mode == ActionMode::Add) {
			saveNewItem();
		}
		else if (mode == ActionMode::Modify) {
			updateExistingItem();
		}
	}
}

void StudentManagementForm::saveNewItem() 
{
	if (controller.insertStudent(Student(ui.lineEditFirstname->text().trimmed().toStdString(),
											ui.lineEditLastname->text().trimmed().toStdString(),
											ui.plainTextEditComments->toPlainText().trimmed().toStdString()))) {
		toggleEditMode(ActionMode::None);
		refreshItemsTable();
	}
	else {
		showError(controller.getLastError());
	}
}

void StudentManagementForm::updateExistingItem() 
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
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
	}
}

void StudentManagementForm::pushButtonCancel_Click()
{
	toggleEditMode(ActionMode::None);
}

bool StudentManagementForm::validateEntry() const
{
	if (ui.lineEditFirstname->text().trimmed().isEmpty()) {
		showError("The firstname is required!");
		return false;
	}
	if (ui.lineEditFirstname->text().trimmed().length() > 30) {
		showError("The firstname must not be greater than 30 characters!");
		return false;
	}
	if (ui.lineEditLastname->text().trimmed().isEmpty()) {
		showError("The lastname is required!");
		return false;
	}
	if (ui.lineEditLastname->text().trimmed().length() > 30) {
		showError("The lastname must not be greater than 30 characters!");
		return false;
	}
	return true;
}

void StudentManagementForm::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape && mode != ActionMode::None)
		pushButtonCancel_Click();
	else
		QDialog::keyPressEvent(e);
}
