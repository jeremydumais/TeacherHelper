#include "cityManagementForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

using namespace std;

CityManagementForm::CityManagementForm(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ManagementFormBase(databaseController.getDatabaseConnection()),
	  ui(Ui::cityManagementFormClass()),
	  controller(databaseController)
{
	ui.setupUi(this);
	ui.frameDetails->setEnabled(false);
	connect(ui.pushButtonClose, &QPushButton::clicked, this, &CityManagementForm::close);
	connect(ui.pushButtonAdd, &QPushButton::clicked, this, &CityManagementForm::pushButtonAdd_Click);
	connect(ui.pushButtonModify, &QPushButton::clicked, this, &CityManagementForm::pushButtonModify_Click);
	connect(ui.pushButtonDelete, &QPushButton::clicked, this, &CityManagementForm::pushButtonDelete_Click);
	connect(ui.pushButtonOK, &QPushButton::clicked, this, &CityManagementForm::pushButtonOK_Click);
	connect(ui.pushButtonCancel, &QPushButton::clicked, this, &CityManagementForm::pushButtonCancel_Click);
	tableWidgetItemsKeyWatcher.installOn(ui.tableWidgetItems);
	connect(&tableWidgetItemsKeyWatcher, &QTableWidgetKeyPressWatcher::keyPressed, this, &CityManagementForm::tableWidgetItems_keyPressEvent);

	ui.tableWidgetItems->setHorizontalHeaderItem(0, new QTableWidgetItem("Id"));
	ui.tableWidgetItems->setHorizontalHeaderItem(1, new QTableWidgetItem("Name"));
	ui.tableWidgetItems->setColumnHidden(0, true);
	connect(ui.tableWidgetItems->selectionModel(), 
		&QItemSelectionModel::selectionChanged, 
		this,
  		&CityManagementForm::itemsTableSelectionChanged);
	connect(ui.tableWidgetItems, 
		&QTableWidget::itemDoubleClicked,
		this,
		&CityManagementForm::itemsTableSelectionDoubleClicked);
}

void CityManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	controller.loadCities();
    refreshItemsTable();
	setWindowIcon(QIcon(":/global/cities64.png"));
} 

void CityManagementForm::refreshItemsTable()
{
	ui.tableWidgetItems->model()->removeRows(0, ui.tableWidgetItems->rowCount());
	size_t row {0};
    for (const auto &city : controller.getCities()) {
		ui.tableWidgetItems->insertRow(row);
		ui.tableWidgetItems->setItem(row, 0, new QTableWidgetItem(to_string(city.getId()).c_str()));
		ui.tableWidgetItems->setItem(row, 1, new QTableWidgetItem(city.getName().c_str()));
		row++;
    }
	toggleTableControls(false);
}

void CityManagementForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonModify->setEnabled(itemSelected);
	ui.pushButtonDelete->setEnabled(itemSelected);
}

void CityManagementForm::toggleEditMode(ActionMode mode)
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

void CityManagementForm::itemsTableSelectionChanged(const QItemSelection &selected)
{	
	toggleTableControls(selected.size() == 1);
}

void CityManagementForm::itemsTableSelectionDoubleClicked(QTableWidgetItem *item) 
{
	if (item) {
		pushButtonModify_Click();
	}
}

void CityManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	toggleEditMode(ActionMode::Add);
}

void CityManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected city
		auto editedCity = controller.findCity(row[0].data().toUInt());
		if (editedCity != nullptr) {
			ui.lineEditName->setText(editedCity->getName().c_str());
			toggleEditMode(ActionMode::Modify);
		}
		else {
			showError("Unable to find the selected city.");
		}
	}
}

void CityManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		//Find the selected city
		auto editedCity = controller.findCity(row[0].data().toUInt());
		if (editedCity != nullptr) {
			msgBox.setText(fmt::format("Are you sure you want to delete the city {0}?", editedCity->getName()).c_str());
			msgBox.setWindowTitle("Confirmation");
			msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Cancel);
			if (msgBox.exec() == QMessageBox::Yes) {
				if (controller.deleteCity(editedCity->getId())) {
					refreshItemsTable();
				}
				else {
					showError(controller.getLastError());
				}
			}
		}
		else {
			showError("Unable to find the selected city.");
		}
	}
}

void CityManagementForm::pushButtonOK_Click()
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

void CityManagementForm::saveNewItem()
{
	//Ensure that the new name is available
	string newName = ui.lineEditName->text().trimmed().toStdString();
	if (controller.isNewNameAvailableForAdd(newName)) {
		if (controller.insertCity(City(ui.lineEditName->text().trimmed().toStdString()))) {
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

void CityManagementForm::updateExistingItem()
{
	auto row = ui.tableWidgetItems->selectionModel()->selectedIndexes();
	//Find the selected city
	size_t currentlyEditedCityId = row[0].data().toUInt();
	auto editedCity = controller.findCity(currentlyEditedCityId);
	if (editedCity != nullptr) {
		//Ensure that the new name is available
		string newName = ui.lineEditName->text().trimmed().toStdString();
		if (controller.isNewNameAvailableForUpdate(newName, currentlyEditedCityId)) {
			City editedCityClone { *editedCity };
			editedCityClone.setName(newName);
			if (controller.updateCity(editedCityClone)) {
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
		showError("Unable to find the selected city.");
	}
}

void CityManagementForm::pushButtonCancel_Click()
{
	toggleEditMode(ActionMode::None);
}

bool CityManagementForm::validateEntry() const
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

void CityManagementForm::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape && mode != ActionMode::None) {
		pushButtonCancel_Click();
	}
	else {
		QDialog::keyPressEvent(e);
	}
}

void CityManagementForm::tableWidgetItems_keyPressEvent(int key, int, int) 
{
	if (key == Qt::Key_Delete) {
		pushButtonDelete_Click();
	}
}
