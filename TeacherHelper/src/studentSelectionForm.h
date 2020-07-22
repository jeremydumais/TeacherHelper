#ifndef STUDENTSELECTIONFORM_H
#define STUDENTSELECTIONFORM_H

#include "ui_studentSelectionForm.h"
#include "studentController.h"

class StudentSelectionForm : public QDialog
{
Q_OBJECT

public:
	StudentSelectionForm(QWidget *parent, const DatabaseConnection &connection);
	~StudentSelectionForm() = default;
	void showEvent(QShowEvent *event) override;
	void refreshItemsTable(const std::string &filter);
	void toggleTableControls(bool itemSelected);
	const Student *getSelectedStudent() const;
private:
	Ui::studentSelectionFormClass ui;
	StudentController controller;
	const Student *selectedStudent;
	bool isStudentInFilter(const std::string &filter, const Student &student) const;
private slots: 
	void itemsTableSelectionChanged(const QItemSelection &selected);
	void lineEditFilterTextChanged(const QString &value);
	void pushButtonOK_Click();
	void pushButtonCancel_Click();
};

#endif // STUDENTSELECTIONFORM_H
