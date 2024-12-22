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
    const vec2i & destination = player_.inputs.getDestination();
    if (player_.getDeck().isIndexValid(start.getJ())){
        if (map.isSlotFree(destination)) {
            //qDebug()<<"\n\nC'est free";
            map.putInsectTo(player_.getDeck().getInsectAt(start.getJ()), destination);

            map.getInsectAt(destination)->setCoordinates(destination);
            map.addToHistoric(start,destination);//If the movement is a rewind, goBack will manage the historic
            player_.addActiveInsectsFromDeck(start.getJ());
            player_.deck.removeAt(start.getJ());
            turn++;
        } else {
            player_.inputs.setMessage("Can't put your insect here");
        }
    } else {
        throw HiveException("solver.h:Solver:deckToMapGestion", "cursor1 is invalid for deck1");
    }
}


int Solver::update(Player & player_){

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
            if (isStartValid(player_)){
                int loc = getStartLocation(player_);

                if (loc == 0){

                    //Decalage pour que l'on travail pas sur les bords de la map
                    vec2i start = player_.inputs.getStart() + vec2i{offset,offset};

                    if (!map.isSlotFree(start))
                    {
                        map.getInsectAt(start)->getPossibleMovements(map);
                        player_.inputs.setPossibleDestinations(map.getInsectAt(start)->getPossibleMovements(map));
                        player_.inputs.noNeedForPossibleDestinationsUpdate();
                        return 0;
                    }
                    else return -1;

                }
                else if (loc == player_.getId()){

                    auto possiblesDestinations = map.setRule(player_.getId() % 2);

                    player_.inputs.setPossibleDestinations(possiblesDestinations);

                    player_.inputs.noNeedForPossibleDestinationsUpdate();
                    return 0;
                }
                else{
                    return -1;
                }
            }

            return -1;
        }
        else{
            if(player_.inputs.movementNeeded()){
                if (isStartValid(player_) && isDestinationValid(player_)){

                    const int loc = getStartLocation(player_);

                    if (loc == 0){
                        if (map.getInsectAt(player_.inputs.getStart() + vec2i{offset,offset})->getColor() == player_.getId()%2){
                            mapToMapMovement(player_);
                            return 1;
                        }
                        else return -1;
                    }
                    else if (loc == player_.getId()){
                        deckToMapMovement(player_);
                        return 1;
                    }
                    else{
                        //Wrong deck selected;
                        //throw HiveException("solver.h:Solver:update", "cursor 1 invalid");
                        return -1;
                    }
                }
                return -1;
            }
        }
        return 0;
    }

