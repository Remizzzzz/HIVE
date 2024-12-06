/********************************************************************************
** Form generated from reading UI file 'hiverenderer.ui'
**
** Created by: Qt User Interface Compiler version 6.8.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HIVERENDERER_H
#define UI_HIVERENDERER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_hiveRenderer
{
public:
    QWidget *centralwidget;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *hiveRenderer)
    {
        if (hiveRenderer->objectName().isEmpty())
            hiveRenderer->setObjectName("hiveRenderer");
        hiveRenderer->resize(800, 600);
        centralwidget = new QWidget(hiveRenderer);
        centralwidget->setObjectName("centralwidget");
        hiveRenderer->setCentralWidget(centralwidget);
        menubar = new QMenuBar(hiveRenderer);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        hiveRenderer->setMenuBar(menubar);
        statusbar = new QStatusBar(hiveRenderer);
        statusbar->setObjectName("statusbar");
        hiveRenderer->setStatusBar(statusbar);

        retranslateUi(hiveRenderer);

        QMetaObject::connectSlotsByName(hiveRenderer);
    } // setupUi

    void retranslateUi(QMainWindow *hiveRenderer)
    {
        hiveRenderer->setWindowTitle(QCoreApplication::translate("hiveRenderer", "hiveRenderer", nullptr));
    } // retranslateUi

};

namespace Ui {
    class hiveRenderer: public Ui_hiveRenderer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HIVERENDERER_H
