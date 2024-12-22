#ifndef HIVERENDERER_H
#define HIVERENDERER_H

#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QDebug>

#include "hexagonalbutton.h"
#include "../hive.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class hiveRenderer;
}
QT_END_NAMESPACE



class hiveRenderer : public QMainWindow {
    Q_OBJECT

public:
    hiveRenderer(QWidget *parent = nullptr, int rewind=5,Mode mod=PvP, bool ladybug=false, bool mosquitoe=false, bool load=false, QString nomJ1="Player1", QString nomJ2="Player2");
    ~hiveRenderer();

    void updateInputT() { if (inputT) inputT=false; else inputT=true; }
    bool getInputT() { return inputT; }
    void updatePlayerTurn() { if (playerTurn) playerTurn=false; else playerTurn=true; }

    void showWinner(QString winner);
    void AIMovement(Player* opponent);

    vec2i convertCoordinates(vec2i coordinates) {
        coordinates.setJ(coordinates.getJ()-hive.getOffset()-1);
        coordinates.setI(coordinates.getI()-hive.getOffset()-1);
        return coordinates;
    }
    vec2i reconvertCoordinates(vec2i coordinates) {
        coordinates.setJ(coordinates.getJ()+hive.getOffset()+1);
        coordinates.setI(coordinates.getI()+hive.getOffset()+1);
        return coordinates;
    }

private:
    Hive hive;
    int buttonSize=25;
    int sizeDeck=11;
    int renderedMapSize = 30;// Taille de la map
    int rows=renderedMapSize/2; //30 lignes
    int cols=renderedMapSize*2; //30 colonnes
    HexagonalButton* lastClicked=nullptr;
    bool mosExten;
    Mode mode;
    bool ladExten;
    QString nomJ1;
    QString nomJ2;
    bool inputT=true;
    bool playerTurn=false;
    HexagonalButton* buttons[31][30];
    Ui::hiveRenderer *ui;
    QWidget *centralWidget;      // Conteneur principal
    QLabel *infoLabel;           // Label pour afficher les infos
    void setupHexagonalGrid(int rows, int cols, int buttonSize);
    void setupDeck(int buttonSize, QString nomJ1, QString nomJ2);
    void loadGame(bool load);

private slots : //Private slots, c'est pour détecter les signaux des boutons
    void handleButtonClick(); // Slot pour gérer les clics de bouton
    void handleParamButtonClick();
};

#endif // HIVERENDERER_H
