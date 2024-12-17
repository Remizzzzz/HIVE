#include "hiverenderer.h"
#include "hexagonalbutton.h"
#include "./ui_hiverenderer.h"
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include "MainWindow.h"
#include "ParamButton.h"
//#include "hive.h" -> Déclenche ENORMEMENT d'erreur
hiveRenderer::hiveRenderer(QWidget *parent, int rewind, Mode mod=PvP, bool ladybug, bool mosquitoe, bool load)
    : QMainWindow(parent),
    centralWidget(new QWidget(this)),
    infoLabel(new QLabel("Cliquez sur un bouton", this)),
    ui(new Ui::hiveRenderer),
    ladExten(ladybug),
    mosExten(mosquitoe),
    mode(mod)
{ui->setupUi(this);
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setGeometry(0, 0, width(), 30);  // Placer le label en haut
    centralWidget->setGeometry(0, 50, width(), height() - 50);  // Ajuster la taille du widget central
    hive.runQt(ladybug, mosquitoe);
    hive.setRewindNumber(rewind);
    if (ladExten) sizeDeck+=2;

    if (mosExten) sizeDeck+=2;

    // Créer la grille hexagonale de boutons
    setupHexagonalGrid(rows, cols, buttonSize);  // 30x30 boutons, taille de 25 pixels max, à cause du displacement, le rapport entre la hauteur et la largeur est de 1*4 pour une map 2x2
    setupDeck(buttonSize);
    // Configurez la fenêtre
    setWindowTitle("Hive !");
    resize(1000,600);
    loadGame(load);
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

    auto rewind=QString("Rewind");
    auto *button = new ParamButton(this,rewind);
    button->setType(Rewind);
    button->setParent(centralWidget);
    button->move(15, 30);
    connect(button, &ParamButton::clicked, this, &hiveRenderer::handleParamButtonClick);

    auto *saveButton = new ParamButton(this, QString("Save game"));
    saveButton->setType(Save);
    saveButton->setParent(centralWidget);
    saveButton->move(15, 60);
    connect(saveButton, &ParamButton::clicked, this, &hiveRenderer::handleSaveButtonClick);
}

void hiveRenderer::setupDeck(int buttonSize){
    vec2i deck(-1,-1);
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
            button->setCoordinates(deck);
            qreal x = num*35*buttonSize+60;
            qreal y = i*buttonSize+100;
            QPixmap pixmap(":/assets/ant.png"); // Remplacez par le chemin de votre image
            button->setIcon(QIcon(pixmap));
            button->setIconSize(pixmap.size());
            button->setParent(centralWidget);
            button->move(x,y);
            button->setPlayer(num);
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
        if (mosExten) {
            buttons[30][11+num*sizeDeck]->setInsectType(mosquitoe);
            buttons[30][12+num*sizeDeck]->setInsectType(mosquitoe);
        }
        if (ladExten){
            int mos=0;
            if (mosExten) mos=1;
            buttons[30][11+2*mos+num*sizeDeck]->setInsectType(ladybug);
            buttons[30][12+2*mos+num*sizeDeck]->setInsectType(ladybug);
        }
    }
}
void hiveRenderer::loadGame(bool load) {
    if (load) {
        hive.loadGame("../hive_parameters.txt");
        int renderedSize=hive.getRenderedMapSideSize();
        for (int i=0;i<renderedSize;i++) {
            for (int j=0;j<renderedSize;j++) {
                vec2i pos(i,j);
                Insect* in=hive.getMap().getInsectAt(pos);
                if (in!=nullptr) {
                    //Actu de la map
                    buttons[i][j]->setInsectType(in->getIT());
                    buttons[i][j]->setPlayer(!in->getColor());
                    buttons[i][j]->updateState(0);
                    //Actu des decks
                    int index=0;
                    while (in->getIT()!=buttons[30][index+!in->getColor()*sizeDeck]->getInsectType()) {
                        index++;
                    }
                    buttons[30][index+!in->getColor()*sizeDeck]->updateState(2);
                }
            }
        }
    }
}
hiveRenderer::~hiveRenderer() {
    delete ui;
}

