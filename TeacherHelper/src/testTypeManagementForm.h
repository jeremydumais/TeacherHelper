
#ifndef TESTYPEMANAGEMENTFORM_H
#define TESTYPEMANAGEMENTFORM_H

#include "managementFormBase.h"
#include "ui_testTypeManagementForm.h"
#include "testTypeController.h"

class TestTypeManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	TestTypeManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~TestTypeManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::testTypeManagementFormClass ui;
	TestTypeController controller;
	void refreshItemsTable() override;
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	void saveNewItem();
	void updateExistingItem();
private slots: 
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
};

#endif // TESTYPEMANAGEMENTFORM_H
