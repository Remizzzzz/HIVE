//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_MAP_H
#define HIVE_MAP_H

#include <iostream>
#include <vector>
#include <list>

#include "insect.h"

class Map{
private:

    class movement{
    public:
        vec2i from{};
        vec2i to{};

        movement(const vec2i & from_, const vec2i & to_) : from(from_), to(to_) {};
        movement() = default;
    };

    //friend class renderer;

    std::vector<const Insect *> slots{};
    std::list<movement> historic{};
    vec2i relativePos{};
    int sideSize{32};
    int rewind=5;


    /**
     * @brief \n convert the vec2i in an index
     * @param pos_ position to convert
     * @return index
     */
    [[nodiscard]] int posToIndex(const vec2i & pos_) const{

        if(pos_.getI() >= sideSize || pos_.getJ() >= sideSize) std::cerr << "map::Map::posToIndex outOfRange";

        return (pos_.getI() * sideSize + pos_.getJ());
    }

public:

    explicit Map(int & sideSize_,int &n) : sideSize(sideSize_),rewind(n){
        for(int i = 0; i < sideSize * sideSize; i++){
            slots.push_back(nullptr);
        }
    }

    /**
     * @brief \n put in the slot on position pos_ the instect instect_
     * @param insect_ : insect to put
     * @param pos_ : position of the slot in which the insect will be put
     */
    void putInsect(const Insect * insect_, const vec2i & pos_){
        slots[posToIndex(pos_)] = insect_;
    }
    const Insect * getInsect(const vec2i & pos_) {
        return slots[posToIndex(pos_)];
    }

    const int & getSideSize() const{
        return sideSize;
    }

    /**
     * @brief \n set the slot on position pos_ to nullptr
     * @param pos_ : position of the slot to remove
     */
    void removeInsect(const vec2i & pos_){
        slots[posToIndex(pos_)] = nullptr;
    }

    /**
     * @brief \n check if the slop in position pos_ is free
     * @param pos_ : position of the slot to check
     */
    bool isSlotFree(const vec2i & pos_){
        return slots[posToIndex(pos_)] == nullptr;
    }

    /**@brief move the insect on pos1_ to the pos2_.*/
    void move(const vec2i & pos1_, const vec2i & pos2_){
        if(!(isSlotFree(pos1_) || pos1_ == pos2_)){
            putInsect(slots[posToIndex(pos1_)], pos2_);
            removeInsect(pos1_);
        }
        addToHistoric(pos1_,pos2_);//If the movement is a rewind, goBack will manage the historic
    }

    //return positions of the filled slots around pos_
    std::list<vec2i> getNeighbours(const vec2i & pos_){
        std::list<vec2i> neighbours{};

        if(pos_.getI() % 2 == 0) {
           neighbours.push_back(pos_ - vec2i(-sideSize,0));
           neighbours.push_back( pos_ - vec2i(-sideSize + 1,0));
           neighbours.push_back( pos_ - vec2i(-1,0));
           neighbours.push_back( pos_ - vec2i(1,0));
           neighbours.push_back( pos_ - vec2i(sideSize,0));
           neighbours.push_back( pos_ - vec2i(sideSize + 1,0));
        }
        else{
           neighbours.push_back( pos_ - vec2i(-sideSize - 1,0));
           neighbours.push_back( pos_ - vec2i(-sideSize,0));
           neighbours.push_back( pos_ - vec2i(-1,0));
           neighbours.push_back( pos_ - vec2i(1,0));
           neighbours.push_back( pos_ - vec2i(sideSize - 1,0));
           neighbours.push_back( pos_ - vec2i(sideSize,0));
        }

        return neighbours;
    }
    //Function that adds a move to historic
    void addToHistoric(const vec2i & from, const vec2i & to) {
        historic.emplace_front(from,to);
        while (historic.size() > rewind){ //While just in case, but if works normally
            historic.pop_back();
        }
    }
    void goBack() {
        if (!historic.empty()) {
            move(historic.front().to,historic.front().from); //Rewind move
            historic.pop_front();          // Erase the head of historic (here, the goBack move)
            historic.pop_front();               // Erase the head of historic again (here, the move we just rewinded)
        }
    }

//------- ANTI MAP EXIT -------
private:

    /**
     * @brief check if an instect is close to the left border
     */
    bool isLeftBorderTouched(){
        bool b = false;

        for(int i = 0; i < sideSize * sideSize ; i += sideSize){
            b = b || (slots[i] != nullptr);
        }
        return b;
    }

    /**
     * @brief check if an insect is close to the right border
     */
    bool isRightBorderTouched(){
        bool b = false;

        for(int i = sideSize - 1; i < sideSize * sideSize ; i += sideSize){
            b = b || (slots[i] != nullptr);
        }
        return b;
    }

