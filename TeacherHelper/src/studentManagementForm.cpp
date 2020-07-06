#include "studentManagementForm.h"
#include "studentStorage.h"
#include <sstream>
#include <qt5/QtWidgets/qmessagebox.h>
#include <qt5/QtGui/QKeyEvent>

using namespace std;

StudentManagementForm::StudentManagementForm(QWidget *parent)
	: QDialog(parent)
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
    refreshItemsTable();
} 

void StudentManagementForm::refreshItemsTable()
{
	StudentStorage studentStorage(*dbConnection);
	list<Student> students = studentStorage.getAllStudents();
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &student : students) {
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
	toggleEditMode(ActionMode::Add);
}

void StudentManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (row.size() > 0) {
		ui.lineEditFirstname->setText(row[1].data().toString());
		ui.lineEditLastname->setText(row[2].data().toString());
		toggleEditMode(ActionMode::Modify);
	}
}

void StudentManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	stringstream ss;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();

	ss << "Are you sure you want to delete the student " << row[1].data().toString().toStdString()
		<< " " << row[2].data().toString().toStdString() << "?";
	msgBox.setText(ss.str().c_str());
	msgBox.setWindowTitle("Confirmation");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);

	if (msgBox.exec() == QMessageBox::Yes) {
		StudentStorage storage(*dbConnection);

		if (storage.deleteStudent(row[0].data().toUInt())) {
			refreshItemsTable();
		}
		else {
			showError(storage.getLastError());
		}
	}
}

void StudentManagementForm::pushButtonOK_Click()
{
	if (mode == ActionMode::Add) {
		if (validateEntry()) {
			StudentStorage storage(*dbConnection);
			if (storage.insertStudent(Student(ui.lineEditFirstname->text().trimmed().toStdString(),
											ui.lineEditLastname->text().trimmed().toStdString()))) {
				toggleEditMode(ActionMode::None);
				refreshItemsTable();
			}
			else {
				showError(storage.getLastError());
			}
		}
	}
	else if (mode == ActionMode::Modify) {
		if (validateEntry()) {
			StudentStorage storage(*dbConnection);
			auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();

			if (storage.updateStudent(Student(row[0].data().toUInt(),
											ui.lineEditFirstname->text().trimmed().toStdString(),
											ui.lineEditLastname->text().trimmed().toStdString()))) {
				toggleEditMode(ActionMode::None);
				refreshItemsTable();
			}
			else {
				showError(storage.getLastError());
			}
		}	
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
