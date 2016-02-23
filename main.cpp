#include <QApplication>
#include "visualconfig.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    VisualConfig w;
    QCoreApplication::setOrganizationName("MYTH Inc");
    QCoreApplication::setApplicationName("waterer");
    w.show();

    return a.exec();
}
