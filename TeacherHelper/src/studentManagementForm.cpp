#include "studentManagementForm.h"
#include "studentStorage.h"
#include <sstream>

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

	tableItemModel = new QStandardItemModel(0, 3);
	tableItemModel->setHorizontalHeaderItem(0, new QStandardItem("Id"));
	tableItemModel->setHorizontalHeaderItem(1, new QStandardItem("Firstname"));
	tableItemModel->setHorizontalHeaderItem(2, new QStandardItem("Lastname"));

	ui.tableViewItems->setModel(tableItemModel);
	ui.tableViewItems->verticalHeader()->setVisible(false);
	ui.tableViewItems->setColumnHidden(0, true);
	connect(ui.tableViewItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &, const QItemSelection &)));
}

StudentManagementForm::~StudentManagementForm()
{
	delete tableItemModel;
}

void StudentManagementForm::setDatabaseConnection(const DatabaseConnection &connection)
{
	this->dbConnection = &connection;
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
	tableItemModel->removeRows(0, tableItemModel->rowCount());
	size_t row {0};
    for (const auto &student : students) {
		tableItemModel->setItem(row, 0, new QStandardItem(to_string(student.getId()).c_str()));
		tableItemModel->setItem(row, 1, new QStandardItem(student.getFirstName().c_str()));
		tableItemModel->setItem(row, 2, new QStandardItem(student.getLastName().c_str()));
		row++;
    }
	toggleTableControls(false);
}

void StudentManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void StudentManagementForm::toggleEditMode(bool editMode)
{
	ui.frameDetails->setEnabled(editMode);
	ui.tableViewItems->setEnabled(!editMode);
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
	toggleEditMode(true);
	isInAddMode = true;
}

void StudentManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableViewItems->selectionModel()->selectedIndexes();
	if (row.size() > 0) {
		ui.lineEditFirstname->setText(row[1].data().toString());
		ui.lineEditLastname->setText(row[2].data().toString());
		toggleEditMode(true);
		isInAddMode = false;
	}
}

void StudentManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	stringstream ss;
	auto row = ui.tableViewItems->selectionModel()->selectedIndexes();

	ss << "Are you sure you want to delete the student " << row[1].data().toString().toStdString()
		<< " " << row[2].data().toString().toStdString() << "?";
	msgBox.setText(ss.str().c_str());
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
	if (isInAddMode) {
		if (validateEntry()) {
			StudentStorage storage(*dbConnection);
			if (storage.insertStudent(Student(ui.lineEditFirstname->text().trimmed().toStdString(),
											ui.lineEditLastname->text().trimmed().toStdString()))) {
				toggleEditMode(false);
				refreshItemsTable();
			}
			else {
				showError(storage.getLastError());
			}
		}
	}
	else {
		if (validateEntry()) {
			StudentStorage storage(*dbConnection);
			auto row = ui.tableViewItems->selectionModel()->selectedIndexes();

			if (storage.updateStudent(Student(row[0].data().toUInt(),
											ui.lineEditFirstname->text().trimmed().toStdString(),
											ui.lineEditLastname->text().trimmed().toStdString()))) {
				toggleEditMode(false);
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
	toggleEditMode(false);
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

void StudentManagementForm::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}

