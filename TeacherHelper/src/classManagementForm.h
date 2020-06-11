
#ifndef CLASSMANAGEMENTNFORM_H
#define CLASSMANAGEMENTNFORM_H

#include "managementFormBase.h"
#include "ui_classManagementForm.h"

class ClassManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	explicit ClassManagementForm(QWidget *parent = 0);
	~ClassManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::classManagementFormClass ui;
	void refreshItemsTable() override;
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
};

#endif // CLASSMANAGEMENTNFORM_H
