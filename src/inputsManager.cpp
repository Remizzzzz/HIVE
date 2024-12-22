//
// Created by joanc on 22/12/2024.
//


#include <iostream>
#include <conio.h>
#include <solver.h>
#include <QDebug>
#include <algorithm>

#include "inputsManager.h"
#include "features/inputs.h"
#include "features/deck.h"
#include "features/player.h"
#include "utils/utils.h"
#include "utils/hiveException.h"
#include "utils/random.hpp"
#include <features/map.h>



void InputsManager::moveCursor(Player & player, const int cursorId_, const int i_, const int j_) const {
    Inputs & inputs = player.inputs;
    switch (cursorId_) {
        case 1:
            if ((inputs.getStart().getI() + i_ >= 0 && inputs.getStart().getI() + i_ < renderedMapSideSize &&
                    inputs.getStart().getJ() + j_ >= 0 && inputs.getStart().getJ() + j_ < renderedMapSideSize)
                    || ((inputs.getStart().getI() + i_ == -1 || inputs.getStart().getI() + i_ == renderedMapSideSize)
                        && (inputs.getStart().getJ() + j_ >= 0 && inputs.getStart().getJ() + j_ < player.getDeck().getInsectNb())))
            {
                std::cout << "----------\n ";
                std::cout << inputs.getStart() << ',' << vec2i{i_,j_} << '\n';
                inputs.setStart(inputs.getStart() + vec2i{i_,j_});
                std::cout << inputs.getStart();
            }
        break;
        case 2:
            std::cout << "laaaaa";
        if (inputs.getPossibleDestinationsNumber() > 1) {
            if (i_ + j_ > 0) {
                inputs.setDestinationIndex((inputs.getDestinationIndex() + 1) % inputs.getPossibleDestinationsNumber());
            } else{
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


void InputsManager::updateAIInputs(Player & player_, bool Qt, bool inputT) {
    Inputs & inputs = player_.inputs;
    int cursorId=0;
    if (!Qt) cursorId = inputs.isStartSelected() + 1;
    else {
        if (inputT) cursorId=1;  //Si c'est la première sélection
        else cursorId = 2;  //Si c'est la deuxième sélection
    }

    int randomValue = random.getRandomInt(0,4);
    const Insect * selectedInsect;
    switch (cursorId) {
        case 1:
            if (player_.deck.isEmpty() && player_.activeInsects.empty()){
                throw HiveException("inputsManager.h:InputsManager:updateAIInputs","deck and activeInsects are empty");
                return;
            }

            if (player_.getActiveInsects().empty() && !player_.getDeck().isEmpty()) {
                int index=player_.getDeck().returnIndex(bee); //On place bee pour pas avoir de problèmes de mouvements impossibles
                selectedInsect = player_.getDeck().getInsectAt(index);
            } else if (randomValue == 0 && !player_.getDeck().isEmpty()){
                selectedInsect = player_.deck.getInsectAt(random.getRandomInt(0,int(player_.getDeck().getInsectNb())));
            } else {
                selectedInsect = player_.getActiveInsects()[random.getRandomInt(0,int(player_.getActiveInsects().size()))];
                inputs.setStart(selectedInsect->getCoordinates());
                vec2i startPos = selectedInsect->getCoordinates();
                inputs.setStart(startPos);
                inputs.needPossibleDestinationsUpdate();
                break;
            }
            if (!Qt) {  //Si on n'est pas dans Qt
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
                    } else {//Si l'IA ne peut pas poser d'insectes, elle reupdate ses inputs (récursif n'est pas optimisé, mais moins long à écrire pour l'instant)
                        updateAIInputs(player_, Qt, !inputT);
                        updateAIInputs(player_, Qt, inputT);
                    }
                } else {
                    inputs.setPossibleDestinations(map.getInsectAt(inputs.getStart())->getPossibleMovements(map));
                    if (!inputs.getPossibleDestinations().empty()) {
                        inputs.setDestinationIndex(random.getRandomInt(0,inputs.getPossibleDestinationsNumber()));
                    } else {//Si l'IA ne peut pas déplacer d'insectes, elle reupdate ses inputs (récursif n'est pas optimisé, mais moins long à écrire pour l'instant)
                        updateAIInputs(player_, Qt, !inputT);
                        updateAIInputs(player_, Qt, inputT);
                    }
                }
            }
        break;
        default:
            throw HiveException("inputsManager.h:InputsManager:moveCursor", "cursorId_ invalid");
        break;
    }
}


void InputsManager::updatePlayerInputs(Player & player_) {
    std::cout << "bien dans update playerinput"<< std::endl;
    Inputs & inputs = player_.inputs;
    int key = _getch();
    std::cout << "appel de inputqlqchose"<< std::endl;
    int cursorId = inputs.isStartSelected() + 1;

    if (key == 0 || key == 224) {
        key = _getch();
        switch (key) {
            case 72: moveCursor(player_,cursorId,-1,0); std::cout << "Flèche Haut\n"; break;
            case 80: moveCursor(player_,cursorId,1,0); std::cout << "Flèche Bas\n"; break;
            case 75: moveCursor(player_,cursorId,0,-1); std::cout << "Flèche Gauche\n"; break;
            case 77: moveCursor(player_,cursorId,0,1); std::cout << "Flèche Droite\n"; break;
            case 83: inputs.needLeave(); std::cout << "Leaves\n"; break;
            default: std::cout << "Autre touche spéciale: Code " << key << "\n"; break;
        }
    } else {
        switch (key) {
            case 13:
                if (!inputs.isStartSelected()){
                    std::cout << "here";
                    inputs.selectStart();
                    inputs.needPossibleDestinationsUpdate();
                }
                else inputs.selectDestination();
                std::cout << "Enter.\n";
                break;
            case 27:
                inputs.needLeave();
                std::cout << "Sortie.\n";
                break;
            default: break;
        }
    }
}


void InputsManager::updatePlayerInputsQt(Player * player_, vec2i clickedPos, bool inputT, bool turnP) {
    Inputs & inputs = player_->inputs;
    const int deck=-1;
    if (inputT) {  //Si c'est la première sélection
        inputs.setStart(clickedPos);
        if (clickedPos.getI()!=deck) {//Si la position n'est pas dans le deck
            std::vector<vec2i> possibleMovements = map.getInsectAt(clickedPos)->getPossibleMovements(map);
            inputs.setPossibleDestinations(map.getInsectAt(clickedPos)->getPossibleMovements(map));
        } else {
            //Si la position est dans le deck
            inputs.setPossibleDestinations(map.setRule(!turnP));
        }
    } else {//Si c'est la deuxième sélection
        auto it = std::find(inputs.getPossibleDestinations().begin(), inputs.getPossibleDestinations().end(), clickedPos);
        if (it != inputs.getPossibleDestinations().end()) {  //On trouvera toujours l'index, la vérification de l'existence est faite avant l'appel de fonction
            inputs.setDestinationIndex(std::distance(inputs.getPossibleDestinations().begin(), it));
        }
    }
}


void InputsManager::convertQtToSolver(Player* player_) {
    Inputs & inputs = player_->inputs;
    inputs.setStart(inputs.getStart()-vec2i{1,1});
    std::vector<vec2i> newPossibleMovements;
    for (auto destination : inputs.getPossibleDestinations()) {
        newPossibleMovements.push_back(destination-vec2i{1,1});
    }
    inputs.setPossibleDestinations(newPossibleMovements);
}


void InputsManager::convertSolverToQt(Player* player_) {
    Inputs & inputs = player_->inputs;
    inputs.setStart(inputs.getStart()+vec2i{1,1});
    std::vector<vec2i> newPossibleMovements;
    for (auto destination : inputs.getPossibleDestinations()) {
        newPossibleMovements.push_back(destination+vec2i{1,1});
    }
    inputs.setPossibleDestinations(newPossibleMovements);
}
