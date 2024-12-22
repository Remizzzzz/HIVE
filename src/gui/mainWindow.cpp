#include "MainWindow.h"
#include "hiverenderer.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QProcess>
#include <windows.h>
#include <QLineEdit>
#include <QThread>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include "../mainConsole.cpp"
#include <iostream>



//Fonction pour le lancement console
void enableVirtualTerminalProcessing() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (hOut == INVALID_HANDLE_VALUE) return;

    if (!GetConsoleMode(hOut, &dwMode)) return;

    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {
    //Titre et taille de la fenêtre
    setWindowTitle(QString("Hive"));
    resize(1000, 600);

    // Widget central avec tous les éléments
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // création d'un espace pour la mise en page
    auto *mainLayout = new QVBoxLayout();
    centralWidget->setLayout(mainLayout);

    // Logo HIVE
    auto *logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setPixmap(QPixmap("../assets/hive_logo.png").scaled(200, 200, Qt::KeepAspectRatio));
    mainLayout->addWidget(logoLabel);

    /* Titre */
    auto *titleLabel = new QLabel("Bienvenue dans le jeu Hive", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    /* Création des boutons */
    startButton = new QPushButton(QString("Démarrer une nouvelle partie"), this);
    resumeButton = new QPushButton(QString("Charger la dernière partie sauvegardée"), this);
    tutorialButton = new QPushButton(QString("Tutoriel"), this);
    settingsButton = new QPushButton(QString("Changer les paramètres"), this);
    launchConsoleButton = new QPushButton(QString("Menu Console"), this);
    quitButton = new QPushButton(QString("Quitter"), this);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startNewGame);
    connect(resumeButton, &QPushButton::clicked, this, &MainWindow::resumeGame);
    connect(tutorialButton, &QPushButton::clicked, this, &MainWindow::showTutorial);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::changeSettings);
    connect(launchConsoleButton, &QPushButton::clicked, this, &MainWindow::launchConsoleApp);
    connect(quitButton, &QPushButton::clicked, this, &MainWindow::quitMenu);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resumeButton);
    buttonLayout->addWidget(tutorialButton);
    buttonLayout->addWidget(settingsButton);
    buttonLayout->addWidget(launchConsoleButton);
    buttonLayout->addWidget(quitButton);
    mainLayout->addLayout(buttonLayout);

    // Endroit pour afficher les trucs sous les boutons
    stackedWidget = new QStackedWidget();
    // Ajout de l'espace pour centrer les éléments
    auto *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    mainLayout->addSpacerItem(spacer);
    // Ajouter le QStackedWidget après les boutons
    mainLayout->addWidget(stackedWidget);


    // Initialisation des vues
    initializeEmptyWidget();
    initializeTutorialWidget();
    initializeSettingsWidget();
}

void MainWindow::initializeEmptyWidget() {
    auto *emptyWidget = new QWidget(stackedWidget);
    stackedWidget->addWidget(emptyWidget);
}

void MainWindow::initializeTutorialWidget() {
    auto *tutorialWidget = new QWidget(this);

    // Texte des règles
    QString rulesText =
        "\n=== Règles du jeu Hive ===\n\n"

        "Objectif du jeu\n"
        "-----------------\n"
        "Chaque joueur doit utiliser ses insectes pour encercler complètement l'abeille reine de son adversaire. "
        "Le premier joueur à capturer l'abeille ennemie remporte la partie.\n\n"

        "Règles générales\n"
        "-------------------\n"
        "1. Les pièces doivent toujours rester connectées. Vous ne pouvez pas diviser la ruche en deux parties.\n"
        "2. Chaque joueur doit poser son abeille reine sur le plateau dans les 4 premiers tours.\n"
        "3. Les mouvements des insectes doivent respecter les limites physiques de la ruche :\n"
        "   - Un insecte ne peut pas passer dans un espace trop étroit (règle du \"glissement\").\n"
        "   - Certains insectes ont des mouvements uniques, décrits ci-dessous.\n\n"

        "Caractéristiques des insectes\n"
        "--------------------------------\n"
        "• Reine (Abeille) : Se déplace d'une seule case par tour. "
        "Elle est la pièce la plus importante. Si elle est complètement encerclée, vous perdez.\n"

        "• Fourmi : Se déplacer d'autant de cases que souhaité autour de la ruche. "
        "Très utile pour la mobilité et la stratégie.\n"

        "• Scarabée : Se déplace d'une case comme la reine, mais peut grimper sur d'autres pièces. "
        "Lorsqu'il grimpe, il bloque la pièce en dessous et la rend inutilisable.\n"

        "• Sauterelle : Saute en ligne droite au-dessus d'une ou plusieurs pièces adjacentes. "
        "Ne peut pas s'arrêter sur des pièces, seulement sur des espaces vides.\n"

        "• Araignée : Se déplacer exactement de trois cases. "
        "Ne peut pas revenir en arrière ou changer de direction avant d'avoir atteint trois cases.\n"

        "Bon jeu !\n";

    // Mise en page
    auto *tutorialLayout = new QVBoxLayout;
    auto *tutorialLabel = new QLabel(rulesText, tutorialWidget);
    tutorialLabel->setAlignment(Qt::AlignCenter);
    tutorialLayout->addWidget(tutorialLabel);
    tutorialWidget->setLayout(tutorialLayout);
    stackedWidget->addWidget(tutorialWidget);
}

