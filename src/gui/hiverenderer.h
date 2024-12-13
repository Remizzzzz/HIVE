#ifndef HIVERENDERER_H
#define HIVERENDERER_H
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include "hexagonalbutton.h"
#include "../hive.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class hiveRenderer;
}
QT_END_NAMESPACE

class hiveRenderer : public QMainWindow
{
    Q_OBJECT

public:
    hiveRenderer(QWidget *parent = nullptr);
    ~hiveRenderer();
    void updateInputT(){
        if (inputT){
            inputT=false;
        } else {
            inputT=true;
        }
    }
    bool getInputT(){return inputT;}
    void updatePlayerTurn() {
        if (playerTurn) {
            playerTurn=false;
        } else {
            playerTurn=true;
        }
    }
    void showWinner();
private:
    Hive hive;
    int buttonSize=25;
    int renderedMapSize = 30;// Taille de la map
    int rows=renderedMapSize/2; //30 lignes
    int cols=renderedMapSize*2; //30 colonnes
    HexagonalButton* lastClicked=nullptr;
    bool inputT=true;
    bool playerTurn=false;
    HexagonalButton* buttons[31][30]; //Le 31 est pour les decks
    Ui::hiveRenderer *ui;
    QWidget *centralWidget;      // Conteneur principal
    QLabel *infoLabel;           // Label pour afficher les infos
    void setupHexagonalGrid(int rows, int cols, int buttonSize);
    void setupDeck(int buttonSize);

private slots : //Private slots, c'est pour détecter les signaux des boutons
    void handleButtonClick(); // Slot pour gérer les clics de bouton
    void handleParamButtonClick();
};

#endif // HIVERENDERER_H
