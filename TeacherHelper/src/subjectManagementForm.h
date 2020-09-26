
#ifndef SUBJECTMANAGEMENTFORM_H
#define SUBJECTMANAGEMENTFORM_H

#include "managementFormBase.h"
#include "qTableWidgetKeyPressWatcher.h"
#include "subjectController.h"
#include "ui_subjectManagementForm.h"

class SubjectManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	SubjectManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~SubjectManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::subjectManagementFormClass ui;
	SubjectController controller;
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

#endif // SUBJECTMANAGEMENTFORM_H
