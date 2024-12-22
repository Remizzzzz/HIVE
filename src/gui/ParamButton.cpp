//
// Created by rzong on 12/12/2024.
//

#include "ParamButton.h"


ParamButton::ParamButton( QWidget *parent, QString label)
    : QPushButton(parent) {
    setText(label);
}
