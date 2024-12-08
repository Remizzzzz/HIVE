//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_INPUTMANAGER_H
#define HIVE_INPUTMANAGER_H

#include "features/inputs.h"
#include "features/deck.h"
#include "features/player.h"

#include "utils/utils.h"
#include "utils/hiveException.h"
#include "utils/random.hpp"

#include <iostream>
#include <conio.h>


enum Mode{PvP,PvAI};

class InputsManager{

private:
    const int renderedMapSideSize;
    Map & map;


    lv::Random random;

private:

    void moveCursor(Player & player, const int cursorId_, const int i_, const int j_) const{

        Inputs & inputs = player.inputs;

        switch (cursorId_){
            case 1:
                if ((inputs.getStart().getI() + i_ >= 0 && inputs.getStart().getI() + i_ < renderedMapSideSize &&
                        inputs.getStart().getJ() + j_ >= 0 && inputs.getStart().getJ() + j_ < renderedMapSideSize)
                        || ((inputs.getStart().getI() + i_ == -1 || inputs.getStart().getI() + i_ == renderedMapSideSize) && inputs.getStart().getJ() + j_ < player.getDeck().getInsectNb()))
                    {
                    std::cout << "----------\n ";
                    std::cout << inputs.getStart() << ',' << vec2i{i_,j_} << '\n';
                    inputs.setStart(inputs.getStart() + vec2i{i_,j_});
                    std::cout << inputs.getStart();
                }
                break;
            case 2:
                if (i_ + j_ > 0){
                    inputs.setDestionationIndex((inputs.getDestinationIndex() + 1) % inputs.getPossibleDestinationsNumber());
                }
                else{
                    inputs.setDestionationIndex((inputs.getDestinationIndex() - 1) % inputs.getPossibleDestinationsNumber());
                }
                break;

            default:
                throw HiveException("inputsManager.h:InputsManager:moveCursor", "cursorId_ invalid");
                break;
        }
    }

    void updatePossibleMovements(Player & player_){

    }

public:

    explicit InputsManager(Mode mode_, const int renderedMapSideSize_, Map & map_):
    renderedMapSideSize(renderedMapSideSize_), map(map_), random(){}

    void updateAIInputs(Player & player_){
        Inputs & inputs = player_.inputs;

        int cursorId = inputs.isStartSelected() + 1;

        int randomValue = random.getRandomInt(0,4);
        const Insect * selectedInsect;

        switch (cursorId){
            case 1:
                if ((player_.deck.isEmpty() && player_.activeInsects.empty())){
                    throw HiveException("inputsManager.h:InputsManager:updateAIInputs","deck and activeInsects are empty");
                    return;
                }

                if ((randomValue == 0 || player_.getActiveInsects().empty()) && !player_.getDeck().isEmpty()){
                    selectedInsect = player_.deck.getInsectAt(random.getRandomInt(0,int(player_.getDeck().getInsectNb())));
                }
                else{
                    selectedInsect = player_.getActiveInsects()[random.getRandomInt(0,int(player_.getActiveInsects().size()))];
                }

                inputs.setStart(selectedInsect->getCoordinates());

                inputs.needPossibleDestinationsUpdate();

                break;
            case 2:
                if (!inputs.isPossibleDestinationsEmpty()){
                    inputs.setDestionationIndex(random.getRandomInt(0,int (inputs.getPossibleDestinationsNumber())));
                }
                else{
                    //lancer exception
                }
                break;
            default:
                //Lancer exception
                break;
        }

    }


    void updatePlayerInputs(Player & player_){
        Inputs & inputs = player_.inputs;

        int key = _getch();

        int cursorId = inputs.isStartSelected() + 1;

        if (key == 0 || key == 224) {
            key = _getch();

            switch (key) {
                case 72:
                    moveCursor(player_,cursorId,-1,0); std::cout << "Flèche Haut\n"; break;
                case 80:
                    moveCursor(player_,cursorId,1,0); std::cout << "Flèche Bas\n"; break;
                case 75:
                    moveCursor(player_,cursorId,0,-1); std::cout << "Flèche Gauche\n"; break;
                case 77:
                    moveCursor(player_,cursorId,0,1); std::cout << "Flèche Droite\n"; break;
                default: std::cout << "Autre touche spéciale: Code " << key << "\n"; break;
            }
        }
        else {
            switch (key) {
                case 13:
                    if (!inputs.isStartSelected()){
                        inputs.selectStart();
                        inputs.needPossibleDestinationsUpdate();
                    }
                    else{
                        inputs.selectDestination();
                    }
                    std::cout << "Enter.\n";
                    break;
                case 27:
                    inputs.reset();
                    std::cout << "Sortie.\n";
                    break;
                default: break;
            }
        }
    }
};


#endif //HIVE_INPUTMANAGER_H
