
#ifndef STUDENTMANAGEMENTNFORM_H
#define STUDENTMANAGEMENTNFORM_H

#include "managementFormBase.h"
#include "ui_studentManagementForm.h"

class StudentManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	explicit StudentManagementForm(QWidget *parent = 0);
	~StudentManagementForm();
	void showEvent(QShowEvent *event) override;
private:
	Ui::studentManagementFormClass ui;
	void refreshItemsTable() override;
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected, const QItemSelection &);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
};

#endif // STUDENTMANAGEMENTNFORM_H
