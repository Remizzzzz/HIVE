//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_INPUTMANAGER_H
#define HIVE_INPUTMANAGER_H

#include "features/inputs.h"
#include "features/deck.h"

#include "utils/utils.h"
#include "utils/hiveException.h"
#include "utils/random.hpp"

#include <iostream>
#include <conio.h>


enum Mode{PvP,PvAI};


class InputsManager{

private:
    Mode mode;
    Inputs & inputs;
    int switcher;

    const int renderedMapSideSize;
    Deck & deck1;
    Deck & deck2;

    lv::Random random;

private:

    void randomUpdateOfCursor(const int cursorId_){
        switch (cursorId_) {
            case 1:
                switch (random.getRandomInt(0,3)) {
                    case 0:
                        inputs.cursor1.setI(-1);
                        inputs.cursor1.setJ(random.getRandomInt(0,deck1.getInsectNb()));
                        break;
                    case 1:
                        inputs.cursor1.setI(random.getRandomInt(0,renderedMapSideSize));
                        inputs.cursor1.setJ(random.getRandomInt(0,renderedMapSideSize));
                        break;
                    case 2:
                        inputs.cursor1.setI(renderedMapSideSize);
                        inputs.cursor1.setJ(random.getRandomInt(0,deck2.getInsectNb()));
                        break;
                    default:
                        throw HiveException("inputsManager.h:InputsManager:randomUpdateOfCursor", "wrong random number for cursor1");
                        break;
                }
                break;
            case 2:
                switch (random.getRandomInt(0,3)) {
                    case 0:
                        inputs.cursor2.setI(-1);
                        inputs.cursor2.setJ(random.getRandomInt(0,deck1.getInsectNb()));
                        break;
                    case 1:
                        inputs.cursor2.setI(random.getRandomInt(0,renderedMapSideSize));
                        inputs.cursor2.setJ(random.getRandomInt(0,renderedMapSideSize));
                        break;
                    case 2:
                        inputs.cursor2.setI(renderedMapSideSize);
                        inputs.cursor2.setJ(random.getRandomInt(0,deck2.getInsectNb()));
                        break;
                    default:
                        throw HiveException("inputsManager.h:InputsManager:randomUpdateOfCursor", "wrong random number for cursor1");
                        break;
                }
                break;
            default:
                throw HiveException("inputsManager.h:InputsManager:randomUpdateOfCursor", "cursorId_ invalid");
        }
    }

    void updateAIInputs(){
        randomUpdateOfCursor(1);
        randomUpdateOfCursor(2);
    }

    void moveCursor(const int cursorId_, const int i_, const int j_){
        switch (cursorId_){
            case 1:
                if (inputs.cursor1.getI() + i_ >= -1 && inputs.cursor1.getI() + i_ <= renderedMapSideSize &&
                        inputs.cursor1.getJ() + j_ >= -1 && inputs.cursor1.getJ() + j_ <= renderedMapSideSize){
                    inputs.cursor1 = inputs.cursor1 + vec2i{i_,j_};
                }
                break;
            case 2:
                if (inputs.cursor2.getI() + i_ >= -1 && inputs.cursor2.getI() + i_ <= renderedMapSideSize &&
                    inputs.cursor2.getJ() + j_ >= -1 && inputs.cursor2.getJ() + j_ <= renderedMapSideSize){
                    inputs.cursor2 = inputs.cursor2 + vec2i{i_,j_};
                }
                break;

            default:


        }
    }

    void updatePlayerInputs(){
        int key = _getch();

        if (key == 0 || key == 224) {
            key = _getch();

            switch (key) {
                case 72: std::cout << "Flèche Haut\n"; break;
                case 80: std::cout << "Flèche Bas\n"; break;
                case 75: std::cout << "Flèche Gauche\n"; break;
                case 77: std::cout << "Flèche Droite\n"; break;
                default: std::cout << "Autre touche spéciale: Code " << key << "\n"; break;
            }
        }
        else {
            // Sinon, c'est une touche normale
            switch (key) {
                case 32: std::cout << "Espace\n"; break;   // Code ASCII pour Espace
                case 27: std::cout << "Sortie.\n"; return 0;  // Code ASCII pour Échap
                default: std::cout << "Touche normale: '" << static_cast<char>(key) << "'\n"; break;
            }
        }
    }

public:

    explicit InputsManager(Mode mode_, Inputs & inputs_, const int renderedMapSideSize_, Deck & deck1_, Deck & deck2_) :
        mode(mode_), inputs(inputs_), renderedMapSideSize(renderedMapSideSize_), deck1(deck1_), deck2(deck2_), switcher(0), random(){}

    void updateInputs(){

        switch (mode) {
            case PvP:
                updatePlayerInputs();
                break;
            case PvAI:
                if(switcher){
                    switcher = (switcher + 1) % 2;
                    updatePlayerInputs();
                    break;
                }
                else{
                    switcher = (switcher + 1) % 2;
                    updateAIInputs();
                    break;
                }
            default:
                throw HiveException("inputsManager.h:InputsManager:getInputs", "invalid mode");
                break;
        }
    }

};


#endif //HIVE_INPUTMANAGER_H
