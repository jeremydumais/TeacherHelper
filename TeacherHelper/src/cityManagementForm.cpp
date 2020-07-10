#include "cityManagementForm.h"
#include <qt5/QtGui/QKeyEvent>
#include <qt5/QtWidgets/qmessagebox.h>
#include <sstream>
#include <boost/algorithm/string.hpp>

using namespace std;

CityManagementForm::CityManagementForm(QWidget *parent, const DatabaseConnection &connection)
	: QDialog(parent),
	  ManagementFormBase(connection),
	  ui(Ui::cityManagementFormClass()),
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
	ui.tableWidgeItems->setColumnHidden(0, true);
	connect(ui.tableWidgeItems->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged(const QItemSelection &)));
	connect(ui.tableWidgeItems, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonModify_Click()));
}

void CityManagementForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	controller.loadCities();
    refreshItemsTable();
} 

void CityManagementForm::refreshItemsTable()
{
	ui.tableWidgeItems->model()->removeRows(0, ui.tableWidgeItems->rowCount());
	size_t row {0};
    for (const auto &city : controller.getCities()) {
		ui.tableWidgeItems->insertRow(row);
		ui.tableWidgeItems->setItem(row, 0, new QTableWidgetItem(to_string(city.getId()).c_str()));
		ui.tableWidgeItems->setItem(row, 1, new QTableWidgetItem(city.getName().c_str()));
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
	ui.tableWidgeItems->setEnabled(!editMode);
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

void CityManagementForm::pushButtonAdd_Click()
{
	ui.lineEditName->clear();
	toggleEditMode(ActionMode::Add);
}

void CityManagementForm::pushButtonModify_Click()
{
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	if (!row.empty()) {
		ui.lineEditName->setText(row[1].data().toString());
		toggleEditMode(ActionMode::Modify);
	}
}

void CityManagementForm::pushButtonDelete_Click()
{
	QMessageBox msgBox;
	stringstream ss;
	auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
	//Find the selected city
	const City *editedCity = controller.findCity(row[0].data().toUInt());
	ss << "Are you sure you want to delete the city " << editedCity->getName() << "?";
	msgBox.setText(ss.str().c_str());
	msgBox.setWindowTitle("Confirmation");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);

	if (msgBox.exec() == QMessageBox::Yes) {

		if (controller.deleteCity(row[0].data().toUInt())) {
			refreshItemsTable();
		}
		else {
			showError(controller.getLastError());
		}
	}
}

void CityManagementForm::pushButtonOK_Click()
{
	if (mode == ActionMode::Add) {
		if (validateEntry()) {
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
	}
	else if (mode == ActionMode::Modify) {
		if (validateEntry()) {
			auto row = ui.tableWidgeItems->selectionModel()->selectedIndexes();
			//Find the selected city
			size_t currentlyEditedCityId = row[0].data().toUInt();
			const City *editedCity = controller.findCity(currentlyEditedCityId);
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