void hiveRenderer::handleButtonClick() {

    // Récupérez le bouton qui a déclenché le signal
    HexagonalButton *button = qobject_cast<HexagonalButton *>(sender());
    Player * actualP=hive.getPlayer1();
    Player * opponent = hive.getPlayer2();
    vec2i deck(-1,-1);
    vec2i index(-1,-1);
    if (playerTurn){
        actualP=hive.getPlayer2();//Si c'est au tour du joueur 2, on change le joueur actuel
        opponent=hive.getPlayer1();
    }
    if (button) {
        //Affichez le texte du bouton dans le label
        infoLabel->setText(QString("Bouton cliqué : %1, selection : %2, tour %3").arg(button->text()).arg(inputT).arg(hive.getSolver()->getTurn()));
        if (!getInputT()){//Si c'est la deuxième selection
            if (lastClicked!=nullptr){
                if (button->getState()==3) { //Si la case sélectionnée est bien dans les mouvements possibles
                    if (lastClicked->getInsectType()!=none){
                        hive.getInputsManager()->updatePlayerInputsQt(actualP,button->getCoordinates(),inputT, playerTurn);

                        button->setInsectType(lastClicked->getInsectType());

                        if (lastClicked->getInsectType()==beetle && lastClicked->getCoordinates().getI()>-1) {

                            Beetle* b = dynamic_cast<Beetle*>(hive.getMap().getInsectAt(actualP->getInputs().getStart()));

                            if (b->getInsectUnder()!=nullptr) {

                                insectType beetleInsectType=b->getInsectUnder()->getIT();
                                bool  beetleInsectPlayer=b->getInsectUnder()->getColor();
                                lastClicked->setInsectType(beetleInsectType);
                                lastClicked->setPlayer(!beetleInsectPlayer);
                                lastClicked->updateState(0);
                            } else {
                                lastClicked->setInsectType(none);
                                lastClicked->updateState(2);//La case devient vide
                            }
                        }else {
                            lastClicked->setInsectType(none);
                            lastClicked->updateState(2);//La case devient vide
                        }

                        if (actualP->getInputs().getStart().getI()==-1) {//Si c'est deckToMap movement
                            hive.getSolver()->deckToMapMovement(*actualP);
                            hive.decrRewindUsed();
                            if (mode==PvAI) {
                                //lauchAiMove()
                            }
                        } else { //Si c'est mapToMapMovement
                            hive.getSolver()->mapToMapMovement(*actualP);
                            hive.decrRewindUsed();
                        }

                        hive.switchPlayer();
                        updatePlayerTurn();
                        button->updateState(0);
                        button->setPlayer(!playerTurn);
                        if (opponent->lostGame(hive.getMap())) {
                            showWinner(actualP);
                        } else if (actualP->lostGame(hive.getMap())) {
                            showWinner(opponent);
                        }
                        qDebug()<<"\nHistorique : ";
                        for (auto move : hive.getMap().getHistoric()) {
                            qDebug()<<"\n("<<move.from.getI()<<","<<move.from.getJ()<<")";
                            qDebug()<<"-> ("<<move.to.getI()<<","<<move.to.getJ()<<")";
                        }
                    }
                } else {
                    lastClicked->updateState(0);
                }
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
            if (button->getInsectType()!=none && button->getPlayer()==playerTurn) {
                if (!hive.getSolver()->queenInDeck(*actualP) || hive.getSolver()->getTurn()<4) {//Si la reine n'est pas dans le deck, ou si le tour est inférieur à 4
                    if (!hive.getSolver()->queenInDeck(*actualP)) {
                        button->updateState(1); //Insect a été sélectionné
                        lastClicked=button;

                        //Pour toujours avoir un index valide
                        if (button->getCoordinates()==deck) {
                            vec2i i(-1,actualP->getDeck().returnIndex(button->getInsectType()));
                            index=i;
                        } else {
                            index=button->getCoordinates();
                        }

                        hive.getInputsManager()->updatePlayerInputsQt(actualP,index,inputT,playerTurn);
                        for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                            buttons[b.getI()][b.getJ()]->updateState(3);
                        }
                        updateInputT();
                    } else {
                        if (button->getCoordinates()!=deck) {
                            lastClicked=nullptr;
                        }else {
                            button->updateState(1); //Insect a été sélectionné
                            lastClicked=button;
                            //Pour toujours avoir un index valide
                            vec2i i(-1,actualP->getDeck().returnIndex(button->getInsectType()));
                            index=i;
                            hive.getInputsManager()->updatePlayerInputsQt(actualP,index,inputT,playerTurn);
                            for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                                buttons[b.getI()][b.getJ()]->updateState(3);
                            }
                            updateInputT();
                        }
                    }
                } else {
                    if (button->getInsectType()!=bee) {
                        lastClicked=nullptr;
                    } else {
                        button->updateState(1); //Insect a été sélectionné
                        lastClicked=button;
                        //Pour toujours avoir un index valide
                        vec2i i(-1,actualP->getDeck().returnIndex(button->getInsectType()));
                        index=i;
                        hive.getInputsManager()->updatePlayerInputsQt(actualP,index,inputT,playerTurn);
                        for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                            buttons[b.getI()][b.getJ()]->updateState(3);
                        }
                        updateInputT();
                    }
                }
            } else if (button->getInsectType()==none){
                lastClicked=nullptr;
            }
        }
    }
}
void hiveRenderer::showWinner(Player* winner) {
    // Création d'une nouvelle fenêtre pour afficher les règles du jeu
    QWidget *winWindow = new QWidget;
    winWindow->setWindowTitle("Winner !");
    winWindow->setFixedSize(1000, 600);

    // Mise en page
    QVBoxLayout *layout = new QVBoxLayout;

    // Les règles (à écrire)
    QString message = QString("Congratulations ! Player %1 just won the game !!").arg(winner->getId());
    QLabel *winLabel = new QLabel(message, winWindow);
    winLabel->setAlignment(Qt::AlignCenter);
    QFont winFont = winLabel->font();
    winFont.setPointSize(16);
    winLabel->setFont(winFont);
    layout->addWidget(winLabel);
    winWindow->setLayout(layout);
    winWindow->show();

    // Cache temporairement la fenêtre principale
    this->hide();
}

