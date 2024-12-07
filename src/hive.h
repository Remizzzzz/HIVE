//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_HIVE_H
#define HIVE_HIVE_H

#include <vector>
#include <set>
#include "utils/hiveException.h"

#include "features/insect.h"
#include "features/deck.h"

#include "inputsManager.h"
#include "solver.h"
#include "renderer.h"

//enum Mode{PvP,PvAI};
enum Version{console, graphic};

class Hive{

    bool isInit = false;
    std::set<insectType> extensions;
    Mode mode;
    Version version;

    std::vector<Insect *> insects;

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

    void generateAllInsects(){

        for (int i = 0; i < 6; ++i) {
            bool color = bool(int (float (i) / 3.f));
            Insect * insect = new Ant(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Beetle(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

        for (int i = 0; i < 6; ++i) {
            bool color = bool(int (float (i) / 3.f));

            Insect * insect = new Grasshopper(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

        for (int i = 0; i < 2; ++i) {
            bool color = bool(int (float (i) / 1.f));
            Insect * insect = new Bee(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Spider(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Mosquitoe(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Ladybug(color);
            insects.push_back(insect);

            if (color){
                player1.deck.addInsect(insect);
            }
            else{
                player2.deck.addInsect(insect);
            }
        }

    }

    void switchPlayer(){
        if (currentPlayer == &player1){
            currentPlayer = &player2;
        }
        else{
            currentPlayer = &player1;
        }
    }

    void static resetInputs(Player & player_){
        player_.inputs.reset();
    }

    int displayMenu();

public:
    Hive() : mode(PvP), version(console),
                insects(),
                rewindNb(1), map(trueMapSideSize,rewindNb),
                player1(1), player2(2), currentPlayer(&player1),
                inputsManager(mode, renderedMapSideSize, map),
                solver(map, trueMapSideSize),renderer( nullptr){}

    /*Hive(Mode mode_, Version version_, int rewindNb_ = 3)
            :   mode(mode_), version(version_),
                insects(),
                rewindNb(rewindNb_), map(trueMapSideSize,rewindNb),
                player1(1), player2(2), currentPlayer(&player1),
                inputsManager(mode, renderedMapSideSize, map),
                solver(map, trueMapSideSize)
    {
    };*/

    ~Hive(){
        delete renderer;
    }
    void static displayRules() ;
    void changeSettings();
    void saveGame(const std::string& filename)const;
    void loadGame(const std::string& filename);
    int initIfNeeded(){
        if (!isInit){
            if (displayMenu() == 1){
                if (mode == PvAI){
                    player2.setHumanity(false);
                }
                generateAllInsects();
                std::cout << insects.size();
                renderer->displayMap(*currentPlayer);
                isInit = true;
                return 1;
            }
            else{
                return 0;
            }

        }
        return 2;
    }

    int run(){
        if (initIfNeeded() == 0){
            return 0;
        }

        if (currentPlayer->isHuman){
            inputsManager.updatePlayerInputs(*currentPlayer);
        }
        else{
            inputsManager.updateAIInputs(*currentPlayer);
        }

        std::cout << "\n:" <<currentPlayer->inputs.getStart();

        switch (solver.update(*currentPlayer)) {
            case 0:
                //le mouvement est pas bon
                resetInputs(*currentPlayer);
                break;
            case 1:
                //Le travail est en cours
                    break;
            case 2:
                //mouvement fait
                resetInputs(*currentPlayer);
                switchPlayer();
                break;
            default:
                throw HiveException("hive.h:Hive", "retour de run mauvais");
        }
        std::cout << "\n:" <<currentPlayer->inputs.getStart();

        renderer->displayMap(*currentPlayer);

        return 1;
    }


};


#endif //HIVE_HIVE_H
