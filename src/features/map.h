//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_MAP_H
#define HIVE_MAP_H

//#include <QDebug>
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

        // Getter pour 'from'
        const vec2i& getFrom() const {
            return from;
        }

        // Getter pour 'to'
        const vec2i& getTo() const {
            return to;
        }


        movement(const vec2i & from_, const vec2i & to_) : from(from_), to(to_) {};
        movement() = default;
    };
private:


    //friend class renderer;

    std::vector<Insect *> slot{};
    std::list<movement> historic{};
    vec2i relativePos{};
    const int sideSize;
    int rewind;


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

    Map(const int & sideSize_, const int &rewindNumber_) :
        sideSize(sideSize_), rewind(rewindNumber_){
        for(int i = 0; i < sideSize * sideSize; i++){
            slot.push_back(nullptr);
        }
    }
    Map (const int sSize, const int rewindNumber_, const vec2i rPos, const std::list<movement> h):
        sideSize(sSize),rewind(rewindNumber_), relativePos(rPos),historic(h) {
        for(int i = 0; i < sideSize * sideSize; i++){
            slot.push_back(nullptr);
        }
    }
    /**
     * @brief \n put in the slot on position pos_ the insect insect_
     * @param insect_ : insect to put
     * @param pos_ : position of the slot in which the insect will be put
     */
    void putInsectTo(Insect * insect_, const vec2i & pos_){
        slot[posToIndex(pos_)] = insect_;
    }
    [[nodiscard]] Insect * getInsectAt(const vec2i & pos_) const{
        return slot[posToIndex(pos_)];
    }

    Map & operator=(const Map &) {
        return *this;
    }
    [[nodiscard]] const int & getSideSize() const{
        return sideSize;
    }
    [[nodiscard]] const int & getRewind() const{
        return rewind;
    }
    [[nodiscard]] const vec2i getRelativePos() const{
        return relativePos;
    }

    [[nodiscard]] std::vector<Insect *> getSlots() const{
        return slot;
    }

    std::vector<vec2i> setRule( bool color_insect);

    std::list<movement>& getHistoric() {
        return historic;
    }

    /**
     * @brief \n set the slot on position pos_ to nullptr
     * @param pos_ : position of the slot to remove
     */
    void removeInsectAt(const vec2i & pos_){
        slot[posToIndex(pos_)] = nullptr;
    }

    /**
     * @brief \n check if the slop in position pos_ is free
     * @param pos_ : position of the slot to check
     */
    bool isSlotFree(const vec2i & pos_) const{
        return slot[posToIndex(pos_)] == nullptr;
    }

    /**@brief move the insect on pos1_ to the pos2_.*/
    void moveInsect(const vec2i & pos1_, const vec2i & pos2_){

        // Récupérer les insectes aux positions de départ et d'arrivée
        Insect* movingInsect = getInsectAt(pos1_);
        Insect* targetInsect = getInsectAt(pos2_);

        // Vérification qu'il y a un insecte à déplacer
        if (movingInsect == nullptr) {
            throw HiveException("Map::moveInsect", "Aucun insecte à déplacer à la position donnée.");
        }

        // Si l'insecte est un scarabée
        if (movingInsect->getIT() == beetle) {
            // Cast dynamique pour obtenir un pointeur de type Beetle
            Beetle* beetlePointer = dynamic_cast<Beetle*>(movingInsect);
            if (beetlePointer == nullptr) {
                throw HiveException("Map::moveInsect", "Erreur du dynamic_cast pour le scarabée.");
            }

            // Si le scarabée a un insecte en dessous
            if (beetlePointer->getInsectUnder() != nullptr) {
                // Remettre l'insecte en dessous à la position actuelle
                putInsectTo(beetlePointer->getInsectUnder(), pos1_);
            } else {
                // Pas d'insecte en dessous, retirer l'insecte de la position initiale
                removeInsectAt(pos1_);
            }



            // Si un insecte est présent à la position cible, positionner le scarabée au-dessus
            if (targetInsect != nullptr) {
                beetlePointer->setAboveOf(targetInsect);
            }else {
                beetlePointer->setAboveOf(nullptr);
            }

            // Déplacer le scarabée à la position cible
            putInsectTo(movingInsect, pos2_);

        } else {
            // Cas général pour les autres types d'insectes
            putInsectTo(movingInsect, pos2_);
            movingInsect->setCoordinates(pos2_);
            removeInsectAt(pos1_);
        }
    }

    //return positions of the filled slots around pos_
    std::list<vec2i> getNeighbours(const vec2i & pos_) const{
        std::list<vec2i> neighbours{};
        if(pos_.getI() % 2 == 0) {
           neighbours.push_back(pos_ - vec2i(1,1));
           neighbours.push_back( pos_ - vec2i(1,0));
           neighbours.push_back( pos_ - vec2i(0,1));
           neighbours.push_back( pos_ - vec2i(0,-1));
           neighbours.push_back( pos_ - vec2i(-1,1));
           neighbours.push_back( pos_ - vec2i(-1,0));
        }
        else{
           neighbours.push_back( pos_ - vec2i(1,0));
           neighbours.push_back( pos_ - vec2i(1,-1));
           neighbours.push_back( pos_ - vec2i(0,1));
           neighbours.push_back( pos_ - vec2i(0,-1));
           neighbours.push_back( pos_ - vec2i(-1,0));
           neighbours.push_back( pos_ - vec2i(-1,-1));
        }

        return neighbours;
    }
    //Function that adds a move to historic
    void addToHistoric(const vec2i & from, const vec2i & to) {
        historic.emplace_front(from,to);
        while (historic.size() > rewind){ //While just in case, but it works normally
            historic.pop_back();
        }
    }
    void goBack() {
        if (!historic.empty()) {
            vec2i oldC=historic.front().from;
            vec2i curC=historic.front().to;
            moveInsect(curC,oldC); //Rewind move
            historic.pop_front();          // Erase the head of historic again (here, the move we just rewinded)
        }
    }

    [[nodiscard]] bool isSlotUsable(const vec2i & pos_) const
    {
        if(pos_.getI() > 0 && pos_.getI() < getSideSize() && pos_.getJ() > 0 && pos_.getJ() < getSideSize())
        {
            size_t compteur =0;
            std::list<vec2i> neighbours = getNeighbours(pos_);
            for(auto & neighbour : neighbours)
            {
                if(!isSlotFree(neighbour)) compteur++;
            }
            return compteur < 5;
        }
        return true;

    }
