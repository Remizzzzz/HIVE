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
enum buttonType {Rewind};
class ParamButton : public QPushButton {
    Q_OBJECT
    public:
    explicit ParamButton(QWidget *parent = nullptr, QString label="");
    void updateState(int event){currentEvent=event;}
    int getState(){return currentEvent;}
    void setType(buttonType t){bT=t;}
    buttonType getType(){return bT;}
private:
    int currentEvent=0;
    buttonType bT;
};



#endif //PARAMBUTTON_H
