#include "studentManagementForm.h"
#include <fmt/format.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/QKeyEvent>

using namespace std;

StudentManagementForm::StudentManagementForm(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ManagementFormBase(databaseController.getDatabaseConnection()),
	  ui(Ui::studentManagementFormClass()),
	  controller(databaseController)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &StudentManagementForm::close);
	connect(ui.pushButtonAdd, &QPushButton::clicked, this, &StudentManagementForm::pushButtonAdd_Click);
	connect(ui.pushButtonModify, &QPushButton::clicked, this, &StudentManagementForm::pushButtonModify_Click);
	connect(ui.pushButtonDelete, &QPushButton::clicked, this, &StudentManagementForm::pushButtonDelete_Click);
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &StudentManagementForm::pushButtonOK_Click);
	connect(ui.pushButtonCancel, &QPushButton::clicked, this, &StudentManagementForm::pushButtonCancel_Click);
	tableWidgetItemsKeyWatcher.installOn(ui.tableWidgetItems);
	connect(&tableWidgetItemsKeyWatcher, &QTableWidgetKeyPressWatcher::keyPressed, this, &StudentManagementForm::tableWidgetItems_keyPressEvent);

	ui.tableWidgetItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Firstname"));
	ui.tableWidgetItems->setHorizontalHeaderItem(2, new QTableWidgetItem("Lastname"));
	ui.tableWidgetItems->setColumnHidden(0, true);
	connect(ui.tableWidgetItems->selectionModel(), 
		&QItemSelectionModel::selectionChanged, 
		this,
  		&StudentManagementForm::itemsTableSelectionChanged);
	connect(ui.tableWidgetItems, 
		&QTableWidget::itemDoubleClicked,
		this,
		&StudentManagementForm::itemsTableSelectionDoubleClicked);
}

StudentManagementForm::~StudentManagementForm()
{
}

void StudentManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/student64.png"));
	controller.loadStudents();
    refreshItemsTable();
} 

void StudentManagementForm::refreshItemsTable()
{
	ui.tableWidgetItems->model()->removeRows(0, ui.tableWidgetItems->rowCount());
	size_t row {0};
    for (const auto &student : controller.getStudents()) {
		ui.tableWidgetItems->insertRow(row);
		ui.tableWidgetItems->setItem(row, 0, new QTableWidgetItem(to_string(student.getId()).c_str()));
		ui.tableWidgetItems->setItem(row, 1, new QTableWidgetItem(student.getFirstName().c_str()));
		ui.tableWidgetItems->setItem(row, 2, new QTableWidgetItem(student.getLastName().c_str()));
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
	ui.tableWidgetItems->setEnabled(!editMode);
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

void StudentManagementForm::itemsTableSelectionDoubleClicked(QTableWidgetItem *item) 
{
	if (item) {
		pushButtonModify_Click();
	}
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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

void StudentManagementForm::tableWidgetItems_keyPressEvent(int key, int, int) 
{
	if (key == Qt::Key_Delete) {
		pushButtonDelete_Click();
	}
}
