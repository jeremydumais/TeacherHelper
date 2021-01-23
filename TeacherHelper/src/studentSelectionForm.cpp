#include "studentSelectionForm.h"
#include <QtGui/QKeyEvent>
#include <QtWidgets/qmessagebox.h>
#include <fmt/format.h>
#include <boost/algorithm/string.hpp>

using namespace std;

StudentSelectionForm::StudentSelectionForm(QWidget *parent, const IDatabaseController &databaseController)
	: QDialog(parent),
	  ui(Ui::studentSelectionFormClass()),
	  controller(databaseController),
	  selectedStudent(vector<const Student *>())
{
	ui.setupUi(this);
	connect(ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(pushButtonOK_Click()));
	connect(ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(pushButtonCancel_Click()));

	ui.tableWidgetStudents->setColumnHidden(0, true);
	connect(ui.tableWidgetStudents->selectionModel(), 
		SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
  		SLOT(itemsTableSelectionChanged()));
	connect(ui.tableWidgetStudents, 
		SIGNAL(cellDoubleClicked(int, int)), 
		SLOT(pushButtonOK_Click()));
	connect(ui.lineEditFilter,
		SIGNAL(textChanged(const QString &)),
		SLOT(lineEditFilterTextChanged(const QString &)));	
}

void StudentSelectionForm::showEvent(QShowEvent *event) 
{
    QDialog::showEvent(event);
	setWindowIcon(QIcon(":/global/student64.png"));
	controller.loadStudents();
	refreshItemsTable("");
	ui.lineEditFilter->setFocus();
}

void StudentSelectionForm::refreshItemsTable(const string &filter)
{
	ui.tableWidgetStudents->model()->removeRows(0, ui.tableWidgetStudents->rowCount());
	size_t row {0};
    for (const auto &itemStudent : controller.getStudents()) {
		if (controller.isStudentInFilter(filter, itemStudent)) {
			string comments = boost::trim_copy(itemStudent.getComments());
			if (!boost::empty(comments)) {
				comments = " (" + comments + ")";
			}
			ui.tableWidgetStudents->insertRow(row);
			ui.tableWidgetStudents->setItem(row, 0, new QTableWidgetItem(to_string(itemStudent.getId()).c_str()));
			ui.tableWidgetStudents->setItem(row, 1, new QTableWidgetItem(fmt::format("{0}, {1}{2}", 
																		itemStudent.getLastName(), 
																		itemStudent.getFirstName(), 
																		comments).c_str()));
			row++;
		}
    }
	//If there is only one student left in the filter, select it
	if (row == 1) {
		ui.tableWidgetStudents->selectRow(0);
		toggleTableControls(true);
	} 
	else {
		toggleTableControls(false);
	}
}

void StudentSelectionForm::toggleTableControls(bool itemSelected)
{
	ui.pushButtonOK->setEnabled(itemSelected);
}

const vector<const Student *> StudentSelectionForm::getSelectedStudent() const
{
	return selectedStudent;
}

void StudentSelectionForm::itemsTableSelectionChanged()
{	
	toggleTableControls(ui.tableWidgetStudents->selectionModel()->selectedIndexes().size() >= 1);
}

void StudentSelectionForm::lineEditFilterTextChanged(const QString &value)
{
	//Filter the list with the text value
	refreshItemsTable(boost::to_upper_copy(value.toStdString()));
}

void StudentSelectionForm::pushButtonOK_Click()
{
	selectedStudent.clear();
	auto rows = ui.tableWidgetStudents->selectionModel()->selectedIndexes();
	if (rows.size() > 0) {
		for(const auto &row : rows) {
			const Student *student { controller.findStudent(row.data().toUInt()) };
			if (student != nullptr) {
				selectedStudent.push_back(student);
			}
		}
		close();
	}
}

void StudentSelectionForm::pushButtonCancel_Click()
{
	selectedStudent.clear();
	close();
}