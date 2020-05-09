
#ifndef STUDENTMANAGEMENTNFORM_H
#define STUDENTMANAGEMENTNFORM_H

#include <Qt/qmainwindow.h>
#include <QtGui/QtGui>
#include "ui_studentManagementForm.h"
#include "databaseConnection.h"

class StudentManagementForm : public QDialog
{
Q_OBJECT

public:
	StudentManagementForm(QWidget *parent = 0);
	~StudentManagementForm();
	void showEvent(QShowEvent *event);
	void setDatabaseConnection(const DatabaseConnection &connection);
private:
	Ui::studentManagementFormClass ui;
	const DatabaseConnection * dbConnection;
	QStandardItemModel *tableItemModel;
	bool isInAddMode;
	void refreshItemsTable();
	void toggleTableControls(bool itemSelected);
	void toggleEditMode(bool editMode);
	bool validateEntry() const;
	void showError(const std::string &message) const;
private slots:
	void itemsTableSelectionChanged(const QItemSelection &selected, const QItemSelection &);
	void pushButtonAdd_Click();
	void pushButtonModify_Click();
	void pushButtonDelete_Click();
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
};

#endif // STUDENTMANAGEMENTNFORM_H
