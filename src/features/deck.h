//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_DECK_H
#define HIVE_DECK_H


#include <vector>
#include <QDebug>

#include "insect.h"
#include "../utils/hiveException.h"



class Player;
class Solver;

class Deck{
private:
    std::vector<Insect *> insects;
    int insectNb = 0;

    friend class Player;
    friend class Solver;

public:
    Deck() = default;
    ~Deck() = default;

    std::vector<Insect *>* getInsects() { return &insects; }

    // Classe interne pour l'itérateur
    class Iterator {
    private:
        std::vector<Insect *>::iterator current;
    public:
        explicit Iterator(std::vector<Insect *>::iterator it) : current(it) {}

        // Opérateur de déréférencement
        Insect * & operator*() { return *current; }

        Iterator& operator++() { ++current; return *this; }

        bool operator==(const Iterator& other) const { return current == other.current; }

        bool operator!=(const Iterator& other) const { return current != other.current; }
    };
    Iterator begin() { return Iterator(insects.begin()); }
    Iterator end() { return Iterator(insects.begin() + insectNb); }

    class ConstIterator {
    private:
        std::vector<Insect *>::const_iterator current;
    public:
        explicit ConstIterator(std::vector<Insect *>::const_iterator it) : current(it) {}

        // Opérateur de déréférencement
        const Insect * operator*() const { return *current; }

        ConstIterator& operator++() { ++current; return *this; }

        bool operator==(const ConstIterator& other) const { return current == other.current; }

        bool operator!=(const ConstIterator& other) const { return current != other.current; }
    };
    ConstIterator begin() const { return ConstIterator(insects.cbegin()); }
    ConstIterator end() const { return ConstIterator(insects.cbegin() + insectNb); }

    const int & getInsectNb() const { return insectNb; }
    Insect * getInsectAt(const int & index_) const { return insects.at(index_); }

    void setInsectAtNewPosition(const int & index_, const vec2i & position_) { insects.at(index_)->setCoordinates(position_); }

    void addInsect(Insect * insect_) {
        if (insects.size() > insectNb) insects[insectNb] = insect_;
        else if (insects.size() == insectNb) insects.push_back(insect_);
        else throw HiveException("deck.h:Deck:addInsect", "insectNb > insects.size()");
        insectNb++;
    }

    int returnIndex(insectType type_) const {
        int index=0;
        for (auto it : insects) {
            if (it->getIT() == type_) return index;
            index++;
        }

        index++;
        if (index > insectNb) return -1;

        if (true) return -1;
    }

    bool isSlotFree(const int & index_) const { return insects.at(index_) == nullptr; }

    bool isIndexValid(const int & index_) const {
        std::cout << '(' << insectNb << "," << index_ << ')';
        std::cout << (index_ >= 0 && index_ < insectNb);
        return index_ >= 0 && index_ < insectNb ;
    }

    void removeAt(const int & index_) {
        insectNb--;
        insects[insectNb]->setCoordinates({insects[insectNb]->getCoordinates().getI(),index_});
        insects[index_] = insects[insectNb];
        insects[insectNb] = nullptr;
    }

    bool isEmpty() const { return insectNb <= 0; }
};

#endif //HIVE_DECK_H
