#include "hiverenderer.h"
#include "hexagonalbutton.h"
#include "ui_hiverenderer.h"
#include <QPixmap>
#include <QIcon>
#include <QDebug>
#include "MainWindow.h"
#include "ParamButton.h"


QDebug operator<<(QDebug debug, const vec2i &v) {
    QDebugStateSaver saver(debug); // Sauvegarde l'état de qDebug pour éviter de modifier son format global
    debug.nospace() << "vec2i(" << v.getI() << ", " << v.getJ() << ")";
    return debug;
}


hiveRenderer::hiveRenderer(QWidget *parent, int rewind, Mode mod, bool ladybug, bool mosquitoe, bool load)
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
    hive.setRewindNumber(2*rewind);
    if (ladExten) sizeDeck+=2;

    if (mosExten) sizeDeck+=2;

    // Créer la grille hexagonale de boutons
    setupHexagonalGrid(rows, cols, buttonSize);  // 30x30 boutons, taille de 25 pixels max, à cause du displacement, le rapport entre la hauteur et la largeur est de 1*4 pour une map 2x2
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
    button->setStyleSheet("background: grey;");

    auto *saveButton = new ParamButton(this, QString("Save game"));
    saveButton->setType(Save);
    saveButton->setParent(centralWidget);
    saveButton->move(15, 60);
    connect(saveButton, &ParamButton::clicked, this, &hiveRenderer::handleParamButtonClick);
    saveButton->setStyleSheet("background : grey;");

    auto menu=QString("Menu");
    auto *menuButton = new ParamButton(this,menu);
    menuButton->setType(Menu);
    menuButton->setParent(centralWidget);
    menuButton->move(30*25+100, 30);
    connect(menuButton, &ParamButton::clicked, this, &hiveRenderer::handleParamButtonClick);
    menuButton->setStyleSheet("background: grey;");
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
            QPixmap pixmap(":/assets/ant.png");
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
        //hive.setRenderer(hiveRenderer()) ;
        hive.loadGame("../hive_parameters.txt");
        mode=hive.getMode();
        int renderedSize=hive.getRenderedMapSideSize();
        for (int i=0;i<renderedSize;i++) {
            for (int j=0;j<renderedSize;j++) {
                vec2i pos(i,j);
                pos=reconvertCoordinates(pos);
                Insect* in=hive.getMap().getInsectAt(pos);
                if (in!=nullptr) {
                    //Actu de la map
                    buttons[i][j]->setInsectType(in->getIT());
                    buttons[i][j]->setPlayer(!in->getColor());
                    buttons[i][j]->updateState(0);
                }
            }
        }
        switch (hive.getExtension()) {
            case 0:
                ladExten=false;
                mosExten=false;
            case 1:
                ladExten=true;
                mosExten=false;
                break;
            case 2 :
                ladExten=false;
                mosExten=true;
                break;
            case 3 :
                ladExten=true;
                mosExten=true;
        }
        if (ladExten) sizeDeck+=2;
        if (mosExten) sizeDeck+=2;
        setupDeck(buttonSize);
        //Actu des decks
        int index=0;
        for (index=0;index<2*sizeDeck;index++) {
            buttons[30][index]->setInsectType(none);
            buttons[30][index]->updateState(2);
        }
        index=0;
        for (auto it : hive.getPlayer1()->getDeck()) {
            buttons[30][index]->updateState(0);
            buttons[30][index]->setInsectType(it->getIT());
            index++;
        }
        index=0;
        for (auto it : hive.getPlayer2()->getDeck()) {
            buttons[30][index+sizeDeck]->updateState(0);
            buttons[30][index+sizeDeck]->setInsectType(it->getIT());
            index++;
        }

        if (hive.getCurrentPlayer()==hive.getPlayer2()->getId())playerTurn=false;
        else playerTurn=true;

        hive.setRewindNumber(hive.getRewindMax());
    } else {
        setupDeck(buttonSize);
    }
}
hiveRenderer::~hiveRenderer() {
    delete ui;
}

