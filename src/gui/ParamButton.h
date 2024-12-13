//
// Created by rzong on 12/12/2024.
//

#ifndef PARAMBUTTON_H
#define PARAMBUTTON_H

#include <QPushButton>
#include <QPainter>
#include <QPolygon>
#include <QRegion>
#include "../features/insect.h"

class ParamButton : public QPushButton {
    Q_OBJECT
    public:
    explicit ParamButton(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void updateState(int event){currentEvent=event;}
    int getState(){return currentEvent;}
private:
    int currentEvent=0;
};



#endif //PARAMBUTTON_H