//------- ANTI MAP EXIT -------
private:

    /**
     * @brief check if an instect is close to the left border
     */
    bool isLeftBorderTouched(){
        bool b = false;

        for(int i = 0; i < sideSize * sideSize ; i += sideSize){
            b = b || (slot[i] != nullptr);
        }
        return b;
    }

    /**
     * @brief check if an insect is close to the right border
     */
    bool isRightBorderTouched(){
        bool b = false;

        for(int i = sideSize - 1; i < sideSize * sideSize ; i += sideSize){
            b = b || (slot[i] != nullptr);
        }
        return b;
    }

    /**
     * @brief check if an instect is close to the top border
     */
    bool isTopBorderTouched(){
        bool b = false;

        for(int i = 0; i < sideSize; i++){
            b = b || (slot[i] == nullptr);
        }
        return b;
    }

    /**
     * @brief check if an instect is close to the bottom border
     */
    bool isBottomBorderTouched(){
        bool b = false;

        for(int i = sideSize * (sideSize - 1); i < sideSize; i++){
            b = b || (slot[i] == nullptr);
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
                            moveInsect(curr,curr + tVec);
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
                            moveInsect(curr,curr + tVec);
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
                            moveInsect(curr,curr + tVec);
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
                            moveInsect(curr,curr + tVec);
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
};

#endif //HIVE_MAP_H
