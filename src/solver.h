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

    Deck & deck1;
    Deck & deck2;

    const int & trueMapSideSize;

public:

    Solver(Map & map_, Deck & deck1_, Deck & deck2_, const int & trueMapSideSize_) :
        map(map_), deck1(deck1_), deck2(deck2_), trueMapSideSize(trueMapSideSize_){};


private:
    //0 -> no, 1 -> deck 1, -> 1 deck 2
    int isCursor1FromDeck(const Inputs & inputs_) const{
        return 1 * (inputs_.cursor1.getI() == -1) + 2 * (inputs_.cursor1.getI() == trueMapSideSize);
    }

    bool isCursor1Valid(const Inputs & inputs_) const{
        return inputs_.cursor1.getI() >= 0 && inputs_.cursor1.getI() < trueMapSideSize &&
               inputs_.cursor1.getJ() >= 0 && inputs_.cursor1.getJ() < trueMapSideSize;
    }

    bool isCursor2Valid(const Inputs & inputs_) const{
        return inputs_.cursor2.getI() >= 0 && inputs_.cursor2.getI() < trueMapSideSize &&
                inputs_.cursor2.getJ() >= 0 && inputs_.cursor2.getJ() < trueMapSideSize;
    }

public:

    void update(Inputs & inputs_){
        if(inputs_.movementNeeded()){
            if (isCursor1Valid(inputs_) && isCursor2Valid(inputs_)){
                vec2i & cursor1 = inputs_.cursor1;
                vec2i & cursor2 = inputs_.cursor2;
                switch (isCursor1FromDeck(inputs_)) {
                    case 1:
                        if (!deck1.isIndexValid(cursor1.getJ())){
                            if (map.isSlotFree(cursor2)){
                                map.putInsect(deck1.getInsectAt(cursor1.getJ()),cursor2);
                                deck1.deleteAt(cursor1.getJ());
                            }
                            else if (deck1.getInsectAt(cursor1.getJ())->getIT() == grasshoper) {
                                //---------------A FAIRE--------------------
                                /*
                                 deck1.getInsectAt(cursor1.getJ())
                                map.putInsect(deck1.getInsectAt(cursor1.getJ()),cursor2);
                                deck1.deleteAt(cursor1.getJ());
                                 */
                            }
                        }
                        else throw HiveException("solver.h:Solver:update", "cursor1 is invalid for deck1")



                    
                }
                
                
                
            }
            else throw HiveException("solver.h:Solver:update", "cursor1 or cursor2 invalid for map");


        }
    }

};

#endif //HIVE_SOLVER_H
