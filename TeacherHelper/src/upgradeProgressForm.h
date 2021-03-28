
#ifndef UPGRADEPROGRESSFORM_H
#define UPGRADEPROGRESSFORM_H

#include "ui_upgradeProgressForm.h"
#include <string>

class UpgradeProgressForm : public QDialog
{
Q_OBJECT

public:
	explicit UpgradeProgressForm(QWidget *parent = 0);
	~UpgradeProgressForm() = default;
private:
	Ui::upgradeProgressFormClass ui;
public:
	void setProgress(size_t progress);
	void addMessage(const std::string &message);
	void enabledFinishButton();
private slots:
	void pushButtonFinish_Click();
};

#endif // UPGRADEPROGRESSFORM_H
