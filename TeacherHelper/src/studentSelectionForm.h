#ifndef STUDENTSELECTIONFORM_H
#define STUDENTSELECTIONFORM_H

#include "ui_studentSelectionForm.h"
#include "studentSelectionController.h"

class StudentSelectionForm : public QDialog
{
Q_OBJECT

public:
	StudentSelectionForm(QWidget *parent, const IDatabaseController &databaseController);
	~StudentSelectionForm() = default;
	void showEvent(QShowEvent *event) override;
	void refreshItemsTable(const std::string &filter);
	void toggleTableControls(bool itemSelected);
	const Student *getSelectedStudent() const;
private:
	Ui::studentSelectionFormClass ui;
	StudentSelectionController controller;
	const Student *selectedStudent;
private slots: 
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void lineEditFilterTextChanged(const QString &value);
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
};

#endif // STUDENTSELECTIONFORM_H
