#include "managementFormBase.h"
#include <qt5/QtWidgets/qmessagebox.h>

using namespace std;

ManagementFormBase::ManagementFormBase(const DatabaseConnection &connection)
    : mode(ActionMode::None),
      dbConnection(&connection)
{
}

ManagementFormBase::~ManagementFormBase()
{
}

void ManagementFormBase::setDatabaseConnection(const DatabaseConnection &connection)
{
	this->dbConnection = &connection;
}

void ManagementFormBase::showError(const string &message) const
{
	QMessageBox msgBox;
	msgBox.setText(QString(message.c_str()));
	msgBox.setWindowTitle("Error");
	msgBox.setIcon(QMessageBox::Icon::Critical);
	msgBox.exec();
}