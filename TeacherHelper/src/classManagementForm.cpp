#include "classManagementForm.h"
#include "studentSelectionForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <boost/algorithm/string.hpp>
#include <fmt/format.h>
#include <iostream>

using namespace std;

ClassManagementForm::ClassManagementForm(QWidget *parent, const DatabaseConnection &connection)
	: QDialog(parent),
	  ManagementFormBase(connection),
	  ui(Ui::classManagementFormClass()),
	  controller(connection),
	  schoolController(connection),
	  studentController(connection)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pushButtonAdd, SIGNAL(clicked()), this, SLOT(pushButtonAdd_Click()));
	connect(ui.pushButtonDuplicate, SIGNAL(clicked()), this, SLOT(pushButtonDuplicate_Click()));
	connect(ui.pushButtonModify, SIGNAL(clicked()), this, SLOT(pushButtonModify_Click()));
	connect(ui.pushButtonDelete, SIGNAL(clicked()), this, SLOT(pushButtonDelete_Click()));
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(pushButtonOK_Click()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(pushButtonCancel_Click()));
	connect(ui.pushButtonAddMember, SIGNAL(clicked()), this, SLOT(pushButtonAddMember_Click()));
	connect(ui.pushButtonRemoveMember, SIGNAL(clicked()), this, SLOT(pushButtonRemoveMember_Click()));

	ui.tableWidgeItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgeItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgeItems->setHorizontalHeaderItem(2, new QTableWidgetItem("School"));
	ui.tableWidgeItems->setHorizontalHeaderItem(3, new QTableWidgetItem("School City"));
	ui.tableWidgeItems->setHorizontalHeaderItem(4, new QTableWidgetItem("school_id"));
	ui.tableWidgeItems->setColumnHidden(0, true);
	ui.tableWidgeItems->setColumnHidden(4, true);
	connect(ui.tableWidgeItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &)));
	connect(ui.tableWidgeItems, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonModify_Click()));
	ui.tableWidgetMembers->setHorizontalHeaderItem(1, new QTableWidgetItem("Members"));
	ui.tableWidgetMembers->setColumnHidden(0, true);
	connect(ui.tableWidgetMembers->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(membersTableSelectionChanged(const QItemSelection &)));
}

void ClassManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	controller.loadClasses();
	schoolController.loadSchools();
	studentController.loadStudents();
	refreshSchoolTable();
    refreshItemsTable();
	//pushButtonAdd_Click();
} 

void ClassManagementForm::refreshItemsTable()
{
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &itemClass : controller.getClasses()) {
		ui.tableWidgeItems->insertRow(row);
		ui.tableWidgeItems->setItem(row, 0, new QTableWidgetItem(to_string(itemClass.getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 1, new QTableWidgetItem(itemClass.getName().c_str()));
		ui.tableWidgeItems->setItem(row, 2, new QTableWidgetItem(itemClass.getSchool().getName().c_str()));
		ui.tableWidgeItems->setItem(row, 3, new QTableWidgetItem(itemClass.getSchool().getCity().getName().c_str()));
		ui.tableWidgeItems->setItem(row, 4, new QTableWidgetItem(to_string(itemClass.getSchool().getId()).c_str()));
		row++;
    }
	toggleTableControls(false);
	toggleMembersTableControls(false);
}

void ClassManagementForm::refreshSchoolTable()
{
	ui.comboBoxSchool->clear();
	//Add a first empty choice
	ui.comboBoxSchool->addItem("", -1);
	for(const auto &school : schoolController.getSchools()) {
		ui.comboBoxSchool->addItem(fmt::format("{0} ({1})", school.getName(), school.getCity().getName()).c_str(), 
								   static_cast<uint>(school.getId()));
	}
}

void ClassManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonDuplicate->setEnabled(itemSelected);
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void ClassManagementForm::toggleMembersTableControls(bool itemSelected) 
{
	ui.pushButtonRemoveMember->setEnabled(itemSelected);
}

void ClassManagementForm::toggleEditMode(ActionMode mode)
{
	this->mode = mode;
	bool editMode = (mode == ActionMode::Add || mode == ActionMode::Modify);
	ui.frameDetails->setEnabled(editMode);
	ui.tableWidgeItems->setEnabled(!editMode);
	ui.frameActionButtons->setEnabled(!editMode);
	if(!editMode) {
		ui.lineEditName->clear();
		ui.comboBoxSchool->setCurrentIndex(0);
		ui.tableWidgetMembers->model()->removeRows(0, ui.tableWidgetMembers->rowCount());
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

void ClassManagementForm::membersTableSelectionChanged(const QItemSelection &selected) 
{
	toggleMembersTableControls(selected.size() == 1);
}

void ClassManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	ui.comboBoxSchool->setCurrentIndex(0);
	toggleEditMode(ActionMode::Add);
}

void ClassManagementForm::pushButtonDuplicate_Click()
{
	pushButtonModify_Click();
	toggleEditMode(ActionMode::Add);
	ui.lineEditName->setText(ui.lineEditName->text() + " copy");
}

void ClassManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected school
		auto editedClass = controller.findClass(row[0].data().toUInt());
		if (editedClass) {
			ui.lineEditName->setText(editedClass->getName().c_str());
			//Find the selected school
			if (!selectSchoolInEditPanel(editedClass->getSchool().getId())) {
				showError("Cannot select the school.");
				return;
			}
			//Add all members
			size_t row {0};
			for(const auto &member : editedClass->getMembers()) {
				string comments = boost::trim_copy(member.getComments());
				if (!boost::empty(comments)) {
					comments = " (" + comments + ")";
				}
				ui.tableWidgetMembers->insertRow(row);
				ui.tableWidgetMembers->setItem(row, 0, new QTableWidgetItem(to_string(member.getId()).c_str()));
				ui.tableWidgetMembers->setItem(row, 1, new QTableWidgetItem(fmt::format("{0}, {1}{2}", 
																						member.getLastName(), 
																						member.getFirstName(), 
																						comments).c_str()));
				}
			toggleEditMode(ActionMode::Modify);
		}
		else {
			showError("Cannot find the selected class.");
		}
	}
}

