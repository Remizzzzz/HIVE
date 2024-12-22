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

    void deckToMapMovement(Player & player_) {
        const vec2i & start = player_.inputs.getStart();
        const vec2i & destination = player_.inputs.getDestination();
        if (player_.getDeck().isIndexValid(start.getJ())){
            if (map.isSlotFree(destination)) {
                qDebug()<<"\n\nC'est free";
                map.putInsectTo(player_.getDeck().getInsectAt(start.getJ()), destination);

                map.getInsectAt(destination)->setCoordinates(destination);
                map.addToHistoric(start,destination);//If the movement is a rewind, goBack will manage the historic
                player_.addActiveInsectsFromDeck(start.getJ());
                player_.deck.removeAt(start.getJ());
                turn++;
            } else if (player_.getDeck().getInsectAt(start.getJ())->getIT() == grasshopper) {
            } else {
                player_.inputs.setMessage("Can't put your insect here");
            }
        } else {
            throw HiveException("solver.h:Solver:deckToMapGestion", "cursor1 is invalid for deck1");
        }
    }

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
    int update(Player & player_){

        if (player_.inputs.isLeaveNeeded())
        {
            return 2;
        }
        if (player_.inputs.isRewindNeeded())
        {
            return 3;
        }

        if (!player_.isHuman && player_.inputs.getStart().getI() != -1 && player_.inputs.getStart().getI() < renderedMapSideSize)
        {
            player_.inputs.setStart(player_.inputs.getStart() - vec2i{offset,offset});
        }

        if (player_.inputs.isPossibleDestinationsNeeded()){
            std::cout << "possibleDestinationsNeeded\n";
            if (isStartValid(player_)){
                std::cout << "startValid\n" ;
                int loc = getStartLocation(player_);

                std::cout << "loc : " << loc << "\n";
                std::cout << "player : " << player_.getId() << "\n";

                if (loc == 0){

                    //Decalage pour que l'on travail pas sur les bords de la map
                    vec2i start = player_.inputs.getStart() + vec2i{offset,offset};

                    std::cout << "loc0";
                    std::cout << "ici ?.";
                    std::cout << (map.getInsectAt(start) == nullptr);
                    std::cout << (map.getInsectAt(start)->getCoordinates());
                    std::cout << "ici ?";
                    //Decalage
                    map.getInsectAt(start)->getPossibleMovements(map);
                    std::cout << "et Ici ?";
                    player_.inputs.setPossibleDestinations(map.getInsectAt(start)->getPossibleMovements(map));
                    player_.inputs.noNeedForPossibleDestinationsUpdate();
                    return 0;
                }
                else if (loc == player_.getId()){
                    std::cout << "loc1 ou deux";
                    //player_.inputs.setPossibleDestinations(map.getInsectAt(player_.inputs.getStart())->setRule(map));
                    auto possiblesDestinations = map.setRule(player_.getId());

                    player_.inputs.setPossibleDestinations(map.setRule(player_.getId()));
                    //player_.inputs.setPossibleDestinations(map.setRule((player_.getId())%2));
                    //player_.inputs.setPossibleDestinations(std::vector<vec2i>{{15,15},{16,16},{14,14}});

                    for(auto & a : player_.inputs.getPossibleDestinations()){
                        std::cout << a << " \n";
                    }

                    player_.inputs.noNeedForPossibleDestinationsUpdate();
                    return 0;
                }
                else{
                    //Wrong deck selected;
                    //throw HiveException("solver.h:Solver:update", "cursor 1 invalid");
                    return -1;
                }
            }

            std::cout << "::::" << player_.inputs.getStart();
            return -1;
        }
        else{
            std::cout << "possibleDestinationsIsNotNeeded\n";
            std::cout << player_.inputs.isStartSelected();
            std::cout << player_.inputs.isDestinationSelected();
            if(player_.inputs.movementNeeded()){
                std::cout << "movementNeeded\n";
                std::cout << "for start : " << isStartValid(player_) << '\n';
                std::cout << "for destination : " << isDestinationValid(player_) << '\n';
                if (isStartValid(player_) && isDestinationValid(player_)){
                    std::cout << "Both cursor or valid\n";

                    int loc = getStartLocation(player_);

                    std::cout << "loc" << loc << " id :" << player_.getId() <<  "\n";
                    std::cout << "start :" << player_.inputs.getStart();

                    if (loc == 0){
                        if (map.getInsectAt(player_.inputs.getStart() + vec2i{offset,offset})->getColor() == player_.getId()%2){
                            std::cout << "good color";
                            std::cout << "map to map";
                            mapToMapMovement(player_);
                            return 1;
                        }
                        else return -1;
                    }
                    else if (loc == player_.getId()){
                        std::cout << "deck to map";
                        deckToMapMovement(player_);
                        return 1;
                    }
                    else{
                        //Wrong deck selected;
                        //throw HiveException("solver.h:Solver:update", "cursor 1 invalid");
                        return -1;
                    }
                }
                else throw HiveException("solver.h:Solver:update", "start or destination invalid for map");
            }
        }
        return 0;
    }

};

#endif //HIVE_SOLVER_H