void hiveRenderer::handleParamButtonClick() {
    auto *button = qobject_cast<ParamButton *>(sender());
    qDebug()<<"\n nombre Rewind : "<<hive.getRewindMax()-hive.getRewindUsed();
    if (button->getType()==Rewind) {
        if (hive.getRewindUsed()<hive.getRewindMax()) {
            vec2i from =hive.getMap().getHistoric().front().from;
            vec2i to = hive.getMap().getHistoric().front().to;
            qDebug()<<"\nfrom I : "<<from.getI();
            if(from.getI()>=0) {
                //c'etait un map to map movement
                *buttons[from.getI()][from.getJ()]=*buttons[to.getI()][to.getJ()];
                buttons[to.getI()][to.getJ()]->updateState(2);
                buttons[to.getI()][to.getJ()]->setInsectType(none);
                hive.getMap().goBack();
            } else {
                int sizeDeck=15;
                int index=0;
                int turn=1;
                Player * actualP=hive.getPlayer2();
                if (playerTurn) {
                    actualP=hive.getPlayer1();//Si c'est au tour du joueur 1, on change le joueur actuel
                    turn=0;
                }

                while (buttons[30][from.getJ()+index+(turn)*sizeDeck]->getState()!=2) {
                    index++;
                }
                qDebug()<<"\n"<<buttons[30][from.getJ()+(turn)*sizeDeck]->getState();
                *buttons[30][from.getJ()+index+(turn)*sizeDeck]=*buttons[to.getI()][to.getJ()];
                buttons[to.getI()][to.getJ()]->updateState(2);
                buttons[to.getI()][to.getJ()]->setInsectType(none);
                hive.getSolver()->goBackDeck(*actualP, from, to);
            }
            updatePlayerTurn();
            hive.getSolver()->decrTurn();
            hive.incrRewindUsed();
        }
        qDebug()<<"\nHistorique : ";
        for (auto move : hive.getMap().getHistoric()) {
            qDebug()<<"\n("<<move.from.getI()<<","<<move.from.getJ()<<")";
            qDebug()<<"-> ("<<move.to.getI()<<","<<move.to.getJ()<<")";
        }
    }
}

void hiveRenderer::handleSaveButtonClick() {
    hive.saveGame("../hive_parameters.txt");
}
