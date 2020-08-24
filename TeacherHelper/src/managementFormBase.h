#pragma once

#include "databaseConnection.h"
#include <string>

enum class ActionMode {
    None,
    Add,
    Modify
};

class ManagementFormBase
{
public:
    ManagementFormBase(const DatabaseConnection &connection);
    virtual ~ManagementFormBase();
	void setDatabaseConnection(const DatabaseConnection &connection);
	virtual bool getDataHasChanged() const;
protected:
	ActionMode mode;
    const DatabaseConnection * dbConnection;
	bool dataHasChanged;
	void showError(const std::string &message) const;
private:
    virtual void refreshItemsTable() = 0;
	virtual void toggleTableControls(bool itemSelected) = 0;
	virtual void toggleEditMode(ActionMode mode) = 0;
	virtual bool validateEntry() const = 0;
};