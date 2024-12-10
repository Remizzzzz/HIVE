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
    //hive.runQt();
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
            vec2i pos(xconv, yconv);
            button->setCoordinates(pos);
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
            buttons[xconv][yconv]=button;
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
            vec2i d(-1,-1);
            button->setCoordinates(d);
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
    Player * actualP=hive.getPlayer1();
    vec2i deck(-1,-1);
    if (turn%2!=0) actualP=hive.getPlayer2();//Si c'est au tour du joueur 2, on change le joueur actuel
    if (button) {
        //Affichez le texte du bouton dans le label
        infoLabel->setText(QString("Bouton cliqué : %1").arg(button->text()));
        if (!getInputT()){//Si c'est la deuxième selection
            if (lastClicked!=nullptr){
                if (button->getState()==3) { //Si la case sélectionnée est bien dans les mouvements possibles
                    if (lastClicked->getInsectType()!=none){
                        hive.getInputsManager()->updatePlayerInputsQt(actualP,button->getCoordinates(),inputT);
                        if (actualP->getInputs().getStart()==deck) {//Si c'est deckToMap movement
                             hive.getSolver()->deckToMapMovement(*actualP);
                        }
                        /*
                        button->setInsectType(lastClicked->getInsectType());
                        //A modif pour le scarabée
                        lastClicked->setInsectType(none);
                        lastClicked->updateState(2);//La case devient vide
                        button->updateState(0);*/
                    }
                }
                lastClicked->updateState(0);
                for (auto b : actualP->getInputs().getPossibleDestinations()) {
                    HexagonalButton * selec=buttons[b.getI()][b.getJ()];
                    if (selec->getInsectType()!=none) {
                        selec->updateState(0);
                    } else {
                        selec->updateState(2);
                    }
                }
                hive.getInputsManager()->resetPlayerInputs(actualP);
                lastClicked=nullptr;
            }
            updateInputT();
        } else {//Si c'est la première sélection
            if (button->getInsectType()!=none){
                button->updateState(1); //Insect a été sélectionné
                lastClicked=button;

                hive.getInputsManager()->updatePlayerInputsQt(actualP,button->getCoordinates(),inputT);
                for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                    buttons[b.getI()][b.getJ()]->updateState(3);
                }
            } else if (button->getInsectType()==none){
                lastClicked=nullptr;
            }
            updateInputT();
        }
    }
}