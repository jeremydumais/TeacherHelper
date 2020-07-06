
#ifndef CITYMANAGEMENTFORM_H
#define CITYMANAGEMENTFORM_H

#include "managementFormBase.h"
#include "ui_cityManagementForm.h"
#include "city.h"

class CityManagementForm : public QDialog, public ManagementFormBase
{
Q_OBJECT

public:
	explicit CityManagementForm(QWidget *parent = 0);
	~CityManagementForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::cityManagementFormClass ui;
	std::list<City> cities;
	void refreshItemsTable() override;
	void toggleTableControls(bool itemSelected) override;
	void toggleEditMode(ActionMode mode) override;
	bool validateEntry() const override;
	const City *findCity(size_t id);
	bool isNewNameAvailable(const std::string &name, 
						 size_t currentlyEditedCityId, 
						 ActionMode mode) const;
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
