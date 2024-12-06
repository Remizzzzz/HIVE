#include "hiverenderer.h"

#include <QApplication>

int mainQt(int argc, char *argv[])
{
    QApplication a(argc, argv);
    hiveRenderer w;
    w.show();
    return a.exec();
}
