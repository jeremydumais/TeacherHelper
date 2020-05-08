#include <Qt/qapplication.h>
#include <stdio.h>
#include "mainForm.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Jed# Software");
    QCoreApplication::setApplicationName("Teacher Helper");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    MainForm mainWin;

    mainWin.show();
    return app.exec();
}