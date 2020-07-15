
#ifndef CITYMANAGEMENTFORM_H
#define CITYMANAGEMENTFORM_H

#include "managementFormBase.h"
#include "ui_cityManagementForm.h"
#include "cityController.h"

class CityManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	CityManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~CityManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::cityManagementFormClass ui;
	CityController controller;
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

#endif // CITYMANAGEMENTFORM_H
