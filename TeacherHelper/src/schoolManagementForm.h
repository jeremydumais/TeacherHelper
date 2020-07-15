
#ifndef SCHOOLMANAGEMENTNFORM_H
#define SCHOOLMANAGEMENTNFORM_H

#include "managementFormBase.h"
#include "ui_schoolManagementForm.h"
#include "schoolController.h"
#include "cityController.h"

class SchoolManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	SchoolManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~SchoolManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::schoolManagementFormClass ui;
	SchoolController controller;
	CityController cityController;
	void refreshItemsTable() override;
	void refreshCityTable();
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	bool selectCityInEditPanel(size_t id);
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
};

#endif // SCHOOLMANAGEMENTNFORM_H
