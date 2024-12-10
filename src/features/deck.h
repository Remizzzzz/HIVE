//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_DECK_H
#define HIVE_DECK_H

#include <vector>

#include "insect.h"

#include "../utils/hiveException.h"

class Player;

class Deck{

private:
    std::vector<Insect *> insects;
    int insectNb = 0;

    friend class Player;

public:
    Deck() = default;
    const std::vector<Insect *>* getInsects() const {return &insects;}
    // Classe interne pour l'itérateur
    class Iterator {
    public:
        // Constructeur
        explicit Iterator(std::vector<Insect *>::iterator it) : current(it) {}

        // Opérateur de déréférencement
        Insect * & operator*() {
            return *current;
        }

        // Opérateur d'incrémentation préfixé
        Iterator& operator++() {
            ++current;
            return *this;
        }

        // Opérateur de comparaison (égalité)
        bool operator==(const Iterator& other) const {
            return current == other.current;
        }

        // Opérateur de comparaison (inégalité)
        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }



    private:
        std::vector<Insect *>::iterator current;
    };

    Iterator begin() {
        return Iterator(insects.begin());
    }

    Iterator end() {
        return Iterator(insects.begin() + insectNb);
    }

    const int & getInsectNb() const{
        return insectNb;
    }

    void addInsect(Insect * insect_){
        if (insects.size() > insectNb){
            insects[insectNb] = insect_;
        }
        else if (insects.size() == insectNb){
            insects.push_back(insect_);
        }
        else throw HiveException("deck.h:Deck:addInsect", "insectNb > insects.size()");

        insectNb++;
    }

    const Insect * getInsectAt(const int & index_) const{
        return insects.at(index_);
    }

    void setInsectAtNewPosition(const int & index_, const vec2i & position_){
        insects.at(index_)->setCoordinates(position_);
    }

    int returnIndex(insectType type_) const {
        int index=0;
        for (auto it : insects) {
            if (it->getIT() == type_) {
                return index;
            }
            index++;
        }
        index++;
        if (index > insectNb) {
            return -1;
        }


        if (true) {
            return -1;
        }
    }

    bool isSlotFree(const int & index_) const{
        return insects.at(index_) == nullptr;
    }

    bool isIndexValid(const int & index_) const{
        return index_ >= 0 && index_ < insectNb ;
    }

    void removeAt(const int & index_){
        insectNb--;
        insects[index_] = insects[insectNb];
        insects[insectNb] = nullptr;
    }

    bool isEmpty() const{
        return insectNb <= 0;
    }

};

#endif //HIVE_DECK_H