void hiveRenderer::AIMovement(Player* opponent) {
    hive.getInputsManager()->updateAIInputs2(*opponent,AI{},true);
    qDebug()<<"\nDepart  : "<<opponent->getInputs().getStart();
    qDebug()<<"Arrivee : "<<opponent->getInputs().getPossibleDestinations()[opponent->getInputs().getDestinationIndex()];
    HexagonalButton* startButton;
    if (opponent->getInputs().getStart().getI()==-1) {
        startButton=buttons[30][sizeDeck+opponent->getInputs().getStart().getJ()];
    } else {
        vec2i coor(opponent->getInputs().getStart().getI(),opponent->getInputs().getStart().getJ());
        coor=convertCoordinates(coor);
        startButton=buttons[coor.getI()][coor.getJ()];
    }
    vec2i v=convertCoordinates(opponent->getInputs().getPossibleDestinations()[opponent->getInputs().getDestinationIndex()]);
    qDebug()<<"IA vec2i"<<v;
    *buttons[v.getI()][v.getJ()]=*startButton;
    buttons[v.getI()][v.getJ()]->updateState(0);
    startButton->updateState(2);
    startButton->setInsectType(none);
    qDebug()<< "\nAI("<<opponent->getInputs().getStart().getI()<<","<<opponent->getInputs().getStart().getJ()<<")";
    if (opponent->getInputs().getStart().getI()==-1) {//Si c'est deckToMap movement
        hive.getSolver()->deckToMapMovement(*opponent);
        hive.decrRewindUsed();
    } else { //Si c'est mapToMapMovementdebug
        hive.getSolver()->mapToMapMovement(*opponent);
        hive.decrRewindUsed();
    }


/* //Actualisation forcée de toute la map
    for (int i=0;i<hive.getRenderedMapSideSize();i++) {
        for (int j=0;j<hive.getRenderedMapSideSize();j++) {
            vec2i pos(i,j);
            pos=reconvertCoordinates(pos);
            Insect* in=hive.getMap().getInsectAt(pos);
            if (in!=nullptr) {
                //Actu de la map
                buttons[i][j]->setInsectType(in->getIT());
                buttons[i][j]->setPlayer(!in->getColor());
                buttons[i][j]->updateState(0);
            }
        }
    }*/
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
        //infoLabel->setText(QString("bouton cliqué : %1, %2").arg(button->getCoordinates().getI()).arg(button->getCoordinates().getJ()));
        if (!getInputT()){//Si c'est la deuxième selection
            if (lastClicked!=nullptr){
                if (button->getState()==3) { //Si la case sélectionnée est bien dans les mouvements possibles
                    if (lastClicked->getInsectType()!=none){
                        vec2i coordinates=reconvertCoordinates(button->getCoordinates());
                        hive.getInputsManager()->updatePlayerInputsQt(actualP,coordinates,inputT, playerTurn);

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
                        } else if(lastClicked->getInsectType()==mosquitoe && lastClicked->getCoordinates().getI()>-1){
                            Mosquitoe* b = dynamic_cast<Mosquitoe*>(hive.getMap().getInsectAt(actualP->getInputs().getStart()));

                            if (b->getInsectUnder()!=nullptr) {

                                insectType mosquitoeInsectType=b->getInsectUnder()->getIT();
                                bool  mosquitoeInsectPlayer=b->getInsectUnder()->getColor();
                                lastClicked->setInsectType(mosquitoeInsectType);
                                lastClicked->setPlayer(!mosquitoeInsectPlayer);
                                lastClicked->updateState(0);
                            } else {
                                lastClicked->setInsectType(none);
                                lastClicked->updateState(2);//La case devient vide
                            }

                        } else {
                            lastClicked->setInsectType(none);
                            lastClicked->updateState(2);//La case devient vide
                        }
                        if (actualP->getInputs().getStart().getI()==-1) {//Si c'est deckToMap movement

                            hive.getSolver()->deckToMapMovement(*actualP);
                            hive.decrRewindUsed();
                        } else { //Si c'est mapToMapMovement
                            hive.getInputsManager()->convertQtToSolver(actualP);
                            hive.getSolver()->mapToMapMovement(*actualP);
                            hive.getInputsManager()->convertSolverToQt(actualP);
                            hive.decrRewindUsed();
                        }


                        button->updateState(0);
                        button->setPlayer(playerTurn);
                        if (opponent->lostGame(hive.getMap())) {
                            showWinner(actualP);
                        } else if (actualP->lostGame(hive.getMap())) {
                            showWinner(opponent);
                        }
                        if (mode==PvP) {
                            updatePlayerTurn();
                        } else {
                            AIMovement(opponent);
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
                    b=convertCoordinates(b);
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
                            index=reconvertCoordinates(button->getCoordinates());
                        }

                        hive.getInputsManager()->updatePlayerInputsQt(actualP,index,inputT,playerTurn);
                        for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                            qDebug()<<"b =("<<b.getI()<<","<<b.getJ()<<")";
                            b=convertCoordinates(b);
                            buttons[b.getI()][b.getJ()]->updateState(3);
                        }
                        updateInputT();
                    } else {
                        if (button->getCoordinates()!=deck) {
                            infoLabel->setText(QString("Vous devez poser la reine pour bouger un insecte"));
                            lastClicked=nullptr;
                        }else {
                            button->updateState(1); //Insect a été sélectionné
                            lastClicked=button;
                            //Pour toujours avoir un index valide
                            vec2i i(-1,actualP->getDeck().returnIndex(button->getInsectType()));
                            index=i;
                            hive.getInputsManager()->updatePlayerInputsQt(actualP,index,inputT,playerTurn);
                            for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                                b=convertCoordinates(b);
                                buttons[b.getI()][b.getJ()]->updateState(3);
                            }
                            updateInputT();
                        }
                    }
                } else {
                    if (button->getInsectType()!=bee) {
                        lastClicked=nullptr;
                        infoLabel->setText(QString("Vous devez poser la reine"));
                    } else {
                        infoLabel->setText(QString(""));
                        button->updateState(1); //Insect a été sélectionné
                        lastClicked=button;
                        //Pour toujours avoir un index valide
                        vec2i i(-1,actualP->getDeck().returnIndex(button->getInsectType()));
                        index=i;
                        hive.getInputsManager()->updatePlayerInputsQt(actualP,index,inputT,playerTurn);
                        for (auto b : actualP->getInputs().getPossibleDestinations()) {//On itère dans la liste des destionations possibles
                            b=convertCoordinates(b);
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

    auto menu=QString("Menu");
    auto *menuButton = new ParamButton(winWindow,menu);
    menuButton->setType(Menu);
    menuButton->setParent(winWindow);
    menuButton->move(30*25+100, 30);
    connect(menuButton, &ParamButton::clicked, this, &hiveRenderer::handleParamButtonClick);
    menuButton->setStyleSheet("background: grey;");
    winWindow->show();

    // Cache temporairement la fenêtre principale
    this->hide();
}

void hiveRenderer::handleParamButtonClick() {
    auto *button = qobject_cast<ParamButton *>(sender());
    qDebug()<<"\n nombre Rewind : "<<hive.getRewindMax()-hive.getRewindUsed();
    if (button->getType()==Rewind) {
        for (int i=0; i<2;i++) {
            if (hive.getRewindUsed()<hive.getRewindMax()) {
                vec2i from =hive.getMap().getHistoric().front().from;
                vec2i to = convertCoordinates(hive.getMap().getHistoric().front().to);
                if(from.getI()>=0) {
                    //c'etait un map to map movement
                    from=convertCoordinates(from);
                    *buttons[from.getI()][from.getJ()]=*buttons[to.getI()][to.getJ()];
                    buttons[to.getI()][to.getJ()]->updateState(2);
                    buttons[to.getI()][to.getJ()]->setInsectType(none);
                    hive.getMap().goBack();
                } else {
                    int index=0;
                    int turn=1;
                    Player * actualP=hive.getPlayer2();
                    qDebug()<<"\n index :"<<from.getJ();
                    qDebug()<<"\n\nPlayer 1 ? : "<<playerTurn;
                    if (playerTurn) {
                        actualP=hive.getPlayer1();//Si c'est au tour du joueur 1, on change le joueur actuel
                        turn=0;
                    }

                    while (buttons[30][index+(turn)*(sizeDeck)]->getState()!=2) {
                        index++;
                    }
                    qDebug()<<"\n\nbuttonI "<<from.getJ()+index+(turn)*sizeDeck;
                    *buttons[30][index+(turn)*sizeDeck]=*buttons[to.getI()][to.getJ()];
                    buttons[to.getI()][to.getJ()]->updateState(2);
                    buttons[to.getI()][to.getJ()]->setInsectType(none);
                    hive.getSolver()->goBackDeck(*actualP, from, reconvertCoordinates(to));
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
    } else if (button->getType()==Menu) {
        MainWindow *w=new MainWindow(nullptr);
        w->show();
        this->close();
    } else if (button->getType()==Save) {
        hive.saveGame("../hive_parameters.txt");
        Player * actualP=hive.getPlayer2();
        if (playerTurn) {
            actualP=hive.getPlayer1();//Si c'est au tour du joueur 1, on change le joueur actuel
        }
        for (auto it : actualP->getDeck()) {
            qDebug()<<QString("%1").arg(it->getIT());
        }
    }
}