#include "mainForm.h"

MainForm::MainForm(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
    connect(ui.action_Quit, SIGNAL(triggered()), this, SLOT(close()));
}

MainForm::~MainForm()
{
}