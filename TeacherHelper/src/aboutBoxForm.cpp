#include "aboutBoxForm.h"
#include "imageAttributionsForm.h"

using namespace std;

AboutBoxForm::AboutBoxForm(QWidget *parent)
	: QDialog(parent),
	  ui(Ui::aboutBoxFormClass())
{
	ui.setupUi(this);
    connect(ui.pushButtonImageAttributions, &QPushButton::clicked, this, &AboutBoxForm::pushButtonImageAttributions_Click);
}


void AboutBoxForm::pushButtonImageAttributions_Click() 
{
	ImageAttributionsForm formImageAttributions(this);
	formImageAttributions.exec();
}