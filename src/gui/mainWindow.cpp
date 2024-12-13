// MainWindow.cpp
#include "MainWindow.h"
#include "hiverenderer.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QProcess>
#include <windows.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Titre et taille de la fenêtre principale
    setWindowTitle("Hive Menu");
    resize(1000, 600);

    // Création des boutons
    rulesButton = new QPushButton("Règles du jeu", this);
    secondaryWindowButton = new QPushButton("Ouvrir l'interface graphique", this);
    consoleAppButton = new QPushButton("Lancer l'application console", this);

    // Connexion des boutons aux slots
    connect(rulesButton, &QPushButton::clicked, this, &MainWindow::showGameRules);
    connect(secondaryWindowButton, &QPushButton::clicked, this, &MainWindow::openSecondaryWindow);
    connect(consoleAppButton, &QPushButton::clicked, this, &MainWindow::launchConsoleApp);

    // Message d'accueil
    QLabel *titleLabel = new QLabel("Bienvenue dans le jeu Hive", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);

    // Mise en page principale
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(titleLabel); // Ajout du message d'accueil

    // Disposition horizontale des boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(20); // Espacement entre les boutons
    // Ajustement des tailles des boutons
    rulesButton->setFixedSize(250, 50);
    secondaryWindowButton->setFixedSize(250, 50);
    consoleAppButton->setFixedSize(250, 50);
    // Ajout des boutons à la disposition
    buttonLayout->addWidget(rulesButton); // Bouton pour afficher les règles
    buttonLayout->addWidget(secondaryWindowButton); // Bouton pour l'interface graphique
    buttonLayout->addWidget(consoleAppButton); // Bouton pour l'application console
    layout->addLayout(buttonLayout);

    // Ajout d'un espace pour centrer les trucs
    QSpacerItem *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addSpacerItem(spacer);

    // Configuration du widget central
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
    this->close();
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

void MainWindow::showGameRules() {
    // Création d'une nouvelle fenêtre pour afficher les règles du jeu
    QWidget *rulesWindow = new QWidget;
    rulesWindow->setWindowTitle("Règles du jeu");
    rulesWindow->setFixedSize(1000, 600);

    // Mise en page
    QVBoxLayout *layout = new QVBoxLayout;

    // Les règles (à écrire)
    QLabel *rulesLabel = new QLabel("Bla bla, jsp quoi écrire", rulesWindow);
    rulesLabel->setAlignment(Qt::AlignCenter);
    QFont rulesFont = rulesLabel->font();
    rulesFont.setPointSize(16);
    rulesLabel->setFont(rulesFont);
    layout->addWidget(rulesLabel);

    // Bouton de retour pour revenir à la fenêtre principale
    QPushButton *backButton = new QPushButton("Retour au menu", rulesWindow);
    backButton->setFixedSize(200, 50); // Ajuste la taille du bouton
    connect(backButton, &QPushButton::clicked, [this, rulesWindow]() {
        rulesWindow->close();
        this->show();
    });
    layout->addWidget(backButton);
    layout->setAlignment(backButton, Qt::AlignCenter);

    rulesWindow->setLayout(layout);
    rulesWindow->show();

    // Cache temporairement la fenêtre principale
    this->hide();
}
