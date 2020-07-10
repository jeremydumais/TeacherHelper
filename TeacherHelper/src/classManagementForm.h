
#ifndef CLASSMANAGEMENTNFORM_H
#define CLASSMANAGEMENTNFORM_H

#include "managementFormBase.h"
#include "ui_classManagementForm.h"
#include "schoolStorage.h"

class ClassManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	ClassManagementForm(QWidget *parent, const DatabaseConnection &connection);
	~ClassManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::classManagementFormClass ui;
	std::list<School> schools;
	void refreshItemsTable() override;
	void refreshSchoolTable();
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	const School* findSchool(size_t id) const;
	bool selectSchoolInEditPanel(size_t id);
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
