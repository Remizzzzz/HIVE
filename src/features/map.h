//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_MAP_H
#define HIVE_MAP_H

#include <QDebug>
#include <iostream>
#include <vector>
#include <list>
#include "insect.h"
#include "../utils/utils.h"
//#include <QDebug>








class Map{
public:
    class movement{
    public:
        vec2i from{};
        vec2i to{};

        const vec2i& getFrom() const { return from; }
        const vec2i& getTo() const { return to; }

        movement(const vec2i & from_, const vec2i & to_) : from(from_), to(to_) {};
        movement() = default;
    };
private:
    std::vector<Insect *> slot{};
    std::list<movement> historic{};
    vec2i relativePos{};
    const int sideSize;
    int rewind;

    // converti un vec2i en index
    [[nodiscard]] int posToIndex(const vec2i & pos_) const{
        if(pos_.getI() >= sideSize || pos_.getJ() >= sideSize) std::cerr << "map::Map::posToIndex outOfRange";
        return (pos_.getI() * sideSize + pos_.getJ());
    }

public:
    Map(const int & sideSize_, const int &rewindNumber_) : sideSize(sideSize_), rewind(rewindNumber_) {
        for(int i = 0; i < sideSize * sideSize; i++){
            slot.push_back(nullptr);
        }
    }
    Map (const int sSize, const int rewindNumber_, const vec2i rPos, const std::list<movement> h):
        historic(h),relativePos(rPos), sideSize(sSize),rewind(rewindNumber_) {
        for(int i = 0; i < sideSize * sideSize; i++) slot.push_back(nullptr);
    }

    Insect* getSlot(int nb) const { return slot[nb]; }
    void setSlot(int nb, Insect* insect) { slot[nb]  = insect; }

    void putInsectTo(Insect * insect_, const vec2i & pos_) { slot[posToIndex(pos_)] = insect_; }
    [[nodiscard]] Insect * getInsectAt(const vec2i & pos_) const { return slot[posToIndex(pos_)]; }

    Map & operator=(const Map &) { return *this; }
    [[nodiscard]] const int & getSideSize() const { return sideSize; }
    [[nodiscard]] const int & getRewind() const { return rewind; }
    [[nodiscard]] const vec2i getRelativePos() const { return relativePos; }
    [[nodiscard]] std::vector<Insect *> getSlots() const { return slot; }
    std::list<movement>& getHistoric() { return historic; }

    std::vector<vec2i> setRule( bool color_insect);

    void removeInsectAt(const vec2i & pos_) { slot[posToIndex(pos_)] = nullptr; }

    // vérifie que la position est libre
    bool isSlotFree(const vec2i & pos_) const { return slot[posToIndex(pos_)] == nullptr; }

    // déplace l'insecte d'une position 1 à une position 2
    void moveInsect(const vec2i & pos1_, const vec2i & pos2_);

    // retourne les voisins d'une position
    std::list<vec2i> getNeighbours(const vec2i & pos_) const;

    // ajoute un déplacement à l'historique
    void addToHistoric(const vec2i & from, const vec2i & to) {
        historic.emplace_front(from,to);
        while (historic.size() > rewind) historic.pop_back();
    }

    void goBack() {
        if (!historic.empty()) {
            vec2i oldC=historic.front().from;
            vec2i curC=historic.front().to;
            moveInsect(curC,oldC);
            historic.pop_front();
        }
    }

    [[nodiscard]] bool isSlotUsable(const vec2i & pos_, const vec2i & posToIgnore) const {
        if(pos_.getI() > 0 && pos_.getI() < getSideSize() && pos_.getJ() > 0 && pos_.getJ() < getSideSize()) {
            size_t compteur =0;
            std::list<vec2i> neighbours = getNeighbours(pos_);
            for(auto & neighbour : neighbours) {
                if(!isSlotFree(neighbour)&&neighbour!=posToIgnore) compteur++;
            }
            return compteur < 5;
        }
        return true;

    }


//------- ANTI MAP EXIT -------
private:
    // vérifie qu'un insect touche le bord gauche de la map
    bool isLeftBorderTouched() {
        bool b = false;
        for (int i = 0; i < sideSize * sideSize ; i += sideSize) b = b || (slot[i] != nullptr);
        return b;
    }

    // vérifie qu'un insect touche le bord droit de la map
    bool isRightBorderTouched(){
        bool b = false;
        for (int i = sideSize - 1; i < sideSize * sideSize ; i += sideSize) b = b || (slot[i] != nullptr);
        return b;
    }

    // vérifie qu'un insect touche le bord haut de la map
    bool isTopBorderTouched(){
        bool b = false;
        for (int i = 0; i < sideSize; i++) b = b || (slot[i] == nullptr);
        return b;
    }

    // vérifie qu'un insect touche le bord bas de la map
    bool isBottomBorderTouched(){
        bool b = false;
        for (int i = sideSize * (sideSize - 1); i < sideSize; i++) b = b || (slot[i] == nullptr);
        return b;
    }

    // déplace tous les insectes dans le sens tVec
    void translateMap(const vec2i & tVec);

    void translateHistory(const vec2i & tVec){
        for (auto & move : historic){
            move.from += tVec;
            move.to += tVec;
        }
    }

public:
    void keepInsectsInMap(){
        vec2i tVec{};
        if (isTopBorderTouched()) tVec.setI(1);
        if(isBottomBorderTouched()) tVec.setI(-1);
        if(isLeftBorderTouched()) tVec.setJ(1);
        if(isRightBorderTouched()) tVec.setJ(-1);

        if(!tVec.isNull()) {
            translateMap(tVec);
            translateHistory(tVec);
            relativePos += tVec;
        }
    }
};

#endif //HIVE_MAP_H
