
#ifndef CITYMANAGEMENTFORM_H
#define CITYMANAGEMENTFORM_H

#include "cityController.h"
#include "managementFormBase.h"
#include "qTableWidgetKeyPressWatcher.h"
#include "ui_cityManagementForm.h"

class CityManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	CityManagementForm(QWidget *parent, const IDatabaseController &databaseController);
	~CityManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::cityManagementFormClass ui;
	CityController controller;
	QTableWidgetKeyPressWatcher tableWidgetItemsKeyWatcher;
	void refreshItemsTable() override;
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	void saveNewItem();
	void updateExistingItem();
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

#endif // CITYMANAGEMENTFORM_H
