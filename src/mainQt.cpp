#include <QApplication>
#include <QPushButton>
#include "gui/hiverenderer.h"
#include "gui/mainWindow.h"
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
