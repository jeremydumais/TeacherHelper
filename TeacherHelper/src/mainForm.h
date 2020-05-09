#ifndef MAINFORM_H
#define MAINFORM_H

#include <Qt/qmainwindow.h>
#include <QtGui/QtGui>
#include "ui_mainForm.h"
#include "databaseConnection.h"

class MainForm : public QMainWindow
{
Q_OBJECT

public:
	MainForm(QWidget *parent = 0);
	~MainForm();
private:
	Ui::MainForm ui;
	DatabaseConnection *dbConnection;
private slots:
	void action_StudentsManagement_Click();
};

#endif // MAINFORM_H

