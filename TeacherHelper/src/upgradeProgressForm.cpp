#include "upgradeProgressForm.h"

using namespace std;

UpgradeProgressForm::UpgradeProgressForm(QWidget *parent)
	: QDialog(parent),
	  ui(Ui::upgradeProgressFormClass())
{
	ui.setupUi(this);
    connect(ui.pushButtonFinish, &QPushButton::clicked, this, &UpgradeProgressForm::pushButtonFinish_Click);
	setWindowIcon(QIcon(":/global/TeacherHelper Icon256.png"));
}

void UpgradeProgressForm::setProgress(size_t progress) 
{
	ui.progressBar->setValue(static_cast<int>(progress));
}

void UpgradeProgressForm::addMessage(const std::string &message) 
{
	ui.plainTextEditMessages->appendPlainText(message.c_str());
}

void UpgradeProgressForm::enabledFinishButton() 
{
	ui.pushButtonFinish->setEnabled(true);
}

void UpgradeProgressForm::pushButtonFinish_Click() 
{
	close();
}