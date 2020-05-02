#ifndef MAINFORM_H
#define MAINFORM_H

#include <Qt/qmainwindow.h>
#include <QtGui/QtGui>
#include "ui_mainForm.h"

class MainForm : public QMainWindow
{
public:
	MainForm(QWidget *parent = 0);
	~MainForm();
private:
	Ui::MainFormClass ui;
private slots:
};

#endif // MAINFORM_H
