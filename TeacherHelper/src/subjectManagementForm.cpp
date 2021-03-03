#include "subjectManagementForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

using namespace std;

SubjectManagementForm::SubjectManagementForm(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ManagementFormBase(databaseController.getDatabaseConnection()),
	  ui(Ui::subjectManagementFormClass()),
	  controller(databaseController)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &SubjectManagementForm::close);
	connect(ui.pushButtonAdd, &QPushButton::clicked, this, &SubjectManagementForm::pushButtonAdd_Click);
	connect(ui.pushButtonModify, &QPushButton::clicked, this, &SubjectManagementForm::pushButtonModify_Click);
	connect(ui.pushButtonDelete, &QPushButton::clicked, this, &SubjectManagementForm::pushButtonDelete_Click);
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &SubjectManagementForm::pushButtonOK_Click);
	connect(ui.pushButtonCancel, &QPushButton::clicked, this, &SubjectManagementForm::pushButtonCancel_Click);
	tableWidgetItemsKeyWatcher.installOn(ui.tableWidgetItems);
	connect(&tableWidgetItemsKeyWatcher, &QTableWidgetKeyPressWatcher::keyPressed, this, &SubjectManagementForm::tableWidgetItems_keyPressEvent);

	ui.tableWidgetItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgetItems->setHorizontalHeaderItem(2, new QTableWidgetItem("Default"));
	ui.tableWidgetItems->setColumnHidden(0, true);
	connect(ui.tableWidgetItems->selectionModel(), 
		&QItemSelectionModel::selectionChanged, 
		this,
  		&SubjectManagementForm::itemsTableSelectionChanged);
	connect(ui.tableWidgetItems, 
		&QTableWidget::itemDoubleClicked,
		this,
		&SubjectManagementForm::itemsTableSelectionDoubleClicked);
}

void SubjectManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/subject64.png"));
	controller.loadSubjects();
    refreshItemsTable();
} 

void SubjectManagementForm::refreshItemsTable()
{
	ui.tableWidgetItems->model()->removeRows(0, ui.tableWidgetItems->rowCount());
	int row {0};
    for (const auto &subject : controller.getSubjects()) {
		ui.tableWidgetItems->insertRow(row);
		ui.tableWidgetItems->setItem(row, 0, new QTableWidgetItem(to_string(subject.getId()).c_str()));
		ui.tableWidgetItems->setItem(row, 1, new QTableWidgetItem(subject.getName().c_str()));
		ui.tableWidgetItems->setItem(row, 2, new QTableWidgetItem(subject.getIsDefault() ? "X" : ""));
		ui.tableWidgetItems->item(row, 2)->setTextAlignment(Qt::AlignHCenter);
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
	ui.tableWidgetItems->setEnabled(!editMode);
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

void SubjectManagementForm::itemsTableSelectionDoubleClicked(QTableWidgetItem *item) 
{
	if (item) {
		pushButtonModify_Click();
	}
}

void SubjectManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	toggleEditMode(ActionMode::Add);
}

void SubjectManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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

void SubjectManagementForm::tableWidgetItems_keyPressEvent(int key, int, int) 
{
	if (key == Qt::Key_Delete) {
		pushButtonDelete_Click();
	}
}