#include "classManagementForm.h"
#include "classStorage.h"
#include <qt5/QtGui/QKeyEvent>
#include <qt5/QtWidgets/qmessagebox.h>
#include <sstream>

using namespace std;

ClassManagementForm::ClassManagementForm(QWidget *parent)
	: QDialog(parent),
	  ui(Ui::classManagementFormClass())
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
	ui.tableWidgeItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgeItems->setHorizontalHeaderItem(2, new QTableWidgetItem("School"));
	ui.tableWidgeItems->setHorizontalHeaderItem(3, new QTableWidgetItem("school City"));
	ui.tableWidgeItems->setHorizontalHeaderItem(4, new QTableWidgetItem("school_id"));
	ui.tableWidgeItems->setColumnHidden(0, true);
	ui.tableWidgeItems->setColumnHidden(4, true);
	connect(ui.tableWidgeItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &)));
	connect(ui.tableWidgeItems, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonModify_Click()));
}

void ClassManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
    refreshItemsTable();
} 

void ClassManagementForm::refreshItemsTable()
{
	ClassStorage classStorage(*dbConnection);
	list<Class> classes = classStorage.getAllClasses();
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &itemClass : classes) {
		ui.tableWidgeItems->insertRow(row);
		ui.tableWidgeItems->setItem(row, 0, new QTableWidgetItem(to_string(itemClass.getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 1, new QTableWidgetItem(itemClass.getName().c_str()));
		ui.tableWidgeItems->setItem(row, 2, new QTableWidgetItem(itemClass.getSchool().getName().c_str()));
		ui.tableWidgeItems->setItem(row, 3, new QTableWidgetItem(itemClass.getSchool().getCity().c_str()));
		ui.tableWidgeItems->setItem(row, 4, new QTableWidgetItem(to_string(itemClass.getSchool().getId()).c_str()));
		row++;
    }
	toggleTableControls(false);
}

void ClassManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void ClassManagementForm::toggleEditMode(ActionMode mode)
{
	this->mode = mode;
	bool editMode = (mode ==  ActionMode::Add || mode == ActionMode::Modifiy);
	ui.frameDetails->setEnabled(editMode);
	ui.tableWidgeItems->setEnabled(!editMode);
	ui.frameActionButtons->setEnabled(!editMode);
	if(!editMode) {
		ui.lineEditName->clear();
		//ui.comboBoxSchool->se();
	} 
	else {
		ui.lineEditName->setFocus();
		ui.pushButtonOK->setDefault(true);
	}
}

void ClassManagementForm::itemsTableSelectionChanged(const QItemSelection &selected)
{	
	toggleTableControls(selected.size() == 1);
}

void ClassManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	//ui.lineEditCity->clear();
	toggleEditMode(ActionMode::Add);
}

void ClassManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		ui.lineEditName->setText(row[1].data().toString());
		//ui.lineEditCity->setText(row[2].data().toString());
		toggleEditMode(ActionMode::Modifiy);
	}
}

void ClassManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	stringstream ss;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();

	ss << "Are you sure you want to delete the school " << row[1].data().toString().toStdString() << "?";
	msgBox.setText(ss.str().c_str());
	msgBox.setWindowTitle("Confirmation");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);

	if (msgBox.exec() == QMessageBox::Yes) {
		ClassStorage storage(*dbConnection);

		if (storage.deleteClass(row[0].data().toUInt())) {
			refreshItemsTable();
		}
		else {
			showError(storage.getLastError());
		}
	}
}

void ClassManagementForm::pushButtonOK_Click()
{
	if (mode == ActionMode::Add) {
		if (validateEntry()) {
			/*ClassStorage storage(*dbConnection);
			if (storage.insertClass(Class(ui.lineEditName->text().trimmed().toStdString(),
											ui.lineEditCity->text().trimmed().toStdString()))) {
				toggleEditMode(ActionMode::None);
				refreshItemsTable();
			}
			else {
				showError(storage.getLastError());
			}*/
		}
	}
	else if (mode == ActionMode::Modifiy) {
		if (validateEntry()) {
			/*ClassStorage storage(*dbConnection);
			auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();

			if (storage.updateSchool(School(row[0].data().toUInt(),
											ui.lineEditName->text().trimmed().toStdString(),
											ui.lineEditCity->text().trimmed().toStdString()))) {
				toggleEditMode(ActionMode::None);
				refreshItemsTable();
			}
			else {
				showError(storage.getLastError());
			}*/
		}	
	}
}

void ClassManagementForm::pushButtonCancel_Click()
{
	toggleEditMode(ActionMode::None);
}

bool ClassManagementForm::validateEntry() const
{
	if (ui.lineEditName->text().trimmed().isEmpty()) {
		showError("The name is required!");
		return false;
	}
	if (ui.lineEditName->text().trimmed().length() > 50) {
		showError("The name must not be greater than 50 characters!");
		return false;
	}
	/*if (ui.lineEditCity->text().trimmed().isEmpty()) {
		showError("The city is required!");
		return false;
	}
	if (ui.lineEditCity->text().trimmed().length() > 50) {
		showError("The city must not be greater than 50 characters!");
		return false;
	}*/
	return true;
}

void ClassManagementForm::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape && mode != ActionMode::None) {
		pushButtonCancel_Click();
	}
	else {
		QDialog::keyPressEvent(e);
	}
}
