// MainWindow.cpp
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
    /* Titre et taille de la fenêtre */
    setWindowTitle(QString("Hive"));
    resize(1000, 600);

    // création d'un espace pour la mise en page
    auto *layout = new QVBoxLayout(this);

    // Logo HIVE
    auto *logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setPixmap(QPixmap("../assets/hive_logo.png").scaled(200, 200, Qt::KeepAspectRatio));
    layout->addWidget(logoLabel);

    /* Titre */
    auto *titleLabel = new QLabel("Bienvenue dans le jeu Hive", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

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

    auto *buttonLayout = new QHBoxLayout(this);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resumeButton);
    buttonLayout->addWidget(tutorialButton);
    buttonLayout->addWidget(settingsButton);
    buttonLayout->addWidget(launchConsoleButton);
    buttonLayout->addWidget(quitButton);
    layout->addLayout(buttonLayout);

    // Endroit pour afficher les trucs sous les boutons
    stackedWidget = new QStackedWidget(this);
    // Ajout de l'espace pour centrer les éléments
    auto *spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
    layout->addSpacerItem(spacer);
    // Ajouter le QStackedWidget après les boutons
    layout->addWidget(stackedWidget);
    // Widget central avec tous les éléments
    auto *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

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
        "=== Règles du jeu Hive ===\n"
        "\n1. Hive est un jeu de stratégie abstrait où deux joueurs s'affrontent.\n"
        "2. Le but du jeu est de capturer complètement la reine de l'adversaire en l'entourant.\n"
        "3. Chaque joueur possède un ensemble d'insectes avec des mouvements spécifiques :\n"
        "   - La Reine (Queen Bee) : doit être posée dans les 4 premiers tours. Elle bouge d'une case à la fois.\n"
        "   - Les Fourmis (Ants) : peuvent se déplacer n'importe où autour de la ruche.\n"
        "   - Les Araignées (Spiders) : se déplacent exactement de 3 cases.\n"
        "   - Les Scarabées (Beetles) : peuvent grimper sur d'autres pièces.\n"
        "   - Les Sauterelles (Grasshoppers) : sautent en ligne droite par-dessus des pièces.\n"
        "4. Les pièces doivent toujours rester connectées pour former une seule ruche.\n"
        "5. Aucun mouvement ne peut séparer la ruche en plusieurs parties.\n"
        "6. Le premier joueur à entourer complètement la reine de l'adversaire gagne la partie.\n"
        "7. Si les deux reines sont entourées au même moment, c'est une égalité.\n"
        "8. Les joueurs jouent à tour de rôle, en posant une nouvelle pièce ou en déplaçant une pièce déjà posée.\n"
        "\nAmusez-vous bien et bonne chance !";

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
    QLabel *player1Label = new QLabel("Nom du joueur 1:", this);
    QLineEdit *player1NameEdit = new QLineEdit(this);
    player1NameEdit->setPlaceholderText("Entrez le nom du joueur 1");
    QLabel *player2Label = new QLabel("Nom du joueur 2:", this);
    QLineEdit *player2NameEdit = new QLineEdit(this);
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
            nomJ2 = "AI";
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
    // Ajouter ", nomJ1, nomJ2" quand le constructeur sera prêts ce sont des QString
    auto *hive = new hiveRenderer(nullptr, hiveNbRewind, hiveMode, hasLadybug, hasMosquito, load);
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
    // Lancer l'application console avec QProcess
    //QProcess::startDetached("../cmake-build-debug/HiveConsole.exe");
    // QThread *consoleThread = QThread::create([]() {
    //     mainConsole();
    // });
    //
    // consoleThread->start();
    mainConsole();
    FreeConsole();

    // Ferme la fenêtre de la console une fois l'application console lancée
}


void MainWindow::resumeGame() {
    load=true;
    startNewGame();
}

void MainWindow::quitMenu() { this->close(); }
