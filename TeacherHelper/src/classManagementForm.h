
#ifndef CLASSMANAGEMENTNFORM_H
#define CLASSMANAGEMENTNFORM_H

#include "managementFormBase.h"
#include "ui_classManagementForm.h"
#include "classController.h"
#include "schoolController.h"
#include "studentController.h"

class ClassManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	ClassManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~ClassManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::classManagementFormClass ui;
	ClassController controller;
	SchoolController schoolController;
	StudentController studentController;
	void refreshItemsTable() override;
	void refreshSchoolTable();
	void toggleTableControls(bool itemSelected) override;
	void toggleMembersTableControls(bool itemSelected);
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	void saveNewItem(const School* const selectedSchool);
	void updateExistingItem(const School* const selectedSchool);
	bool selectSchoolInEditPanel(size_t id);
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void membersTableSelectionChanged(const QItemSelection &selected);
	void pushButtonAdd_Click();
	void pushButtonDuplicate_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
	void keyPressEvent(QKeyEvent *e) override;
	void pushButtonAddMember_Click();
	void pushButtonRemoveMember_Click();
};

#endif // CLASSMANAGEMENTNFORM_H