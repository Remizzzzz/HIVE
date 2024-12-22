#include <QApplication>
#include <QDebug>

#include "gui/mainWindow.h"



int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return QApplication::exec(); // return a.exec() warning on a -> using static method
}
