// MainWindow.cpp
#include "MainWindow.h"
#include "hiverenderer.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QProcess>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Création des boutons
    secondaryWindowButton = new QPushButton("Ouvrir l'interface graphique", this);
    consoleAppButton = new QPushButton("Lancer l'application console", this);

    // Connexion des boutons aux slots
    connect(secondaryWindowButton, &QPushButton::clicked, this, &MainWindow::openSecondaryWindow);
    connect(consoleAppButton, &QPushButton::clicked, this, &MainWindow::launchConsoleApp);

    // Layout pour afficher les boutons
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(secondaryWindowButton);
    layout->addWidget(consoleAppButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::openSecondaryWindow()
{
    // Créer et afficher la fenêtre secondaire
    hiveRenderer *hive = new hiveRenderer();
    hive->show();
}

void MainWindow::launchConsoleApp()
{
    // Lance une fenêtre de console vide
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Console ouverte... Lancement de l'application console..." << std::endl;

    // Lancer l'application console avec QProcess
    QProcess::startDetached("../cmake-build-debug/HiveConsole.exe");

    // Ferme la fenêtre de la console une fois l'application console lancée
    FreeConsole();
    this->close();
}
