//
// Created by loic_ on 21/12/2024.
//

#ifndef AI_H
#define AI_H
#include "map.h"
#include "player.h"
#include "../utils/random.hpp"

class AI
{

public:
     void operator()(Map & map, Player & player_, Inputs & inputs, lv::Random & random, bool Qt=false, bool inputT=false)
     {
        int cursorId=0;
        if (!Qt) cursorId = inputs.isStartSelected() + 1;
        else {
            if (inputT) cursorId=1; //Si c'est la première sélection
            else cursorId = 2; //Si c'est la deuxième sélection
        }

        int randomValue = random.getRandomInt(0,4);
        const Insect * selectedInsect;

        switch (cursorId){
            case 1:
                if (player_.getDeck().isEmpty() && player_.getActiveInsects().empty()){
                    throw HiveException("inputsManager.h:InputsManager:updateAIInputs","deck and activeInsects are empty");
                    return;
                }

                if (player_.getActiveInsects().empty() && !player_.getDeck().isEmpty()) {
                    int index=player_.getDeck().returnIndex(bee); //On place bee pour pas avoir de problèmes de mouvements impossibles
                    selectedInsect = player_.getDeck().getInsectAt(index);
                } else if (randomValue == 0 && !player_.getDeck().isEmpty()){
                    selectedInsect = player_.getDeck().getInsectAt(random.getRandomInt(0,int(player_.getDeck().getInsectNb())));
                } else {
                    selectedInsect = player_.getActiveInsects()[random.getRandomInt(0,int(player_.getActiveInsects().size()))];
                    inputs.setStart(selectedInsect->getCoordinates());
                    vec2i startPos = selectedInsect->getCoordinates();
                    inputs.setStart(startPos);
                    inputs.needPossibleDestinationsUpdate();
                    break;
                }
                if (!Qt) {//Si on est pas dans Qt
                    inputs.setStart(selectedInsect->getCoordinates());
                } else {
                    vec2i startPos = selectedInsect->getCoordinates();
                    startPos.setI(-1);
                    inputs.setStart(startPos);
                }

                inputs.needPossibleDestinationsUpdate();

                break;
            case 2:
                if (!Qt) {
                    if (!inputs.isPossibleDestinationsEmpty()) {
                        inputs.setDestinationIndex(random.getRandomInt(0,int (inputs.getPossibleDestinationsNumber())));
                    }
                } else {
                    if (inputs.getStart().getI()==-1) {
                        inputs.setPossibleDestinations(map.setRule(false));
                        if (!inputs.getPossibleDestinations().empty()) {
                            inputs.setDestinationIndex(random.getRandomInt(0,inputs.getPossibleDestinationsNumber()));
                        } else {//Si l'IA ne peut pas poser d'insectes, elle réupdate ses inputs (recursif est pas optimisé, mais moins long à écrire pour l'instant)
                            operator()(map, player_, inputs, random, Qt, !inputT);
                            operator()(map, player_, inputs, random, Qt, inputT);
                        }
                    } else {
                        inputs.setPossibleDestinations(map.getInsectAt(inputs.getStart())->getPossibleMovements(map));
                        if (!inputs.getPossibleDestinations().empty()) {
                            inputs.setDestinationIndex(random.getRandomInt(0,inputs.getPossibleDestinationsNumber()));
                        } else {//Si l'IA ne peut pas déplacer d'insectes, elle réupdate ses inputs (recursif est pas optimisé, mais moins long à écrire pour l'instant)
                            operator()(map, player_, inputs, random, Qt, !inputT);
                            operator()(map, player_, inputs, random, Qt, inputT);
                        }
                    }
                }
            break;
            default:
                throw HiveException("inputsManager.h:InputsManager:moveCursor", "cursorId_ invalid");
            break;
        }
     }

};

#endif//AI_H
