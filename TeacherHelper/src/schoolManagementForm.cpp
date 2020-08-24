#include "schoolManagementForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>

using namespace std;

SchoolManagementForm::SchoolManagementForm(QWidget *parent, const DatabaseConnection &connection)
	: QDialog(parent),
	  ManagementFormBase(connection),
	  ui(Ui::schoolManagementFormClass()),
	  controller(connection),
	  cityController(connection)
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
	ui.tableWidgeItems->setHorizontalHeaderItem(2, new QTableWidgetItem("City_Id"));
	ui.tableWidgeItems->setHorizontalHeaderItem(3, new QTableWidgetItem("City"));
	ui.tableWidgeItems->setColumnHidden(0, true);
	ui.tableWidgeItems->setColumnHidden(2, true);
	connect(ui.tableWidgeItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &)));
	connect(ui.tableWidgeItems, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonModify_Click()));
}

void SchoolManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	controller.loadSchools();
	cityController.loadCities();
	refreshCityTable();
    refreshItemsTable();
}

void SchoolManagementForm::refreshItemsTable()
{
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &school : controller.getSchools()) {
		ui.tableWidgeItems->insertRow(row);
		ui.tableWidgeItems->setItem(row, 0, new QTableWidgetItem(to_string(school.getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 1, new QTableWidgetItem(school.getName().c_str()));
		ui.tableWidgeItems->setItem(row, 2, new QTableWidgetItem(to_string(school.getCity().getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 3, new QTableWidgetItem(school.getCity().getName().c_str()));
		row++;
    }
	toggleTableControls(false);
}

void SchoolManagementForm::refreshCityTable()
{
	ui.comboBoxCity->clear();
	//Add a first empty choice
	ui.comboBoxCity->addItem("", -1);
	for(const auto &city : cityController.getCities()) {
		ui.comboBoxCity->addItem(city.getName().c_str(), static_cast<uint>(city.getId()));
	}
}

void SchoolManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void SchoolManagementForm::toggleEditMode(ActionMode mode)
{
	this->mode = mode;
	bool editMode = (mode ==  ActionMode::Add || mode == ActionMode::Modify);
	ui.frameDetails->setEnabled(editMode);
	ui.tableWidgeItems->setEnabled(!editMode);
	ui.frameActionButtons->setEnabled(!editMode);
	if(!editMode) {
		ui.lineEditName->clear();
		ui.comboBoxCity->setCurrentIndex(0);
	} 
	else {
		ui.lineEditName->setFocus();
		ui.pushButtonOK->setDefault(true);
	}
}

void SchoolManagementForm::itemsTableSelectionChanged(const QItemSelection &selected)
{	
	toggleTableControls(selected.size() == 1);
}

void SchoolManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	ui.comboBoxCity->setCurrentIndex(0);
	toggleEditMode(ActionMode::Add);
}

void SchoolManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected school
		auto editedSchool = controller.findSchool(row[0].data().toUInt());
		if (editedSchool) {
			ui.lineEditName->setText(editedSchool->getName().c_str());
			//Find the selected city
			if (!selectCityInEditPanel(editedSchool->getCity().getId())) {
				showError("Cannot select the city.");
				return;
			}
			toggleEditMode(ActionMode::Modify);
		}
		else {
			showError("Cannot find the selected school.");
		}
	}
}

void SchoolManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected school
		auto editedSchool = controller.findSchool(row[0].data().toUInt());
		msgBox.setText(fmt::format("Are you sure you want to delete the school {0}?", editedSchool->getName()).c_str());
		msgBox.setWindowTitle("Confirmation");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
		msgBox.setDefaultButton(QMessageBox::Cancel);
		if (msgBox.exec() == QMessageBox::Yes) {
			if (controller.deleteSchool(editedSchool->getId())) {
				refreshItemsTable();
			}
			else {
				showError(controller.getLastError());
			}
			dataHasChanged = true;
		}
	}
}

void SchoolManagementForm::pushButtonOK_Click()
{
	if (validateEntry()) {
		//Find the selected city
		const City* const selectedCity = cityController.findCity(ui.comboBoxCity->currentData().toUInt());
		if (selectedCity) {
			if (mode == ActionMode::Add) {
				saveNewItem(selectedCity);
			}
			else if (mode == ActionMode::Modify) {
				updateExistingItem(selectedCity);
			}	
		}
		else {
			showError("Cannot find the selected city.");
		}
	}
}

void SchoolManagementForm::saveNewItem(const City* const selectedCity)
{
	//Ensure that the new name is available
	string newName = ui.lineEditName->text().trimmed().toStdString();
	if (controller.isNewNameAvailableForAdd(newName)) {
		if (controller.insertSchool(School(ui.lineEditName->text().trimmed().toStdString(),
										*selectedCity))) {
			toggleEditMode(ActionMode::None);
			refreshItemsTable();
		}
		else {
			showError(controller.getLastError());
		}
		dataHasChanged = true;
	}
	else {
		showError("The new name is already taken.");
	}
}

void SchoolManagementForm::updateExistingItem(const City* const selectedCity)
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	//Find the selected school
	size_t currentlyEditedSchoolId = row[0].data().toUInt();
	auto editedSchool = controller.findSchool(currentlyEditedSchoolId);
	if (editedSchool != nullptr) {
		//Ensure that the new name is available
		string newName = ui.lineEditName->text().trimmed().toStdString();
		if (controller.isNewNameAvailableForUpdate(newName, currentlyEditedSchoolId)) {
			School editedSchoolClone { *editedSchool };
			editedSchoolClone.setName(newName);
			editedSchoolClone.setCity(*selectedCity);
			if (controller.updateSchool(editedSchoolClone)) {
				toggleEditMode(ActionMode::None);
				refreshItemsTable();
			}
			else {
				showError(controller.getLastError());
			}
			dataHasChanged = true;
		}
		else {
			showError("The new name is already taken.");
		}
	}
	else {
		showError("Unable to find the selected school.");
	}
}

void SchoolManagementForm::pushButtonCancel_Click()
{
	toggleEditMode(ActionMode::None);
}

bool SchoolManagementForm::validateEntry() const
{
	if (ui.lineEditName->text().trimmed().isEmpty()) {
		showError("The name is required!");
		return false;
	}
	if (ui.lineEditName->text().trimmed().length() > 50) {
		showError("The name must not be greater than 50 characters!");
		return false;
	}
	if (ui.comboBoxCity->currentData().toInt() < 1) {
		showError("The city is required!");
		return false;
	}
	return true;
}

void SchoolManagementForm::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape && mode != ActionMode::None) {
		pushButtonCancel_Click();
	}
	else {
		QDialog::keyPressEvent(e);
	}
}

bool SchoolManagementForm::selectCityInEditPanel(size_t id)
{
	for (int i = 0; i < ui.comboBoxCity->count(); i++) {
		if (ui.comboBoxCity->itemData(i).toInt() == static_cast<int>(id)) {
			ui.comboBoxCity->setCurrentIndex(i);
			return true;
		}
	}
	return false;
}
