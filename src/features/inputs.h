//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_INPUTS_H
#define HIVE_INPUTS_H

#include "../utils/utils.h"

struct Inputs{
    //le joueur a validé son action
    bool selected;

    //Position apres la premiere validation
    vec2i cursor1;
    //Position apres la deuxieme validation
    vec2i cursor2;

    //Si un curseur est en y negatif alors il est sur le deck 1
    //S'il est en y plus grand que la taille de la map il est sur le deck 2
    //Donc faut bien prendre en compte que ça vise une carte du deck

};

#endif //HIVE_INPUTS_H
