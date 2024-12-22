//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_HIVE_H
#define HIVE_HIVE_H

#include <vector>
#include <set>
#include <string>
#include <QDebug>

#include "features/insect.h"
#include "inputsManager.h"
#include "solver.h"
#include "renderer.h"

#include "features/AI.h"



enum Version{console, graphic};

class Hive{
private:
    bool isInit = false;
    bool menuPart = true;
    bool gamePart = false;

    std::set<insectType> extensions;
    Mode mode;
    Version version;

    std::vector<Insect *> insects;

    int trueMapSideSize{32};
    int renderedMapSideSize{30};
    int offset;
    int rewindNb;
    int rewindUsed;
    Map map;

    Player player1;
    Player player2;

    const Bee * bee1;
    const Bee * bee2;

    Player * currentPlayer;

    InputsManager inputsManager;

    Solver solver;

    Renderer * renderer;


    std::vector<Insect *>& getInsects() { return insects; }
    Insect* generateSingleInsect(int type, bool color, vec2i vec);
    void generateAllInsects(bool Lad=false, bool Mos=false);

    void static resetInputs(Player & player_) { player_.inputs.reset(); }

public:
    Renderer* getRenderer() const { return renderer; }
    void displayMenu();
    Hive() : mode(PvP), version(console), insects(), rewindNb(5),rewindUsed(rewindNb),
            offset((trueMapSideSize - renderedMapSideSize) / 2.f), map(trueMapSideSize,rewindNb),
             player1(1), player2(2), currentPlayer(&player1), inputsManager(mode, renderedMapSideSize, map),
             solver(map, renderedMapSideSize, offset),renderer( nullptr) {}

    ~Hive() {
        delete renderer;
        for (const auto& insect : insects) delete insect;
    }

    int getExtension() {
        bool lady=false;
        bool mosq=false;
        if (extensions.find(ladybug)!=extensions.end()) {
            lady=true;
        }
        if (extensions.find(mosquitoe)!=extensions.end()) {
            mosq=true;
        }
        if (!lady && !mosq) return 0;
        if (lady && !mosq) return 1;
        if (!lady && mosq) return 2;
        if (lady && mosq) return 3;
        else return 300;
    }
    int getRewindUsed() {return rewindUsed;}
    void incrRewindUsed() {rewindUsed++;}
    void decrRewindUsed() {if (rewindUsed>0) rewindUsed--;}
    int getRewindMax() {return rewindNb;}
    void setRewindNumber(int newRewindNumber) {
        rewindNb = newRewindNumber;
        rewindUsed= rewindNb;
        map.setRewind(rewindNb);
    }
    void setRewindUsed(int newRewindUsed) {
        rewindUsed = newRewindUsed;
    }
    //int launchGame();
    void static displayRules() ;
    void changeSettings();
    void saveGame(const std::string& filename) const;
    void loadGame(const std::string& filename);
    int run();

    void resetInputs() {
        player1.inputs.reset();
        player1.inputs.setStart({!player1.getDeck().isEmpty() * -1,0});

        player2.inputs.reset();
        player2.inputs.setStart({player2.getDeck().isEmpty() * -1 + renderedMapSideSize,0});
    }

    void resetPlayerInputs(Player & player_) {
        player_.inputs.reset();
        if (player_.getId() == 1) player_.inputs.setStart({!player1.getDeck().isEmpty() * -1,0});
        else player_.inputs.setStart({player2.getDeck().isEmpty() * -1 + renderedMapSideSize,0});
    }

    void initIfNeeded() {
        if (!isInit) {
            renderer = new ConsoleRenderer(map, &player1, &player2, renderedMapSideSize, offset);
            generateAllInsects();
            isInit = true;
        }

        if (mode == PvAI) player2.setHumanity(false);
        else player2.setHumanity(true);

        resetInputs();
    }

    Player* getPlayer1() { return &player1; }
    Player* getPlayer2() { return &player2; }
    InputsManager* getInputsManager() { return &inputsManager; }
    Solver* getSolver() { return &solver; }
    Map& getMap(){ return map; }
    Mode getMode() { return mode; }
    int getRenderedMapSideSize() const{ return renderedMapSideSize; }
    bool getCurrentPlayer() { return currentPlayer->getId(); }
    void switchPlayer() {
        if (currentPlayer == &player1) currentPlayer = &player2;
        else currentPlayer = &player1;
    }
    int getOffset() {return offset;}
    void runQt(bool Lady, bool Mosq) {
        if (Lady) {
            extensions.emplace(ladybug);
        }
        if (Mosq) {
            extensions.emplace(mosquitoe);
        }
        generateAllInsects(Lady, Mosq);
    }
};


#endif //HIVE_HIVE_H
