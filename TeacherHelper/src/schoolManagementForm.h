#ifndef SCHOOLMANAGEMENTNFORM_H
#define SCHOOLMANAGEMENTNFORM_H

#include "cityController.h"
#include "IDatabaseController.h"
#include "schoolController.h"
#include "managementFormBase.h"
#include "qTableWidgetKeyPressWatcher.h"
#include "ui_schoolManagementForm.h"

class SchoolManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	SchoolManagementForm(QWidget *parent, const IDatabaseController &databaseController);
	~SchoolManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::schoolManagementFormClass ui;
	SchoolController controller;
	CityController cityController;
	QTableWidgetKeyPressWatcher tableWidgetItemsKeyWatcher;
	void refreshItemsTable() override;
	void refreshCityTable();
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	void saveNewItem(const City* const selectedCity);
	void updateExistingItem(const City* const selectedCity);
	bool selectCityInEditPanel(size_t id);
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void itemsTableSelectionDoubleClicked(QTableWidgetItem *item);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
	void tableWidgetItems_keyPressEvent(int key, int, int);
};

#endif // SCHOOLMANAGEMENTNFORM_H
