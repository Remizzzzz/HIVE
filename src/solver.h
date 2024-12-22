//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_SOLVER_H
#define HIVE_SOLVER_H
#include <QDebug>

#include "features/insect.h"
#include "features/map.h"
#include "features/deck.h"
#include "features/player.h"
#include "utils/utils.h"
#include "utils/hiveException.h"
// #include "QDebug"
class Solver{

private:
    Map & map;
    const int & renderedMapSideSize;
    const int offset;

    //0 -> map, 1 -> deck 1, -> 1 deck 2
    int getStartLocation(Player & player_) const{
        return 1 * (player_.inputs.getStart().getI() == -1) + 2 * (player_.inputs.getStart().getI() == renderedMapSideSize);
    }
    int turn=1; //Sert pour compter les tours
    bool isStartValid(Player & player_) const{

        std::cout << !map.isSlotFree(player_.inputs.getStart()) << '_';
        if (player_.inputs.getStart().getI() >= 0 && player_.inputs.getStart().getI() < renderedMapSideSize &&
            player_.inputs.getStart().getJ() >= 0 && player_.inputs.getStart().getJ() < renderedMapSideSize){
            return true;
        }

        if ((player_.inputs.getStart().getI() == -1 || player_.inputs.getStart().getI() == renderedMapSideSize) &&
            (player_.inputs.getStart().getJ() >= 0 && player_.inputs.getStart().getJ() < player_.getDeck().getInsectNb())){
            return true;
        }

        return false;
    }

    bool isDestinationValid(Player & player_) const{
        return player_.inputs.getDestination().getI() >= 0 && player_.inputs.getDestination().getI() < renderedMapSideSize &&
                player_.inputs.getDestination().getJ() >= 0 && player_.inputs.getDestination().getJ() < renderedMapSideSize;
    }
public:
    [[nodiscard]] int getTurn()const {
        if (turn%2) return turn/2+1;
        return turn/2;
    }

    void setTurn(int val) {
        turn = val;
    }

    static bool queenInDeck(Player & player_) {
        bool result = true;
        std::vector<Insect*> activeList =player_.getActiveInsects();
        for (auto it : activeList) {
            if (it->getIT()==bee) {
                result = false;
            }
        }
        return result;
    }
    Solver(Map & map_, const int & renderedMapSideSize_, const int offset_) :
    map(map_), renderedMapSideSize(renderedMapSideSize_), offset(offset_){}

    void deckToMapMovement(Player & player_);

    void mapToMapMovement(const Player & player_){

        const vec2i & start = player_.inputs.getStart() + vec2i{offset,offset};
        const vec2i & destination = player_.inputs.getDestination();

        if (!map.isSlotFree(start)){
            std::cout << "slot pas free binks";
            map.moveInsect(start,destination);
            map.addToHistoric(start,destination);
            map.getInsectAt(destination)->setCoordinates(destination);
            turn++;

        }
    }
    void goBackDeck(Player & player_, vec2i from, vec2i to) {
        Insect* ins=map.getInsectAt(to);
        std::vector<Insect*> *deckList=player_.getDeck().getInsects();
        deckList->insert(deckList->begin()+from.getJ(),ins);
        map.removeInsectAt(to);
        player_.removeActiveInsect(ins);
        map.getHistoric().pop_front();
    }

    void fullGoBack(Player & player_, vec2i from, vec2i to) {}

    void decrTurn(){turn--;}
    int update(Player & player_);

};

#endif //HIVE_SOLVER_H
