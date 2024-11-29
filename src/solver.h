//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_SOLVER_H
#define HIVE_SOLVER_H


#include "features/insect.h"
#include "features/map.h"
#include "features/deck.h"
#include "features/inputs.h"

#include "utils/hiveException.h"

class Solver{

private:
    Map & map;

    Inputs & inputs;

    Deck & deck1;
    Deck & deck2;

    const int & trueMapSideSize;

public:

    Solver(Map & map_, Inputs & inputs_, Deck & deck1_, Deck & deck2_, const int & trueMapSideSize_) :
        map(map_), inputs(inputs_), deck1(deck1_), deck2(deck2_), trueMapSideSize(trueMapSideSize_){};


private:
    //0 -> map, 1 -> deck 1, -> 1 deck 2
    int getCursor1Location() const{
        return 1 * (inputs.cursor1.getI() == -1) + 2 * (inputs.cursor1.getI() == trueMapSideSize);
    }

    bool isCursor1Valid() const{
        return inputs.cursor1.getI() >= -1 && inputs.cursor1.getI() <= trueMapSideSize &&
               inputs.cursor1.getJ() >= 0 && inputs.cursor1.getJ() < trueMapSideSize;
    }

    bool isCursor2Valid() const{
        return inputs.cursor2.getI() >= 0 && inputs.cursor2.getI() < trueMapSideSize &&
                inputs.cursor2.getJ() >= 0 && inputs.cursor2.getJ() < trueMapSideSize;
    }

    void deckToMapMovement(int deckId_, const vec2i & cursor1_, const vec2i & cursor2_) {
        switch (deckId_) {
            case 1:
                if (!deck1.isIndexValid(cursor1_.getJ())) {
                    if (map.isSlotFree(cursor2_)) {
                        map.putInsectTo(deck1.getInsectAt(cursor1_.getJ()), cursor2_);
                        deck1.removeAt(cursor1_.getJ());
                    } else if (deck1.getInsectAt(cursor1_.getJ())->getIT() == grasshopper) {
                        //---------------A FAIRE--------------------
                        /*
                         deck1.getInsectAt(cursor1.getJ())
                        map.putInsect(deck1.getInsectAt(cursor1.getJ()),cursor2);
                        deck1.deleteAt(cursor1.getJ());
                         */
                    }
                    else{
                        inputs.message = "Can't put your insect here";
                    }
                } else throw HiveException("solver.h:Solver:deckToMapGestion", "cursor1 is invalid for deck1");
                break;
            case 2:
                if (!deck2.isIndexValid(cursor1_.getJ())) {
                    if (map.isSlotFree(cursor2_)) {
                        map.putInsectTo(deck2.getInsectAt(cursor1_.getJ()), cursor2_);
                        deck2.removeAt(cursor1_.getJ());
                    } else if (deck2.getInsectAt(cursor1_.getJ())->getIT() == grasshopper) {
                        //---------------A FAIRE--------------------
                        /*
                         deck2.getInsectAt(cursor1.getJ())
                        map.putInsect(deck2.getInsectAt(cursor1.getJ()),cursor2);
                        deck2.deleteAt(cursor1.getJ());
                         */
                    }
                    else{
                        inputs.message = "Can't put your insect here";
                    }
                } else throw HiveException("solver.h:Solver:deckToMapGestion", "cursor1 is invalid for deck2");
                break;
            default:
                throw HiveException("solver.h:Solver:deckToMapGestion", "deckId_ is invalid");
                break;
        }
    }

    void mapToMapMovement(const vec2i & cursor1_, const vec2i & cursor2_){
        if (!map.isSlotFree(cursor1_)){
            if (map.isSlotFree(cursor2_)){
                map.moveInsect(cursor1_,cursor2_);
            }
            else if (deck2.getInsectAt(cursor1_.getJ())->getIT() == grasshopper) {
                //---------------A FAIRE--------------------
                /*
                 deck2.getInsectAt(cursor1.getJ())
                map.putInsect(deck2.getInsectAt(cursor1.getJ()),cursor2);
                deck2.deleteAt(cursor1.getJ());
                 */
            }
            else{
                inputs.message = "Can't put your insect here";
            }
        }
    }

    void resetInputs(){
        inputs = Inputs();
    }

public:

    void update(bool player_){
        if(inputs.movementNeeded()){
            if (isCursor1Valid() && isCursor2Valid()){
                if (map.getInsectAt(inputs.cursor1)->getColor() == player_){
                    switch (getCursor1Location()) {
                        case 0:
                            mapToMapMovement(inputs.cursor1,inputs.cursor2);
                            break;
                        case 1:
                            deckToMapMovement(1,inputs.cursor1,inputs.cursor2);
                            break;
                        case 2:
                            deckToMapMovement(2,inputs.cursor1,inputs.cursor2);
                            break;
                        default:
                            throw HiveException("solver.h:Solver:update", "cursor 1 invalid");
                            break;
                    }
                    resetInputs();
                }
            }
            else throw HiveException("solver.h:Solver:update", "cursor1 or cursor2 invalid for map");
        }
    }

};

#endif //HIVE_SOLVER_H
