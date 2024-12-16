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
enum buttonType {Rewind, Save, Load};
class ParamButton : public QPushButton {
    Q_OBJECT
    public:
    explicit ParamButton(QWidget *parent = nullptr, QString label="");
    void setType(buttonType t){bT=t;}
    buttonType getType(){return bT;}
private:
    buttonType bT;
};



#endif //PARAMBUTTON_H
