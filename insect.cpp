//
// Created by Thiba on 29/10/2024.
//
#ifndef HIVE_INSECT_CPP
#define HIVE_INSECT_CPP
#define MAP_SIZE 32

#include "insect.h"
#include "map.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>

//Fonctions de Insect

// Renvoie tout les slots dispos sur la map pour poser une pièce
std::vector<vec2i> Insect::setRule(Map m) const {
    std::vector<vec2i> possiblePlace;
     // On cherche les endroits possibles
        // Trouver un insecte sur la map en iterant dessus en sortant de la boucle des qu'il est trouvé
        int found_i = -1, found_j;
        for(int i = 0; i < MAP_SIZE; i++) {
            for(int j = 0; j < MAP_SIZE; j++) {
                if(m.isSlotFree(vec2i{i,j}) == 0) {
                    found_i = i;
                    found_j = j;
                    break;
                }
            }
            if(found_i != -1) {
                break;
            }
        }
        if(found_i == -1) {
            possiblePlace.push_back({MAP_SIZE/2,MAP_SIZE/2});
        }
        std::vector<vec2i> insectSet = {vec2i(found_i,found_j)} , toCheck;
        std::list<vec2i> neighbors;
        int i_insect = 0;
        while (insectSet.size() < i_insect) // On itère dans insectSet tant qu'il y a des elements ;
        {
            neighbors = m.getNeighbours(insectSet[i_insect]);
            for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {// On itère dans les voisins
                if(m.isSlotFree(*it) == 0)//Quand on trouve un voisin de l'elem on l'ajoute dans la liste insectset si pas encore dedans
                    {
                    if (std::find(insectSet.begin(), insectSet.end(), *it)[0] != *it) {
                        // L'élément n'est pas trouvé, donc on l'ajoute
                        insectSet.push_back(*it);
                        i_insect++;
                    }
                }else//Quand on trouve une case vide on l'ajoute dans la liste toCheck si pas encore dedans
                {
                    if (std::find(toCheck.begin(), toCheck.end(), (*it))[0] != *it) {
                        // L'élément n'est pas trouvé, donc on l'ajoute
                        toCheck.push_back(*it);
                    }
                }
            }
        }
        if(insectSet.size() == 1) {// Si un seul insect on propose les six voisins de l'unique element
            neighbors = m.getNeighbours(insectSet[0]);
            std::vector<vec2i> neighborsVector(neighbors.begin(), neighbors.end()); //Conversion en vector
            return neighborsVector;
        }else {
            bool isOk;
            for (auto it = toCheck.begin(); it != toCheck.end(); ++it) {
                neighbors = m.getNeighbours(*it);
                isOk = true;
                for (auto voisin = neighbors.begin(); voisin != neighbors.end(); ++voisin) {
                    if( m.getInsect(voisin) != nullptr && m.getInsect(voisin).getColor() != this->getColor()) {
                        isOk = false; // Indique qu'on ne veut pas l'ajouter dans possible place
                        break; // Pas besoin de checker les autres voisins
                    }
                }
                if (isOk) {// Si pas de voisin de la couleur inverse et pas déjà dans possiblePlace on l'ajoute
                    if (std::find(possiblePlace.begin(), possiblePlace.end(), (*it))[0] != *it) {
                        possiblePlace.push_back(*it);
                    }

                }
            }
        }
    return possiblePlace;
}

//Fonction qui détermine si en enlevant un insecte la ruche est toujours lié (si l'insecte à le droit de bouger)
bool Insect:: isLinkingHive(Map m) {

    // On parcourt toute la map pour compter son nombre d'insecte et garder une valeur d'insect
    int found_i = -1, found_j, nbInsect = 0;
    for(int i = 0; i < MAP_SIZE; i++) {
        for(int j = 0; j < MAP_SIZE; j++) {
            if(m.isSlotFree(vec2i{i,j}) == 0 && getCoordinates() != vec2i{i,j}) {
                found_i = i;
                found_j = j;
                nbInsect++;
            }
        }
    }
    if(found_i == -1) { //Si aucun insect trouvé on renvoie true
        return true;
    }
    std::vector<vec2i> insectSet = {vec2i(found_i,found_j)};
    int i_insect = 0;
    std::list<vec2i> neighbors;
    while (insectSet.size() < i_insect) // On itère dans insectSet tant qu'il y a des elements ;
    {
        neighbors = m.getNeighbours(insectSet[i_insect]);
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {// On itère dans les voisins
            if(m.isSlotFree(*it) == 0)//Quand on trouve un voisin insect de l'elem on l'ajoute dans la liste insectset si pas encore dedans sauf si c'est l'insect sur lequel on a appelé la fonction
            {
                if (std::find(insectSet.begin(), insectSet.end(), *it)[0] != *it && *it != getCoordinates()) {
                    // L'élément n'est pas trouvé, donc on l'ajoute
                    insectSet.push_back(*it);
                    i_insect++;
                }
            }
        }
    }
    return i_insect != nbInsect;
}