    /**
     * @brief check if an instect is close to the top border
     */
    bool isTopBorderTouched(){
        bool b = false;

        for(int i = 0; i < sideSize; i++){
            b = b || (slots[i] == nullptr);
        }
        return b;
    }

    /**
     * @brief check if an instect is close to the bottom border
     */
    bool isBottomBorderTouched(){
        bool b = false;

        for(int i = sideSize * (sideSize - 1); i < sideSize; i++){
            b = b || (slots[i] == nullptr);
        }
        return b;
    }

    /**
     * @brief move all the map's insects according to the param tVec
     * @param tVec : direction of the translation
     */
    void translateMap(const vec2i & tVec){
        //Translation vers le haut donc parcours de haut en bas
        if(tVec.getI() <= 0){
            //Translation vers la gauche donc parcours de gauche à droite
            if(tVec.getJ() <= 0){
                for(int i = 0; i < sideSize; i++){
                    for(int j = 0; j < sideSize; j++){
                        vec2i curr = vec2i(i,j);
                        if(!isSlotFree(curr)){
                            move(curr,curr + tVec);
                        }
                    }
                }
            }
                //Translation vers la droite donc parcours de droite à gauche
            else{
                for(int i = 0; i < sideSize; i++){
                    for(int j = sideSize - 1; j >= 0; j--){
                        vec2i curr = vec2i(i,j);
                        if(!isSlotFree(curr)){
                            move(curr,curr + tVec);
                        }
                    }
                }
            }
        }
            //Translation vers le bas donc parcours de bas en haut
        else{
            //Translation vers la gauche donc parcours de gauche à droite
            if(tVec.getJ() <= 0){
                for(int i = sideSize - 1; i >= 0; i--){
                    for(int j = 0; j < sideSize; j++){
                        vec2i curr = vec2i(i,j);
                        if(!isSlotFree(curr)){
                            move(curr,curr + tVec);
                        }
                    }
                }
            }
                //Translation vers la droite donc parcours de droite à gauche
            else{
                for(int i = sideSize - 1; i >= 0; i--){
                    for(int j = sideSize - 1; j >= 0; j--){
                        vec2i curr = vec2i(i,j);
                        if(!isSlotFree(curr)){
                            move(curr,curr + tVec);
                        }
                    }
                }
            }
        }

    }

    void translateHistory(const vec2i & tVec){
        for(auto & move : historic){
            move.from += tVec;
            move.to += tVec;
        }
    }
public:

    /**
     * @brief \n keep insects in the map and update relativePos
     */
    void keepInsectsInMap(){
        vec2i tVec{};

        if(isTopBorderTouched()){
            tVec.setI(1);
        }
        if(isBottomBorderTouched()){
            tVec.setI(-1);
        }
        if(isLeftBorderTouched()){
            tVec.setJ(1);
        }
        if(isRightBorderTouched()){
            tVec.setJ(-1);
        }

        if(!tVec.isNull()){
            translateMap(tVec);
            translateHistory(tVec);
            relativePos += tVec;
        }
    }


private:
    /**
     * @brief \n write map on default std::cout
     */
    void getMap() const {
        // to show the useful part of the map, i.e. a centered square of (sideSize - numbers of rows used to prevent
        // pawns to go beyond limits)
        size_t nRow = 1;
        size_t nCol = 1;
        while(nRow<32)
        {
            if(nRow==1 and nCol==1)
            {
                std::cout << "  ";
                for(nCol=1; nCol<31;nCol++){
                    std::cout << " /\\ ";
                }
                std::cout << std::endl << "  ";
            }
            else if(nCol==31) // si l'on finit une ligne
            {
                std::cout << "\n ";
                if(nRow% 2 == 0) {
                    std::cout << "\\/";
                }
                std::cout << "/\\\\";
                for(nCol=1; nCol<31;nCol++)
                {
                    std::cout << "//\\\\";
                }
                std::cout << "//\\";
                if(nRow % 2 == 1)
                {
                    std::cout << "\\/";
                }
                std::cout << std::endl;
                if(nRow % 2 == 0)
                {
                    std::cout << "  ";
                }
            }
            for(nCol=1; nCol<31;nCol++)
            {
                if(size_t index = nRow*32 + nCol && slots[index] != nullptr) {
                    std::cout << "|" << slots[index]->getPrintableValue() << "|";
                } else {
                    std::cout << "|  |";
                }
                if(nRow==31)
                {
                    std::cout << "\n";
                    for(nCol=1; nCol<31;nCol++){
                        std::cout << " \\/ ";
                    }
                    std::cout << std::endl << "  ";
                }
                nRow++;
            }
        }
    }


};

#endif //HIVE_MAP_H
