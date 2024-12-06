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

    bool isHuman;
    std::string name;
    Deck deck;
    std::vector<Insect *> activeInsects;

    Inputs inputs;

    friend class Hive;
    friend class InputsManager;
    friend class Solver;
public:

    explicit Player(const int & id_) : id(id_), isHuman(true), deck(), activeInsects(), inputs()
    {

    };

    const int & getId() const{
        return id;
    }

    const Inputs & getInputs() const {
        return inputs;
    }

    const Deck & getDeck() const{
        return deck;
    }

    const std::vector<Insect *> & getActiveInsects() const{
        return activeInsects;
    }

    void addActiveInsectsFromDeck(const int & deckIndex_){
        Insect * insect = deck.insects.at(deckIndex_);
        activeInsects.push_back(insect);
    }

    void setHumanity(const bool & isHuman_) {
        isHuman = isHuman_;
    }

};

#endif //HIVE_PLAYER_H
