#ifndef HIVERENDERER_H
#define HIVERENDERER_H
#include <QGridLayout>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include "hexagonalbutton.h"
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
    void updateInput(){
        if (input){
            input=false;
        }else {
            input=true;
        }
    }
    bool getInput(){return input;}
protected:
    void resizeEvent(QResizeEvent *event) override;  // Surcharge de l'événement de redimensionnement

private:
    int buttonSize=25;
    int rows=15; //30 lignes
    int cols=60; //30 colonnes
    HexagonalButton* lastClicked=nullptr;
    bool input=true;
    HexagonalButton* buttons[31][30]; //Le 31 est pour les decks
    Ui::hiveRenderer *ui;
    QWidget *centralWidget;      // Conteneur principal
    QLabel *infoLabel;           // Label pour afficher les infos
    void setupHexagonalGrid(int rows, int cols, int buttonSize);
    void setupDeck(int buttonSize);

private slots :
    void handleButtonClick(); // Slot pour gérer les clics de bouton

};
#endif // HIVERENDERER_H
