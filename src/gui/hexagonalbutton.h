#ifndef HEXAGONALBUTTON_H
#define HEXAGONALBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPolygon>
#include <QRegion>
#include "../features/insect.h"

class HexagonalButton : public QPushButton {
    Q_OBJECT

public:
    explicit HexagonalButton(int size, QWidget *parent = nullptr);
    void setInsectType(insectType IT);
    insectType getInsectType();
    void paintEvent(QPaintEvent *event) override;
    QPolygon getHexagon(){return hexagon;}
    void updateState(int event);
    int getState(){return currentEvent;}
    vec2i getCoordinates(){return coordinates;}
    void setCoordinates(vec2i coordinates) {
        this->coordinates = coordinates;
    }
    void setPlayer(int player_) {if (player_)player=true; else player=false;}
    bool getPlayer(){return player;}
private:
    bool player;
    int currentEvent=2; //Case vide
    QPolygon hexagon; // Polygone représentant la forme hexagonale
    void createHexagon(int size); // Méthode pour définir la forme
    insectType iT=none;
    vec2i coordinates;
};

void paintButton(HexagonalButton* b,int event);
#endif // HEXAGONALBUTTON_H
