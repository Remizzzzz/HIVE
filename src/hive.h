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

    int trueMapSideSize{32};
    int renderedMapSideSize{30};
    int rewindNb;
    int rewindUsed;
    Map map;

    Player player1;
    Player player2;

    Player * currentPlayer=&player1;

    InputsManager inputsManager;

    Solver solver;

    Renderer * renderer;

private:

    void generateAllInsects(){

        int cpt1 = 0;
        int cpt2 = 0;

        for (int i = 0; i < 2; ++i) {
            bool color = bool(int (float (i) / 1.f));
            Insect * insect = new Bee(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

        for (int i = 0; i < 6; ++i) {
            bool color = bool(int (float (i) / 3.f));
            Insect * insect = new Ant(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Beetle(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

        for (int i = 0; i < 6; ++i) {
            bool color = bool(int (float (i) / 3.f));

            Insect * insect = new Grasshopper(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Spider(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Mosquitoe(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

        for (int i = 0; i < 4; ++i) {
            bool color = bool(int (float (i) / 2.f));

            Insect * insect = new Ladybug(color);
            insects.push_back(insect);

            if (color){
                insect->setCoordinates({-1,cpt1});
                player1.deck.addInsect(insect);
                cpt1++;
            }
            else{
                insect->setCoordinates({renderedMapSideSize,cpt2});
                player2.deck.addInsect(insect);
                cpt2++;
            }
        }

    }


    void static resetInputs(Player & player_){
        player_.inputs.reset();
    }




    int displayMenu();

public:
    Hive() : mode(PvP), version(console),
             insects(),
             rewindNb(5),rewindUsed(rewindNb), map(trueMapSideSize,rewindNb),
             player1(1), player2(2), currentPlayer(&player1),
             inputsManager(mode, renderedMapSideSize, map),
             solver(map, renderedMapSideSize),renderer( nullptr){}



    ~Hive(){
        delete renderer;
        for (const auto& insect : insects) {
            delete insect;
        }
    }
    int getRewindUsed() {return rewindUsed;}
    void incrRewindUsed() {rewindUsed++;}
    void decrRewindUsed() {if (rewindUsed>0) rewindUsed--;}
    int getRewindMax() {return rewindNb;}
    void setRewindNumber(int newRewindNumber) {
        rewindNb = newRewindNumber;
        rewindUsed= rewindNb;
    }
    int launchGame();
    void static displayRules() ;
    void changeSettings();
    void saveGame(const std::string& filename) const;
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
            }else{
                return 0;
            }

        }
        return 2;
    }
    int getRenderedMapSideSize() const {return renderedMapSideSize;}
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

        std::cout << "\n:" <<currentPlayer->inputs;

        switch (solver.update(*currentPlayer)) {
            case -1:
                std::cout << "\n---Reset---\n";
                //le mouvement est pas bon
                resetInputs(*currentPlayer);
                break;
            case 0:
                //Le travail est en cours
                break;
            case 1:
                //mouvement fait
                std::cout << "\n---Deplacement---\n";
                resetInputs(*currentPlayer);
                switchPlayer();
                break;
            default:
                throw HiveException("hive.h:Hive", "retour de run mauvais");
        }
        std::cout << "\n:" <<currentPlayer->inputs.getStart();

        renderer->render(*currentPlayer);

        return 1;
    }

    void runQt() {
        generateAllInsects();
    }
    Player* getPlayer1() {return &player1;}
    Player* getPlayer2() {return &player2;}
    InputsManager* getInputsManager() {return &inputsManager;}
    Solver* getSolver() {return &solver;}
    Map& getMap(){return map;}
    bool getCurrentPlayer(){return currentPlayer->getId();}
    void switchPlayer(){
        if (currentPlayer == &player1){
            currentPlayer = &player2;
        }
        else{
            currentPlayer = &player1;
        }
    }
};


#endif //HIVE_HIVE_H
