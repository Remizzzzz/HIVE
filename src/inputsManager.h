//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_INPUTMANAGER_H
#define HIVE_INPUTMANAGER_H

#include <QDebug>

#include "features/inputs.h"
#include "features/player.h"
#include "utils/utils.h"
#include "utils/random.hpp"
#include <features/map.h>
#include <algorithm>


enum Mode{PvP,PvAI};

class InputsManager{
private:
    const int renderedMapSideSize;
    Map & map;
    lv::Random random;
    void moveCursor(Player & player, const int cursorId_, const int i_, const int j_) const;

public:
    Map& getMap() { return map; }
    void setMap(Map map1) { map = map1; }
    explicit InputsManager(Mode mode_, const int renderedMapSideSize_, Map & map_):
    renderedMapSideSize(renderedMapSideSize_), map(map_), random() {}

    template <class THINKINGFONCTION>
        void updateAIInputs2(Player & player_, THINKINGFONCTION tf, bool Qt=false, bool inputT=false)
    {
        tf(map,player_, player_.inputs, random, Qt, inputT);
    }
    void updatePlayerInputs(Player & player_);
    void updatePlayerInputsQt(Player * player_, vec2i clickedPos, bool inputT, bool turnP);
    void resetPlayerInputs(Player* player_) { player_->inputs.resetQt(); }
    void convertQtToSolver(Player* player_);
    void convertSolverToQt(Player* player_);
};


#endif //HIVE_INPUTMANAGER_H
