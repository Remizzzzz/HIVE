//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_HIVE_H
#define HIVE_HIVE_H

#include <vector>

#include "utils/hiveException.h"

#include "features/insect.h"
#include "features/deck.h"

#include "inputsManager.h"
#include "solver.h"
#include "renderer.h"

//enum Mode{PvP,PvAI};
//enum Version{console, graphic};

class Hive{

    Mode mode;
    Version version;

    std::vector<Insect> insects;

    const int trueMapSideSize{32};
    const int renderedMapSideSize{28};
    int rewindNb;
    Map map;

    Deck deck1;
    Deck deck2;

    Inputs inputs;

    InputsManager inputsManager;

    Solver solver;

    Renderer * renderer;


public:

    Hive(Mode mode_, Version version_, int rewindNb_ = 3)
            :   mode(mode_), version(version_),
                insects(),
                rewindNb(rewindNb_), map(trueMapSideSize,rewindNb),
                inputs(),
                deck1(),
                deck2(),
                inputsManager(mode, inputs),
                solver(map, inputs, deck1, deck2, trueMapSideSize)
    {
        switch (version) {
            case console:
                renderer = new ConsoleRenderer(map);
                break;
            case graphic:
                renderer = new GraphicRenderer(map);
                break;
            default:
                renderer = nullptr;
                throw HiveException("hive.h:Hive", "invalid version");
        }
    };


    void run(){
        inputsManager.updateInputs();
        solver.update();
        renderer->render(inputs);
    }
};


#endif //HIVE_HIVE_H
