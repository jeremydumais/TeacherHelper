
#ifndef TESTYPEMANAGEMENTFORM_H
#define TESTYPEMANAGEMENTFORM_H

#include "managementFormBase.h"
#include "qTableWidgetKeyPressWatcher.h"
#include "testTypeController.h"
#include "ui_testTypeManagementForm.h"

class TestTypeManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	TestTypeManagementForm(QWidget *parent, const IDatabaseController &databaseController);
	~TestTypeManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::testTypeManagementFormClass ui;
	TestTypeController controller;
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

#endif // TESTYPEMANAGEMENTFORM_H