bool ClassManagementForm::selectSchoolInEditPanel(size_t id)
{
	for (int i = 0; i < ui.comboBoxSchool->count(); i++) {
		if (ui.comboBoxSchool->itemData(i).toInt() == static_cast<int>(id)) {
			ui.comboBoxSchool->setCurrentIndex(i);
			return true;
		}
	}
	return false;
}

void ClassManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected class
		auto editedClass = controller.findClass(row[0].data().toUInt());
		msgBox.setText(fmt::format("Are you sure you want to delete the class {0} ({1})?", 
						editedClass->getName(), 
						editedClass->getSchool().getName()).c_str());
		msgBox.setWindowTitle("Confirmation");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		if (msgBox.exec() == QMessageBox::Yes) {
			if (controller.deleteClass(editedClass->getId())) {
				refreshItemsTable();
			}
			else {
				showError(controller.getLastError());
			}
		}
	}
}

void ClassManagementForm::pushButtonOK_Click()
{
	if (validateEntry()) {
		//Find the selected school
		const School* const selectedSchool = schoolController.findSchool(ui.comboBoxSchool->currentData().toUInt());
		if (selectedSchool) {
			if (mode == ActionMode::Add) {
				saveNewItem(selectedSchool);
			}
			else if (mode == ActionMode::Modify) {
				updateExistingItem(selectedSchool);
			}	
		}
		else {
			showError("Cannot find the selected school.");
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
	if (ui.comboBoxSchool->currentData().toInt() < 1) {
		showError("The school is required!");
		return false;
	}
	return true;
}

void ClassManagementForm::saveNewItem(const School* const selectedSchool) 
{
	//Ensure that the new name is available
	string newName = ui.lineEditName->text().trimmed().toStdString();
	if (controller.isNewNameAvailableForAdd(newName, selectedSchool->getId())) {
		auto classToAdd = Class(ui.lineEditName->text().trimmed().toStdString(),
								*selectedSchool);
		for(int i = 0; i < ui.tableWidgetMembers->rowCount(); ++i) {
			auto member = studentController.findStudent(ui.tableWidgetMembers->item(i, 0)->data(0).toUInt());
			if (member) {
				classToAdd.addMember(*member);
			}
			else {
				showError(fmt::format("Unable to add the user {0}", ui.tableWidgetMembers->item(i, 1)->data(0).toString().toStdString()));
				return;
			}
		}
		if (controller.insertClass(classToAdd)) {
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

void ClassManagementForm::updateExistingItem(const School* const selectedSchool) 
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	//Find the selected school
	size_t currentlyEditedClassId = row[0].data().toUInt();
	auto editedClass = controller.findClass(currentlyEditedClassId);
	if (editedClass != nullptr) {
		//Ensure that the new name is available
		string newName = ui.lineEditName->text().trimmed().toStdString();
		if (controller.isNewNameAvailableForUpdate(newName, selectedSchool->getId(), currentlyEditedClassId)) {
			Class editedClassClone { *editedClass };
			editedClassClone.setName(newName);
			editedClassClone.setSchool(*selectedSchool);
			editedClassClone.clearMembers();
			for(int i = 0; i < ui.tableWidgetMembers->rowCount(); ++i) {
				auto member = studentController.findStudent(ui.tableWidgetMembers->item(i, 0)->data(0).toUInt());
				if (member) {
					editedClassClone.addMember(*member);
				}
				else {
					showError(fmt::format("Unable to add the user {0}", ui.tableWidgetMembers->item(i, 1)->data(0).toString().toStdString()));
					return;
				}
			}
			if (controller.updateClass(editedClassClone)) {
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
		showError("Unable to find the selected class.");
	}
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

void ClassManagementForm::pushButtonAddMember_Click() 
{
	StudentSelectionForm formStudentSelection(this, *dbConnection);
	formStudentSelection.exec();
	auto student = formStudentSelection.getSelectedStudent();
	if (student) {
		//Ensure that the student is not already in the list
		if (ui.tableWidgetMembers->findItems(to_string(student->getId()).c_str(), Qt::MatchFlag::MatchExactly).size() == 0) {
			size_t row = ui.tableWidgetMembers->rowCount();
			ui.tableWidgetMembers->insertRow(row);
			ui.tableWidgetMembers->setItem(row, 0, new QTableWidgetItem(to_string(student->getId()).c_str()));
			ui.tableWidgetMembers->setItem(row, 1, new QTableWidgetItem(fmt::format("{0}, {1} ({2})", 
																					student->getLastName(), 
																					student->getFirstName(), 
																					student->getComments()).c_str()));
		}
	}
}

void ClassManagementForm::pushButtonRemoveMember_Click() 
{
	auto row = ui.tableWidgetMembers->selectionModel()->selectedIndexes();
	ui.tableWidgetMembers->removeRow(row[0].row());
}