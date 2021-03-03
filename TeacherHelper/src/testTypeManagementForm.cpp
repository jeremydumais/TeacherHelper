#include "testTypeManagementForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

using namespace std;

TestTypeManagementForm::TestTypeManagementForm(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ManagementFormBase(databaseController.getDatabaseConnection()),
	  ui(Ui::testTypeManagementFormClass()),
	  controller(databaseController)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &TestTypeManagementForm::close);
	connect(ui.pushButtonAdd, &QPushButton::clicked, this, &TestTypeManagementForm::pushButtonAdd_Click);
	connect(ui.pushButtonModify, &QPushButton::clicked, this, &TestTypeManagementForm::pushButtonModify_Click);
	connect(ui.pushButtonDelete, &QPushButton::clicked, this, &TestTypeManagementForm::pushButtonDelete_Click);
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &TestTypeManagementForm::pushButtonOK_Click);
	connect(ui.pushButtonCancel, &QPushButton::clicked, this, &TestTypeManagementForm::pushButtonCancel_Click);
	tableWidgetItemsKeyWatcher.installOn(ui.tableWidgetItems);
	connect(&tableWidgetItemsKeyWatcher, &QTableWidgetKeyPressWatcher::keyPressed, this, &TestTypeManagementForm::tableWidgetItems_keyPressEvent);

	ui.tableWidgetItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgetItems->setColumnHidden(0, true);
	connect(ui.tableWidgetItems->selectionModel(), 
		&QItemSelectionModel::selectionChanged, 
		this,
  		&TestTypeManagementForm::itemsTableSelectionChanged);
	connect(ui.tableWidgetItems, 
		&QTableWidget::itemDoubleClicked,
		this,
		&TestTypeManagementForm::itemsTableSelectionDoubleClicked);
}

void TestTypeManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/testType64.png"));
	controller.loadTestTypes();
    refreshItemsTable();
} 

void TestTypeManagementForm::refreshItemsTable()
{
	ui.tableWidgetItems->model()->removeRows(0, ui.tableWidgetItems->rowCount());
	int row {0};
    for (const auto &testType : controller.getTestTypes()) {
		ui.tableWidgetItems->insertRow(row);
		ui.tableWidgetItems->setItem(row, 0, new QTableWidgetItem(to_string(testType.getId()).c_str()));
		ui.tableWidgetItems->setItem(row, 1, new QTableWidgetItem(testType.getName().c_str()));
		row++;
    }
	toggleTableControls(false);
}

void TestTypeManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void TestTypeManagementForm::toggleEditMode(ActionMode mode)
{
	this->mode = mode;
	bool editMode = (mode ==  ActionMode::Add || mode == ActionMode::Modify);
	ui.frameDetails->setEnabled(editMode);
	ui.tableWidgetItems->setEnabled(!editMode);
	ui.frameActionButtons->setEnabled(!editMode);
	if(!editMode) {
		ui.lineEditName->clear();
	} 
	else {
		ui.lineEditName->setFocus();
		ui.pushButtonOK->setDefault(true);
	}
}

void TestTypeManagementForm::itemsTableSelectionChanged(const QItemSelection &selected)
{	
	toggleTableControls(selected.size() == 1);
}

void TestTypeManagementForm::itemsTableSelectionDoubleClicked(QTableWidgetItem *item) 
{
	if (item) {
		pushButtonModify_Click();
	}
}

void TestTypeManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	toggleEditMode(ActionMode::Add);
}

void TestTypeManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected testType
		auto editedTestType = controller.findTestType(row[0].data().toUInt());
		if (editedTestType != nullptr) {
			ui.lineEditName->setText(editedTestType->getName().c_str());
			toggleEditMode(ActionMode::Modify);
		}
		else {
			showError("Unable to find the selected test type.");
		}
	}
}

void TestTypeManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected testType
		auto editedTestType = controller.findTestType(row[0].data().toUInt());
		if (editedTestType != nullptr) {
			msgBox.setText(fmt::format("Are you sure you want to delete the test type {0}?", editedTestType->getName()).c_str());
			msgBox.setWindowTitle("Confirmation");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			if (msgBox.exec() == QMessageBox::Yes) {
				if (controller.deleteTestType(editedTestType->getId())) {
					refreshItemsTable();
				}
				else {
					showError(controller.getLastError());
				}
			}
		}
		else {
			showError("Unable to find the selected test type.");
		}
	}
}

void TestTypeManagementForm::pushButtonOK_Click()
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

void TestTypeManagementForm::saveNewItem()
{
	//Ensure that the new name is available
	string newName = ui.lineEditName->text().trimmed().toStdString();
	if (controller.isNewNameAvailableForAdd(newName)) {
		if (controller.insertTestType(TestType(ui.lineEditName->text().trimmed().toStdString()))) {
			toggleEditMode(ActionMode::None);
			refreshItemsTable();
		}
		else {
			showError(controller.getLastError());
		}
	}
	else {
		showError("The new name is already taken.");
	}
}

void TestTypeManagementForm::updateExistingItem()
{
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
	//Find the selected testType
	size_t currentlyEditedTestTypeId = row[0].data().toUInt();
	auto editedTestType = controller.findTestType(currentlyEditedTestTypeId);
	if (editedTestType != nullptr) {
		//Ensure that the new name is available
		string newName = ui.lineEditName->text().trimmed().toStdString();
		if (controller.isNewNameAvailableForUpdate(newName, currentlyEditedTestTypeId)) {
			TestType editedTestTypeClone { *editedTestType };
			editedTestTypeClone.setName(newName);
			if (controller.updateTestType(editedTestTypeClone)) {
				toggleEditMode(ActionMode::None);
				refreshItemsTable();
			}
			else {
				showError(controller.getLastError());
			}
		}
		else {
			showError("The new name is already taken.");
		}
	}
	else {
		showError("Unable to find the selected test type.");
	}
}

void TestTypeManagementForm::pushButtonCancel_Click()
{
	toggleEditMode(ActionMode::None);
}

bool TestTypeManagementForm::validateEntry() const
{
	if (ui.lineEditName->text().trimmed().isEmpty()) {
		showError("The name is required!");
		return false;
	}
	if (ui.lineEditName->text().trimmed().length() > 50) {
		showError("The name must not be greater than 50 characters!");
		return false;
	}
	return true;
}

void TestTypeManagementForm::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape && mode != ActionMode::None) {
		pushButtonCancel_Click();
	}
	else {
		QDialog::keyPressEvent(e);
	}
}

void TestTypeManagementForm::tableWidgetItems_keyPressEvent(int key, int, int) 
{
	if (key == Qt::Key_Delete) {
		pushButtonDelete_Click();
	}
}