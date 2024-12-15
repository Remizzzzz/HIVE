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
#include <solver.h>
#include <features/map.h>
#include <algorithm>
enum Mode{PvP,PvAI};

class InputsManager{
private:
    const int renderedMapSideSize;
    Map & map;


    lv::Random random;
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
                std::cout << "laaaaa";
                if (inputs.getPossibleDestinationsNumber() > 1){
                    if (i_ + j_ > 0){
                        inputs.setDestinationIndex((inputs.getDestinationIndex() + 1) % inputs.getPossibleDestinationsNumber());
                    }
                    else{
                        inputs.setDestinationIndex(((inputs.getDestinationIndex() - 1) < 0) * (inputs.getPossibleDestinationsNumber() - 1) +
                                                            ((inputs.getDestinationIndex() - 1) >= 0) * (inputs.getDestinationIndex() - 1));
                    }
                }

                break;

            default:
                throw HiveException("inputsManager.h:InputsManager:moveCursor", "cursorId_ invalid");
                break;
        }
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
                    //lancer exception
                if (!inputs.isPossibleDestinationsEmpty()){
                    inputs.setDestinationIndex(random.getRandomInt(0,int (inputs.getPossibleDestinationsNumber())));
                }
                else{
                    //lancer exception
                }
                break;
            default:
                throw HiveException("inputsManager.h:InputsManager:moveCursor", "cursorId_ invalid");
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
                        std::cout << "here";
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

    void updatePlayerInputsQt(Player * player_, vec2i clickedPos, bool inputT, bool turnP) {
        Inputs & inputs = player_->inputs;
        const int deck=-1;
        if (inputT) {//Si c'est la première sélection
            inputs.setStart(clickedPos);
            if (clickedPos.getI()!=deck) {//Si la position n'est pas dans le deck
                std::vector<vec2i> possibleMovements = map.getInsectAt(clickedPos)->getPossibleMovements(map);
                inputs.setPossibleDestinations(map.getInsectAt(clickedPos)->getPossibleMovements(map));
            } else {
                //Si la position est dans le deck
                //inputs.setPossibleDestinations(test);inputs.getStart()
                //inputs.setPossibleDestinations(map.setRule(map.getInsectAt(inputs.getStart())->getColor()));
                inputs.setPossibleDestinations(map.setRule(!turnP));
            }
        } else {//Si c'est la deuxième sélection
            auto it = std::find(inputs.getPossibleDestinations().begin(), inputs.getPossibleDestinations().end(), clickedPos);
            if (it != inputs.getPossibleDestinations().end()) {//On trouvera toujours l'index, la vérification de l'existence est faite avant l'appel de fonction
                inputs.setDestinationIndex(std::distance(inputs.getPossibleDestinations().begin(), it));
            }
        }
    }

    void resetPlayerInputs(Player* player_) {
        player_->inputs.resetQt();
    }


};


#endif //HIVE_INPUTMANAGER_H