//Fonctions de Bee

//Fonction qui dit si la reine est encerclée
bool Bee::isCircled(Map m) {
    std::list<vec2i> neighbors = m.getNeighbours(getCoordinates());
    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
        // On itère dans les voisins
        if(m.isSlotFree(*it) == 1)//Quand on trouve une case on renvoie false
        { return false;}
    }
    return true;
}

//Renvoi un vector avec les mouvements possibles de la reine
std::vector<vec2i> Bee:: getPossibleMovements(Map m) {
    std::vector<vec2i> possibleMovements;
    if(!this->isLinkingHive(m)) { //Vérifie que l'insect puisse bouger
        std::list<vec2i> neighbors = m.getNeighbours(getCoordinates());//Récupère les voisins de la case
        std::list<vec2i> neighborsPossibleMovements;
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) { //Pour chaque voisin, si le slot est free et attaché à au moins un membre on l'ajoute à possibleMovement
            if(m.isSlotFree(*it) == 1 ) {
                neighborsPossibleMovements = m.getNeighbours(*it);
                for (auto itPossibleMovement = neighborsPossibleMovements.begin(); itPossibleMovement != neighborsPossibleMovements.end(); ++itPossibleMovement) {
                    if(m.isSlotFree(*itPossibleMovement) == 0 && *itPossibleMovement != getCoordinates()) {
                        for (auto itSameNeighbour=neighbors.begin();itSameNeighbour!=neighbors.end();++itSameNeighbour) {
                            if (itPossibleMovement == itSameNeighbour) {
                                possibleMovements.push_back(*it);//Dès qu'on touve une case
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
    return possibleMovements;
}



// Fonctions de Beetle


std::vector<vec2i> Beetle:: getPossibleMovements(Map m) {
    std::vector<vec2i> possibleMovements;
    int breakCount = 0;
    if(!this->isLinkingHive(m)) {
        std::list<vec2i> neighbours = m.getNeighbours(getCoordinates());
        for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
            std::list<vec2i> neighborsPossibleMovements=m.getNeighbours(*it);
            for (auto itPossibleMovement=neighborsPossibleMovements.begin();itPossibleMovement != neighborsPossibleMovements.end(); ++itPossibleMovement) {
                if(*itPossibleMovement!=getCoordinates()) {
                    for (auto itSameNeighbour=neighbours.begin();itSameNeighbour!=neighbours.end();++itSameNeighbour) {
                        if (itPossibleMovement == itSameNeighbour) {
                            possibleMovements.push_back(*it);
                            breakCount++;
                        }
                    }
                }
                if (breakCount>0) {
                    breakCount=0;
                    break;
                }
            }
        }
    }
    return possibleMovements;
}




// Fonctions de Grasshoper

std::vector<vec2i> Grasshopper:: getPossiblesMovement(Map m) {
    std::vector<vec2i> possibleMovements;
    int breakCount = 0;
    if(!this->isLinkingHive(m)) {
        std::list<vec2i> neighboursList = m.getNeighbours(getCoordinates());
        std::vector<vec2i> neighbors(neighboursList.begin(), neighboursList.end()); //Conversion en vector
        int i = 0;
        vec2i it2;
        bool goOn = true;
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
            //On itère dans chaque voisin
            if( !m.isSlotFree(*it)) {
                // S'il y a un voisin
                it2 = neighbors.at(i++);
                while(goOn) {
                    // On itère dans le voisin de position x du voisin x du voisin x etc jusqu'à trouver une case vide
                    it2 =  std::pmr::vector<vec2i>(m.getNeighbours(*it2).begin(),m.getNeighbours(*it2).end()).at(i);
                    if(m.isSlotFree(*it)) {
                        possibleMovements.push_back(*it2); // On ajoute la position de la case vide dans possibleMovement
                        goOn = false; // On arrète la boucle
                    }
                }
            }
        }
    }
    return possibleMovements; // Retourne le vector des coups possibles
}













#endif //HIVE_INSECT_CPP
