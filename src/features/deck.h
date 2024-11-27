//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_DECK_H
#define HIVE_DECK_H

#include <vector>

#include "insect.h"

class Deck{

private:
    std::vector<Insect *> insects;
    int insectNb{0};

public:
    Deck() = default;


    // Classe interne pour l'itérateur
    class Iterator {
    public:
        // Constructeur
        Iterator(std::vector<Insect *>::iterator it) : current(it) {}

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


    Insect * getInsectAt(const int & index_){
        return insects.at(index_);
    }

    bool isSlotFree(const int & index_){
        return insects.at(index_) == nullptr;
    }

    bool isIndexValid(const int & index_){
        return index_ >= 0 && index_ < insectNb ;
    }

    void deleteAt(const int & index_){
        insectNb--;
        insects[index_] = insects[insectNb];
        insects[insectNb] = nullptr;
    }

};

#endif //HIVE_DECK_H