void MainWindow::initializeSettingsWidget() {
    auto *settingsWidget = new QWidget(this);
    auto *settingsLayout = new QVBoxLayout(settingsWidget);
    settingsWidget->setFixedWidth(450);

    // Titre
    auto *titleLabel = new QLabel("Modifier les paramètres du jeu", settingsWidget);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    settingsLayout->addWidget(titleLabel);

    // Nom des joueurs
    auto *player1Label = new QLabel("Nom du joueur 1:", this);
    auto *player1NameEdit = new QLineEdit(this);
    player1NameEdit->setPlaceholderText("Entrez le nom du joueur 1");
    auto *player2Label = new QLabel("Nom du joueur 2:", this);
    auto *player2NameEdit = new QLineEdit(this);
    player2NameEdit->setPlaceholderText("Entrez le nom du joueur 2 (IA par défaut si PvAI)");
    settingsLayout->addWidget(player1Label);
    settingsLayout->addWidget(player1NameEdit);
    settingsLayout->addWidget(player2Label);
    settingsLayout->addWidget(player2NameEdit);

    // Mode de jeu
    auto *modeLabel = new QLabel("Choisir le mode de jeu", settingsWidget);
    auto *modeComboBox = new QComboBox(settingsWidget);
    modeComboBox->addItem("Joueur vs Joueur (PvP)", 0);
    modeComboBox->addItem("Joueur vs IA (PvAI)", 1);
    modeComboBox->setCurrentIndex(modeComboBox->findData(mode));
    settingsLayout->addWidget(modeLabel);
    settingsLayout->addWidget(modeComboBox);

    // Niveau de l'IA
    auto *levelLabel = new QLabel("Choisir le niveau de l'IA (PvAI uniquement)", settingsWidget);
    auto *levelComboBox = new QComboBox(settingsWidget);
    levelComboBox->addItem("Niveau aléatoire", 0);
    /* Pour ajouter d'autres IA c'est ici */
    levelComboBox->setCurrentIndex(levelComboBox->findData(levelIA));
    settingsLayout->addWidget(levelLabel);
    settingsLayout->addWidget(levelComboBox);

    // Extensions
    auto *extensionsLabel = new QLabel("Activer/Désactiver les extensions", settingsWidget);
    auto *ladybugCheckBox = new QCheckBox("Activer l'extension Ladybug", settingsWidget);
    auto *mosquitoCheckBox = new QCheckBox("Activer l'extension Mosquito", settingsWidget);
    ladybugCheckBox->setCheckState(hasLadybug ? Qt::Checked : Qt::Unchecked);
    mosquitoCheckBox->setCheckState(hasMosquito ? Qt::Checked : Qt::Unchecked);
    settingsLayout->addWidget(extensionsLabel);
    settingsLayout->addWidget(ladybugCheckBox);
    settingsLayout->addWidget(mosquitoCheckBox);

    // Nb rewinds
    auto *rewindLabel = new QLabel("Modifier le nombre de rewinds (0 à 10)", settingsWidget);
    auto *rewindSpinBox = new QSpinBox(settingsWidget);
    rewindSpinBox->setRange(0, 10);
    rewindSpinBox->setValue(hiveNbRewind);
    settingsLayout->addWidget(rewindLabel);
    settingsLayout->addWidget(rewindSpinBox);

    // Bouton de confirmation (pour mettre à jour les paramètres)
    auto *applyButton = new QPushButton("Appliquer", settingsWidget);
    settingsLayout->addWidget(applyButton);

    // Mettre à jour les paramètres
    connect(applyButton, &QPushButton::clicked, [this, modeComboBox, ladybugCheckBox, mosquitoCheckBox,
        rewindSpinBox, player1NameEdit, player2NameEdit]() {

        hiveNbRewind = rewindSpinBox->value();
        nomJ1 = player1NameEdit->text();

        if (modeComboBox->currentIndex() == 0) {
            hiveMode = PvP;
            nomJ2 = player2NameEdit->text();
        }
        else {
            hiveMode = PvAI;
            nomJ2 = "IA";
        }

        if (ladybugCheckBox->isChecked()) hasLadybug = true;
        else hasLadybug = false;
        if (mosquitoCheckBox->isChecked()) hasMosquito = true;
        else hasMosquito = false;

        stackedWidget->setCurrentIndex(0);
    });

    // Ajouter le widget dans la stackedWidget
    stackedWidget->addWidget(settingsWidget);
}



void MainWindow::showTutorial() {
    // Afficher le tutoriel sous les boutons
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::changeSettings() {
    // Afficher les paramètres sous les boutons
    stackedWidget->setCurrentIndex(2);  // Afficher le widget des paramètres
}

void MainWindow::startNewGame() {
    auto *hive = new hiveRenderer(nullptr, hiveNbRewind, hiveMode, hasLadybug, hasMosquito, load, nomJ1, nomJ2);
    hive->setStyleSheet("background-color: black;");
    hive->show();
    this->close();
}

void MainWindow::launchConsoleApp() {
    // Lance une fenêtre de console vide
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);  // Redirige l'entrée standard vers la console
    freopen("CONOUT$", "w", stderr);  // Redirige la sortie d'erreur standard vers la console
    enableVirtualTerminalProcessing();
    std::cout << "Console ouverte... Lancement de l'application console..." << std::endl;
    this->close();

    mainConsole();
    FreeConsole();
}


void MainWindow::resumeGame() {
    load=true;
    startNewGame();
}

void MainWindow::quitMenu() { this->close(); }
