/********************************************************************************
** Form generated from reading UI file 'mainFormb28546.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINFORMB28546_H
#define MAINFORMB28546_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainFormClass
{
public:
    QAction *action_Quit;
    QWidget *centralwidget;
    QMenuBar *menubar;
    QMenu *menu_File;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainFormClass)
    {
        if (MainFormClass->objectName().isEmpty())
            MainFormClass->setObjectName(QString::fromUtf8("MainFormClass"));
        MainFormClass->resize(800, 600);
        action_Quit = new QAction(MainFormClass);
        action_Quit->setObjectName(QString::fromUtf8("action_Quit"));
        centralwidget = new QWidget(MainFormClass);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        MainFormClass->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainFormClass);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menu_File = new QMenu(menubar);
        menu_File->setObjectName(QString::fromUtf8("menu_File"));
        MainFormClass->setMenuBar(menubar);
        statusbar = new QStatusBar(MainFormClass);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        MainFormClass->setStatusBar(statusbar);
        toolBar = new QToolBar(MainFormClass);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        MainFormClass->addToolBar(Qt::TopToolBarArea, toolBar);

        menubar->addAction(menu_File->menuAction());
        menu_File->addAction(action_Quit);

        retranslateUi(MainFormClass);

        QMetaObject::connectSlotsByName(MainFormClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainFormClass)
    {
        MainFormClass->setWindowTitle(QApplication::translate("MainFormClass", "Teacher Helper", 0, QApplication::UnicodeUTF8));
        action_Quit->setText(QApplication::translate("MainFormClass", "&Quit", 0, QApplication::UnicodeUTF8));
        menu_File->setTitle(QApplication::translate("MainFormClass", "&File", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("MainFormClass", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainFormClass: public Ui_MainFormClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINFORMB28546_H
