//
// Created by loic_ on 30/11/2024.
//

#ifndef HIVE_PLAYER_H
#define HIVE_PLAYER_H

#include "deck.h"
#include "inputs.h"

class Hive;
class InputsManager;
class Solver;

class Player{
private:
    int id;

    Deck deck;
    std::vector<Insect *> activeInsects;

    Inputs inputs;

    friend class Hive;
    friend class InputsManager;
    friend class Solver;
public:

    explicit Player(const int & id_) : id(id_), deck(), activeInsects(), inputs(){};

    const int & getId() const{
        return id;
    }

    const Inputs & getInputs() {
        return inputs;
    }

    const Deck & getDeck(){
        return deck;
    }

    const std::vector<Insect *> & getActiveInsects(){
        return activeInsects;
    }

    void addActiveInsectsFromDeck(const int & deckIndex_){
        Insect * insect = deck.insects.at(deckIndex_);
        activeInsects.push_back(insect);
    }

};

#endif //HIVE_PLAYER_H
