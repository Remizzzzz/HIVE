//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_SOLVER_H
#define HIVE_SOLVER_H

#include "features/map.h"
#include "features/inputs.h"

class Solver{

private:
    Map & map;

public:

    Solver(Map & map_) : map(map_){};

    void update(const Inputs & inputs_){
        //Update la map en fonction de l'inputs
    }

};

#endif //HIVE_SOLVER_H
