#ifndef HEXAGONALBUTTON_H
#define HEXAGONALBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPolygon>

#include "../features/insect.h"

class HexagonalButton : public QPushButton {
    Q_OBJECT

public:
    explicit HexagonalButton(int size, QWidget *parent = nullptr);
    HexagonalButton& operator=(const HexagonalButton &other);

    insectType getInsectType() { return iT; }
    QPolygon getHexagon() { return hexagon; }
    int getState() { return currentEvent; }
    vec2i getCoordinates() { return coordinates; }
    bool getPlayer() { return player; }

    void setInsectType(insectType IT) { iT=IT; update(); }
    void setCoordinates(vec2i coordinates) { this->coordinates = coordinates; }
    void setPlayer(int player_) { if (player_)player=true; else player=false; }
    void updateState(int event) { currentEvent = event; update(); }

    void paintEvent(QPaintEvent *event) override;

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
