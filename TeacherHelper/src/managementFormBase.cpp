#include "managementFormBase.h"
#include <QtWidgets/qmessagebox.h>

using namespace std;

ManagementFormBase::ManagementFormBase(const IDatabaseConnection &connection)
    : mode(ActionMode::None),
      dbConnection(&connection),
	  dataHasChanged(false)
{
}

ManagementFormBase::~ManagementFormBase()
{
}

void ManagementFormBase::setDatabaseConnection(const IDatabaseConnection &connection)
{
	this->dbConnection = &connection;
}

bool ManagementFormBase::getDataHasChanged() const
{
	return dataHasChanged;
} 

void ManagementFormBase::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}