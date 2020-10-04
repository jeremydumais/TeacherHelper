#include "imageAttributionsForm.h"

using namespace std;

ImageAttributionsForm::ImageAttributionsForm(QWidget *parent)
	: QDialog(parent),
	  ui(Ui::imageAttributionsFormClass())
{
	ui.setupUi(this);
	setWindowIcon(QIcon(":/global/TeacherHelper Icon256.png"));
}