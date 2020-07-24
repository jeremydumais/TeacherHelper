
#ifndef SUBJECTMANAGEMENTFORM_H
#define SUBJECTMANAGEMENTFORM_H

#include "managementFormBase.h"
#include "ui_subjectManagementForm.h"
#include "subjectController.h"

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

#endif // SUBJECTMANAGEMENTFORM_H
