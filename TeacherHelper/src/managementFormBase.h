#pragma once

#include "databaseConnection.h"
#include <string>
#include <QtGui/QtGui>

enum class ActionMode {
    None,
    Add,
    Modifiy
};

class ManagementFormBase
{
public:
    ManagementFormBase();
    virtual ~ManagementFormBase();
	void setDatabaseConnection(const DatabaseConnection &connection);
protected:
	ActionMode mode;
    const DatabaseConnection * dbConnection;
	void showError(const std::string &message) const;
private:
    virtual void refreshItemsTable() = 0;
	virtual void toggleTableControls(bool itemSelected) = 0;
	virtual void toggleEditMode(ActionMode mode) = 0;
	virtual bool validateEntry() const = 0;
};