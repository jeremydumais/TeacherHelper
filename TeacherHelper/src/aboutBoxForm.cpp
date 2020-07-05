#include "aboutBoxForm.h"

using namespace std;

AboutBoxForm::AboutBoxForm(QWidget *parent)
	: QDialog(parent),
	  ui(Ui::aboutBoxFormClass())
{
	ui.setupUi(this);
}

void AboutBoxForm::showEvent(QShowEvent *event) 
{
} 
