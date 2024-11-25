//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_INPUTMANAGER_H
#define HIVE_INPUTMANAGER_H

#include "features/inputs.h"
#include "utils/utils.h"

#include <iostream>
#include "utils/hiveException.h"

enum Mode{PvP,PvAI};


class InputsManager{

private:
    Inputs inputs;
    Mode mode;
    int switcher;

private:

    const Inputs & getAIInputs();
    const Inputs & getPlayerInputs();

public:

    explicit InputsManager(Mode mode_) : mode(mode_), inputs(), switcher(0){}

    const Inputs & getInputs(){

        switch (mode) {
            case PvP:
                return getPlayerInputs();
            case PvAI:
                if(switcher){
                    switcher = (switcher + 1) % 2;
                    return getPlayerInputs();
                }
                else{
                    switcher = (switcher + 1) % 2;
                    return getAIInputs();
                }
            default:
                throw HiveException("inputsManager.h:InputsManager:getInputs", "invalid mode");
        }
    }

};


#endif //HIVE_INPUTMANAGER_H
