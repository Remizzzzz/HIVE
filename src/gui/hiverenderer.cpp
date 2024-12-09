#include "hiverenderer.h"
#include "hexagonalbutton.h"
#include "./ui_hiverenderer.h"
#include <QPixmap>
#include <QIcon>
//#include "hive.h" -> Déclenche ENORMEMENT d'erreur
hiveRenderer::hiveRenderer(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    infoLabel(new QLabel("Cliquez sur un bouton", this)),
    ui(new Ui::hiveRenderer)
{ui->setupUi(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setGeometry(0, 0, width(), 30);  // Placer le label en haut
    centralWidget->setGeometry(0, 50, width(), height() - 50);  // Ajuster la taille du widget central

    // Créer la grille hexagonale de boutons
    setupHexagonalGrid(rows, cols, buttonSize);  // 30x30 boutons, taille de 25 pixels max, à cause du displacement, le rapport entre la hauteur et la largeur est de 1*4 pour une map 2x2
    setupDeck(buttonSize);
    // Configurez la fenêtre
    setWindowTitle("Test Hive");
    resize(1000,600);

    // Ajoutez le widget central sans layout
    setCentralWidget(centralWidget);
}

void hiveRenderer::setupHexagonalGrid(int rows, int cols, int buttonSize) {
    int padding = 50;  // padding
    int xconv=0, yconv=0;
    // ajuste la largeur et la hauteur d'un hexagone en fonction de la taille
    qreal hexWidth = buttonSize * 0.50; // Largeur de l'hexagone (ajustée pour l'espacement)
    qreal hexHeight = buttonSize*1.4;       // Hauteur de l'hexagone

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            // Créer un bouton hexagonal
            HexagonalButton *button = new HexagonalButton(buttonSize, this);
            button->setText(QString("%1,%2").arg(xconv).arg(yconv));


            // Décalage des colonnes impaires pour un motif hexagonal
            int offset = (col % 2 == 0) ? 0 : hexHeight / 2;

            // Calculer les coordonnées (x, y) pour chaque bouton
            qreal x = col * hexWidth+padding+80;
            qreal y = row * hexHeight + offset+padding;

            // Placer le bouton à la position calculée
            button->setParent(centralWidget);  // Ajouter le bouton directement au widget central
            button->move(x, y);  // Déplacer le bouton à la position calculée

            // Connecter le bouton à son slot
            connect(button, &QPushButton::clicked, this, &hiveRenderer::handleButtonClick);
            buttons[row][col]=button;
            if (col%2 == 1) {
                xconv--;
                yconv++;
            } else {
                xconv++;
            }
        }
        xconv+=2;
        yconv=0;
    }
}

void hiveRenderer::setupDeck(int buttonSize){
    int sizeDeck=11;
    for (int num=0;num<2;num++){

        if (num==0) {
            QLabel *name = new QLabel("Player 1");
            name->setGeometry(0,num*30*buttonSize+50,50,10);
        }
        else {
            QLabel *name = new QLabel("Player 2");
            name->setGeometry(0,num*30*buttonSize+50,50,10);
        }
        for (int i=0;i<sizeDeck;i++){
            // Créer un bouton hexagonal
            HexagonalButton *button = new HexagonalButton(buttonSize, this);
            button->setText(QString("%1").arg(num*(-13)+i-13));
            qreal x = num*35*buttonSize+60;
            qreal y = i*buttonSize+100;
            QPixmap pixmap(":/assets/ant.png"); // Remplacez par le chemin de votre image
            button->setIcon(QIcon(pixmap));
            button->setIconSize(pixmap.size());
            button->setParent(centralWidget);
            button->move(x,y);
            button->updateState(0);//Le bouton est un insecte
            connect(button,&QPushButton::clicked, this, &hiveRenderer::handleButtonClick);
            buttons[30][i+num*sizeDeck]=button;
        }
        buttons[30][0+num*sizeDeck]->setInsectType(bee);
        buttons[30][1+num*sizeDeck]->setInsectType(ant);
        buttons[30][2+num*sizeDeck]->setInsectType(ant);
        buttons[30][3+num*sizeDeck]->setInsectType(ant);
        buttons[30][4+num*sizeDeck]->setInsectType(beetle);
        buttons[30][5+num*sizeDeck]->setInsectType(beetle);
        buttons[30][6+num*sizeDeck]->setInsectType(grasshopper);
        buttons[30][7+num*sizeDeck]->setInsectType(grasshopper);
        buttons[30][8+num*sizeDeck]->setInsectType(grasshopper);
        buttons[30][9+num*sizeDeck]->setInsectType(spider);
        buttons[30][10+num*sizeDeck]->setInsectType(spider);
    }
}

