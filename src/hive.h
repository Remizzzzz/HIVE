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
enum Version{console, graphic};

class Hive{

    Mode mode;
    Version version;

    std::vector<Insect> insects;

    const int trueMapSideSize{32};
    const int renderedMapSideSize{30};
    int rewindNb;
    Map map;

    Player player1;
    Player player2;

    Player * currentPlayer;

    InputsManager inputsManager;

    Solver solver;

    Renderer * renderer;


private:

    void switchPlayer(){
        if (currentPlayer == &player1){
            currentPlayer = &player2;
        }
        else{
            currentPlayer = &player1;
        }
    }

    void resetInputs(Player & player_){
        player_.inputs.reset();
    }

public:

    Hive(Mode mode_, Version version_, int rewindNb_ = 3)
            :   mode(mode_), version(version_),
                insects(),
                rewindNb(rewindNb_), map(trueMapSideSize,rewindNb),
                player1(1), player2(2), currentPlayer(&player1),
                inputsManager(mode, renderedMapSideSize, map),
                solver(map, trueMapSideSize)
    {


        switch (version) {
            case console:
                //renderer = new ConsoleRenderer(map);
                break;
            case graphic:
                //renderer = new GraphicRenderer(map);
                break;
            default:
                renderer = nullptr;
                throw HiveException("hive.h:Hive", "invalid version");
        }
    };

    void run(){



        inputsManager.updateAIInputs(*currentPlayer);
        std::cout << currentPlayer->inputs;

        switch (solver.update(*currentPlayer)) {
            case 0:
                //le mouvement est pas bon
                resetInputs(*currentPlayer);
            case 1:
                //Le travail est en cours
            case 2:
                //mouvement fait
                resetInputs(*currentPlayer);
                switchPlayer();
        }


        inputsManager.updateAIInputs(*currentPlayer);
        std::cout << currentPlayer->inputs;
        //renderer->render(inputs);
    }
};


#endif //HIVE_HIVE_H
