
#ifndef ABOUTBOXFORM_H
#define ABOUTBOXFORM_H

#include "ui_aboutBoxForm.h"

class AboutBoxForm : public QDialog
{
Q_OBJECT

public:
	explicit AboutBoxForm(QWidget *parent = 0);
	~AboutBoxForm() = default;
	void showEvent(QShowEvent *event) override;
private:
	Ui::aboutBoxFormClass ui;
};

#endif // ABOUTBOXFORM_H
