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
    Inputs & inputs;
    Mode mode;
    int switcher;

private:

    void updateAIInputs();
    void updatePlayerInputs();

public:

    explicit InputsManager(Mode mode_, Inputs & inputs_) : mode(mode_), inputs(inputs_), switcher(0){}

    void updateInputs(){

        switch (mode) {
            case PvP:
                updatePlayerInputs();
                break;
            case PvAI:
                if(switcher){
                    switcher = (switcher + 1) % 2;
                    updatePlayerInputs();
                    break;
                }
                else{
                    switcher = (switcher + 1) % 2;
                    updateAIInputs();
                    break;
                }
            default:
                throw HiveException("inputsManager.h:InputsManager:getInputs", "invalid mode");
                break;
        }
    }

};


#endif //HIVE_INPUTMANAGER_H
