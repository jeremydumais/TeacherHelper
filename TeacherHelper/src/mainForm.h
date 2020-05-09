#ifndef MAINFORM_H
#define MAINFORM_H

#include <Qt/qmainwindow.h>
#include <QtGui/QtGui>
#include "ui_mainForm.h"
#include "databaseConnection.h"

class MainForm : public QMainWindow
{
public:
	MainForm(QWidget *parent = 0);
	~MainForm();
private:
	Ui::MainFormClass ui;
	DatabaseConnection *dbConnection;
	//static int callback(void *data, int argc, char **argv, char **azColName);
private slots:
};

#endif // MAINFORM_H
