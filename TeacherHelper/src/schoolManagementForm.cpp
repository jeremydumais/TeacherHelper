#include "schoolManagementForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>

using namespace std;

SchoolManagementForm::SchoolManagementForm(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ManagementFormBase(databaseController.getDatabaseConnection()),
	  ui(Ui::schoolManagementFormClass()),
	  controller(databaseController),
	  cityController(databaseController)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &SchoolManagementForm::close);
	connect(ui.pushButtonAdd, &QPushButton::clicked, this, &SchoolManagementForm::pushButtonAdd_Click);
	connect(ui.pushButtonModify, &QPushButton::clicked, this, &SchoolManagementForm::pushButtonModify_Click);
	connect(ui.pushButtonDelete, &QPushButton::clicked, this, &SchoolManagementForm::pushButtonDelete_Click);
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &SchoolManagementForm::pushButtonOK_Click);
	connect(ui.pushButtonCancel, &QPushButton::clicked, this, &SchoolManagementForm::pushButtonCancel_Click);
	tableWidgetItemsKeyWatcher.installOn(ui.tableWidgetItems);
	connect(&tableWidgetItemsKeyWatcher, &QTableWidgetKeyPressWatcher::keyPressed, this, &SchoolManagementForm::tableWidgetItems_keyPressEvent);

	ui.tableWidgetItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgetItems->setHorizontalHeaderItem(2, new QTableWidgetItem("City_Id"));
	ui.tableWidgetItems->setHorizontalHeaderItem(3, new QTableWidgetItem("City"));
	ui.tableWidgetItems->setColumnHidden(0, true);
	ui.tableWidgetItems->setColumnHidden(2, true);
	connect(ui.tableWidgetItems->selectionModel(), 
		&QItemSelectionModel::selectionChanged, 
		this,
  		&SchoolManagementForm::itemsTableSelectionChanged);
	connect(ui.tableWidgetItems,
		&QTableWidget::itemDoubleClicked,
		this,
		&SchoolManagementForm::itemsTableSelectionDoubleClicked);
}

void SchoolManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/schools64.png"));
	controller.loadSchools();
	cityController.loadCities();
	refreshCityTable();
    refreshItemsTable();
}

void SchoolManagementForm::refreshItemsTable()
{
	ui.tableWidgetItems->model()->removeRows(0, ui.tableWidgetItems->rowCount());
	size_t row {0};
    for (const auto &school : controller.getSchools()) {
		ui.tableWidgetItems->insertRow(row);
		ui.tableWidgetItems->setItem(row, 0, new QTableWidgetItem(to_string(school.getId()).c_str()));
		ui.tableWidgetItems->setItem(row, 1, new QTableWidgetItem(school.getName().c_str()));
		ui.tableWidgetItems->setItem(row, 2, new QTableWidgetItem(to_string(school.getCity().getId()).c_str()));
		ui.tableWidgetItems->setItem(row, 3, new QTableWidgetItem(school.getCity().getName().c_str()));
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
	ui.tableWidgetItems->setEnabled(!editMode);
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

void SchoolManagementForm::itemsTableSelectionDoubleClicked(QTableWidgetItem *item) 
{
	if (item) {
		pushButtonModify_Click();
	}
}

void SchoolManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	ui.comboBoxCity->setCurrentIndex(0);
	toggleEditMode(ActionMode::Add);
}

void SchoolManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
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

void SchoolManagementForm::tableWidgetItems_keyPressEvent(int key, int, int) 
{
	if (key == Qt::Key_Delete) {
		pushButtonDelete_Click();
	}
}
