//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_SOLVER_H
#define HIVE_SOLVER_H


#include "features/insect.h"
#include "features/map.h"
#include "features/deck.h"
#include "features/player.h"

#include "utils/hiveException.h"

class Solver{

private:
    Map & map;

    const int & trueMapSideSize;

public:

    Solver(Map & map_, const int & trueMapSideSize_) :
        map(map_), trueMapSideSize(trueMapSideSize_){};


private:
    //0 -> map, 1 -> deck 1, -> 1 deck 2
    int getStartLocation(Player & player_) const{
        return 1 * (player_.inputs.getStart().getI() == -1) + 2 * (player_.inputs.getStart().getI() == trueMapSideSize);
    }

    bool isStartValid(Player & player_) const{
        return player_.inputs.getStart().getI() >= -1 && player_.inputs.getStart().getI() <= trueMapSideSize &&
               player_.inputs.getStart().getJ() >= 0 && player_.inputs.getStart().getJ() < trueMapSideSize &&
               !map.isSlotFree(player_.inputs.getStart());
    }

    bool isDestinationValid(Player & player_) const{
        return player_.inputs.getDestination().getI() >= 0 && player_.inputs.getDestination().getI() < trueMapSideSize &&
                player_.inputs.getDestination().getJ() >= 0 && player_.inputs.getDestination().getJ() < trueMapSideSize;
    }
public:
    void deckToMapMovement(Player & player_) {
        const vec2i & start = player_.inputs.getStart();
        const vec2i & destination = player_.inputs.getDestination();

        if (!player_.getDeck().isIndexValid(start.getJ())) {
            if (map.isSlotFree(destination)) {
                map.putInsectTo(player_.getDeck().getInsectAt(start.getJ()), destination);
                player_.addActiveInsectsFromDeck(start.getJ());
                player_.deck.removeAt(start.getJ());
            } else if (player_.getDeck().getInsectAt(start.getJ())->getIT() == grasshopper) {
                //---------------A FAIRE--------------------
                /*
                 deck1.getInsectAt(cursor1.getJ())
                map.putInsect(deck1.getInsectAt(cursor1.getJ()),destination);
                deck1.deleteAt(cursor1.getJ());
                 */
            }
            else{
                player_.inputs.setMessage("Can't put your insect here");
            }
        } else throw HiveException("solver.h:Solver:deckToMapGestion", "cursor1 is invalid for deck1");
    }

    void mapToMapMovement(Player & player_){
        const vec2i & start = player_.inputs.getStart();
        const vec2i & destination = player_.inputs.getDestination();

        if (!map.isSlotFree(start)){
            if (map.isSlotFree(destination)){
                map.moveInsect(start,destination);
            }
            else if (player_.getDeck().getInsectAt(start.getJ())->getIT() == grasshopper) {
                //---------------A FAIRE--------------------
                /*
                 deck2.getInsectAt(cursor1.getJ())
                map.putInsect(deck2.getInsectAt(cursor1.getJ()),destination);
                deck2.deleteAt(cursor1.getJ());
                 */
            }
            else{
                player_.inputs.setMessage("Can't put your insect here");
            }
        }
    }


    int update(Player & player_){

        if (player_.inputs.isPossibleDestinationsNeeded()){
            if (isStartValid(player_)){
                std::cout << "i" ;
                int loc = getStartLocation(player_);

                if (loc == 0){
                    //Il faudrait plutot appeler une fonction qui dit ou on peut poser une piece depuis le deck
                    player_.inputs.setPossibleDestinations(map.getInsectAt(player_.inputs.getStart())->getPossibleMovements(map));
                    player_.inputs.noNeedForPossibleDestinationsUpdate();
                    return 1;
                }
                else if (loc == player_.getId()){
                    player_.inputs.setPossibleDestinations(map.getInsectAt(player_.inputs.getStart())->getPossibleMovements(map));
                    player_.inputs.noNeedForPossibleDestinationsUpdate();
                    return 1;
                }
                else{
                    //Wrong deck selected;
                    //throw HiveException("solver.h:Solver:update", "cursor 1 invalid");
                }
            }
        }
        else{
            if(player_.inputs.movementNeeded()){
                if (isStartValid(player_) && isDestinationValid(player_)){
                    if (map.getInsectAt(player_.inputs.getStart())->getColor() == player_.getId()){
                        int loc = getStartLocation(player_);

                        if (loc == 0){
                            mapToMapMovement(player_);
                            return 2;
                        }
                        else if (loc == player_.getId()){
                            deckToMapMovement(player_);
                            return 2;
                        }
                        else{
                            //Wrong deck selected;
                            //throw HiveException("solver.h:Solver:update", "cursor 1 invalid");
                        }
                    }
                }
                else throw HiveException("solver.h:Solver:update", "start or destination invalid for map");
            }
        }
        return 0;
    }

};

#endif //HIVE_SOLVER_H