hiveRenderer::~hiveRenderer()
{
    delete ui;
}

void hiveRenderer::handleButtonClick() {
    // Récupérez le bouton qui a déclenché le signal
    HexagonalButton *button = qobject_cast<HexagonalButton *>(sender());
    if (button) {
        //Affichez le texte du bouton dans le label
        infoLabel->setText(QString("Bouton cliqué : %1").arg(button->text()));
        if (!getInput()){//Si c'est la deuxième selection
            if (lastClicked!=nullptr){
                if (lastClicked->getInsectType()!=none){
                    button->setInsectType(lastClicked->getInsectType());
                    lastClicked->setInsectType(none);
                    lastClicked->updateState(2);//La case devient vide
                    button->updateState(0);
                }
            }
            lastClicked=nullptr;
            updateInput();
        } else {//Si c'est la première sélection
            if (button->getInsectType()!=none){
                button->updateState(1); //Insect a été sélectionné
                lastClicked=button;
            } else if (button->getInsectType()==none){
                lastClicked=nullptr;
            }
            updateInput();
        }
    }
}

/*
 * #include "hiverenderer.h"
#include "hexagonalbutton.h"
#include "./ui_hiverenderer.h"

hiveRenderer::hiveRenderer(QWidget *parent)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    gridLayout(new QGridLayout()),
    infoLabel(new QLabel("Cliquez sur un bouton", this))
    , ui(new Ui::hiveRenderer)
{
    ui->setupUi(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    gridLayout->addWidget(infoLabel, 0, 0, 1, 30); // Span sur 32 colonnes

    // Ajoutez une grille de boutons
    for (int row = 1; row <= 30; ++row) {
        for (int col = 0; col < 30; ++col) {

            QPushButton *button = new QPushButton(QString("(%1, %2)").arg(row).arg(col));
            button->setStyleSheet(
                "QPushButton {"
                "    background-color: #b5c02d;"   // Couleur de fond
                "    color: white;"               // Couleur du texte
                "    border: 2px solid #b5c021;"  // Bordure
                "    border-radius: 10px;"        // Coins arrondis
                "    clip-path: polygon(50% 0%, 100% 25%, 100% 75%, 50% 100%, 0% 75%, 0% 25%);" // Forme hexagonale
                "}"
                "QPushButton:hover {"
                "    background-color: #b5c021;"  // Couleur au survol
                "}"
                );
            button->setFixedSize(15,15);
            gridLayout->addWidget(button, row, col);

            // Connectez chaque bouton au slot
            connect(button, &QPushButton::clicked, this, &hiveRenderer::handleButtonClick);
setupHexagonalGrid(30,30,15);

}
}

// Configurez le widget central et son layout
centralWidget->setLayout(gridLayout);
setCentralWidget(centralWidget);

// Configurez la fenêtre
setWindowTitle("Test Hive");
resize(800, 800);
}

hiveRenderer::~hiveRenderer()
{
    delete ui;
}

void hiveRenderer::handleButtonClick() {
    // Récupérez le bouton qui a déclenché le signal
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button) {
        // Affichez le texte du bouton dans le label
        infoLabel->setText(QString("Bouton cliqué : %1").arg(button->text()));
    }
}

void hiveRenderer::setupHexagonalGrid(int rows, int cols, int buttonSize) {
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            HexagonalButton *button = new HexagonalButton(buttonSize, this);
            button->setText(QString("%1,%2").arg(row).arg(col));

            // Décalage des colonnes impaires
            int offset = (col % 2 == 0) ? 0 : buttonSize / 2;

            // Ajouter le bouton au layout
            gridLayout->addWidget(button, row, col);

            // Ajuster la position pour une grille hexagonale réaliste
            button->move(col * (buttonSize * 0.85), row * buttonSize + offset);
            connect(button, &QPushButton::clicked, this, &hiveRenderer::handleButtonClick);
        }
    }
}
*/
