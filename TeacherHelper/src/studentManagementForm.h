
#ifndef STUDENTMANAGEMENTNFORM_H
#define STUDENTMANAGEMENTNFORM_H

#include "managementFormBase.h"
#include "qTableWidgetKeyPressWatcher.h"
#include "studentController.h"
#include "ui_studentManagementForm.h"

class StudentManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	StudentManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~StudentManagementForm();
	void showEvent(QShowEvent *event) override;
private:
	Ui::studentManagementFormClass ui;
	StudentController controller;
	QTableWidgetKeyPressWatcher tableWidgetItemsKeyWatcher;
	void refreshItemsTable() override;
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	void saveNewItem();
	void updateExistingItem();
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected, const QItemSelection &);
	void itemsTableSelectionDoubleClicked(QTableWidgetItem *item);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
	void tableWidgetItems_keyPressEvent(int key, int, int);
};

#endif // STUDENTMANAGEMENTNFORM_H
