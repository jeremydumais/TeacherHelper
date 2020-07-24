#include "subjectManagementForm.h"
#include <qt5/QtGui/QKeyEvent>
#include <qt5/QtWidgets/qmessagebox.h>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

using namespace std;

SubjectManagementForm::SubjectManagementForm(QWidget *parent, const DatabaseConnection &connection)
	: QDialog(parent),
	  ManagementFormBase(connection),
	  ui(Ui::subjectManagementFormClass()),
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
	ui.tableWidgeItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgeItems->setHorizontalHeaderItem(2, new QTableWidgetItem("Default"));
	ui.tableWidgeItems->setColumnHidden(0, true);
	connect(ui.tableWidgeItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &)));
	connect(ui.tableWidgeItems, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonModify_Click()));
}

void SubjectManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	controller.loadSubjects();
    refreshItemsTable();
} 

void SubjectManagementForm::refreshItemsTable()
{
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &subject : controller.getSubjects()) {
		ui.tableWidgeItems->insertRow(row);
		ui.tableWidgeItems->setItem(row, 0, new QTableWidgetItem(to_string(subject.getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 1, new QTableWidgetItem(subject.getName().c_str()));
		ui.tableWidgeItems->setItem(row, 2, new QTableWidgetItem(subject.getIsDefault() ? "X" : ""));
		ui.tableWidgeItems->item(row, 2)->setTextAlignment(Qt::AlignHCenter);
		row++;
    }
	toggleTableControls(false);
}

void SubjectManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void SubjectManagementForm::toggleEditMode(ActionMode mode)
{
	this->mode = mode;
	bool editMode = (mode ==  ActionMode::Add || mode == ActionMode::Modify);
	ui.frameDetails->setEnabled(editMode);
	ui.tableWidgeItems->setEnabled(!editMode);
	ui.frameActionButtons->setEnabled(!editMode);
	if(!editMode) {
		ui.lineEditName->clear();
		ui.checkBoxIsDefault->setCheckState(Qt::CheckState::Unchecked);
	} 
	else {
		ui.lineEditName->setFocus();
		ui.pushButtonOK->setDefault(true);
	}
}

void SubjectManagementForm::itemsTableSelectionChanged(const QItemSelection &selected)
{	
	toggleTableControls(selected.size() == 1);
}

void SubjectManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	toggleEditMode(ActionMode::Add);
}

void SubjectManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected subject
		auto editedSubject = controller.findSubject(row[0].data().toUInt());
		if (editedSubject != nullptr) {
			ui.lineEditName->setText(editedSubject->getName().c_str());
			ui.checkBoxIsDefault->setCheckState(editedSubject->getIsDefault() ? 
												Qt::CheckState::Checked : 
												Qt::CheckState::Unchecked);
			toggleEditMode(ActionMode::Modify);
		}
		else {
			showError("Unable to find the selected subject.");
		}
	}
}

void SubjectManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected subject
		auto editedSubject = controller.findSubject(row[0].data().toUInt());
		if (editedSubject != nullptr) {
			msgBox.setText(fmt::format("Are you sure you want to delete the subject {0}?", editedSubject->getName()).c_str());
			msgBox.setWindowTitle("Confirmation");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			if (msgBox.exec() == QMessageBox::Yes) {
				if (controller.deleteSubject(editedSubject->getId())) {
					refreshItemsTable();
				}
				else {
					showError(controller.getLastError());
				}
			}
		}
		else {
			showError("Unable to find the selected subject.");
		}
	}
}

void SubjectManagementForm::pushButtonOK_Click()
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

void SubjectManagementForm::saveNewItem()
{
	//Ensure that the new name is available
	string newName = ui.lineEditName->text().trimmed().toStdString();
	if (controller.isNewNameAvailableForAdd(newName)) {
		if (controller.insertSubject(Subject(ui.lineEditName->text().trimmed().toStdString(),
											 ui.checkBoxIsDefault->checkState() == Qt::CheckState::Checked))) {
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

void SubjectManagementForm::updateExistingItem()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	//Find the selected subject
	size_t currentlyEditedSubjectId = row[0].data().toUInt();
	auto editedSubject = controller.findSubject(currentlyEditedSubjectId);
	if (editedSubject != nullptr) {
		//Ensure that the new name is available
		string newName = ui.lineEditName->text().trimmed().toStdString();
		if (controller.isNewNameAvailableForUpdate(newName, currentlyEditedSubjectId)) {
			Subject editedSubjectClone { *editedSubject };
			editedSubjectClone.setName(newName);
			editedSubjectClone.setIsDefault(ui.checkBoxIsDefault->checkState() == Qt::CheckState::Checked);
			if (controller.updateSubject(editedSubjectClone)) {
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
		showError("Unable to find the selected subject.");
	}
}

void SubjectManagementForm::pushButtonCancel_Click()
{
	toggleEditMode(ActionMode::None);
}

bool SubjectManagementForm::validateEntry() const
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

void SubjectManagementForm::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape && mode != ActionMode::None) {
		pushButtonCancel_Click();
	}
	else {
		QDialog::keyPressEvent(e);
	}
}
