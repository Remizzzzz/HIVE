//
// Created by joanc on 22/12/2024.
//


#include <QDebug>

#include "features/insect.h"
#include "features/map.h"
#include "features/player.h"
#include "utils/utils.h"
#include "utils/hiveException.h"
#include "solver.h"



void Solver::deckToMapMovement(Player & player_) {
    const vec2i & start = player_.inputs.getStart();
    const vec2i & destination = player_.inputs.getDestination();//+vec2i{offset,offset};
    if (player_.getDeck().isIndexValid(start.getJ())){
        if (map.isSlotFree(destination)) {
            map.putInsectTo(player_.getDeck().getInsectAt(start.getJ()), destination);

            map.getInsectAt(destination)->setCoordinates(destination);
            map.addToHistoric(start,destination);//If the movement is a rewind, goBack will manage the historic
            player_.addActiveInsectsFromDeck(start.getJ());
            player_.deck.removeAt(start.getJ());
            map.getInsectAt(destination)->setCoordinates(destination);
            turn++;
        } else {
            player_.inputs.setMessage("Can't put your insect here");
        }
    } else {
        throw HiveException("solver.h:Solver:deckToMapGestion", "cursor1 is invalid for deck1");
    }
}


int Solver::update(Player & player_) {
    if (player_.inputs.isLeaveNeeded()) return 2;
    else if (player_.inputs.isRewindNeeded()) map.goBack();

    else if (player_.inputs.isPossibleDestinationsNeeded()) {
        std::cout << "possibleDestinationsNeeded\n";
        if (isStartValid(player_)) {
            std::cout << "startValid\n" ;
            int loc = getStartLocation(player_);
            std::cout << "loc : " << loc << "\n";

            if (loc == 0) {
                // décalage pour qu'on ne travaille pas sur les bords de la map
                vec2i start = player_.inputs.getStart() + vec2i{offset,offset};

                std::cout << "loc0";
                std::cout << "ici ?.";
                std::cout << (map.getInsectAt(start) == nullptr);
                std::cout << (map.getInsectAt(start)->getCoordinates());
                std::cout << "ici ?";
                // décalage
                map.getInsectAt(start)->getPossibleMovements(map);
                std::cout << "et Ici ?";
                player_.inputs.setPossibleDestinations(map.getInsectAt(start)->getPossibleMovements(map));
                player_.inputs.noNeedForPossibleDestinationsUpdate();
                return 0;

            } else if (loc == player_.getId()) {
                std::cout << "loc1 ou deux";
                auto possiblesDestinations = map.setRule(player_.getId());

                player_.inputs.setPossibleDestinations(map.setRule(player_.getId()));

                for(auto & a : player_.inputs.getPossibleDestinations()) std::cout << a << " \n";

                player_.inputs.noNeedForPossibleDestinationsUpdate();
                return 0;
            } else{
                return -1;
            }
        }
        return -1;

    } else {
        std::cout << "possibleDestinationsIsNotNeeded\n";
        if(player_.inputs.movementNeeded()) {
            std::cout << "movementNeeded\n";
            if (isStartValid(player_) && isDestinationValid(player_)) {
                std::cout << "Both cursor or valid\n";
                int loc = getStartLocation(player_);

                if (loc == 0) {
                    if (map.getInsectAt(player_.inputs.getStart())->getColor() == player_.getId()) {
                        std::cout << "good color";
                        std::cout << "map to map";
                        mapToMapMovement(player_);
                        return 1;
                    } else return -1;
                } else if (loc == player_.getId()) {
                    std::cout << "deck to map";
                    deckToMapMovement(player_);
                    return 1;
                } else {
                    return -1;
                }
            } else throw HiveException("solver.h:Solver:update", "start or destination invalid for map");
        }
    }

    return 0;
}

