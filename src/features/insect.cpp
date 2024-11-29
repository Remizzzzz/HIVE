//
// Created by Thiba on 29/10/2024.
//
#include <set>
#ifndef HIVE_INSECT_CPP
#define HIVE_INSECT_CPP
#define MAP_SIZE 32

#include "insect.h"
#include "map.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <set>

int Insect::counter = 0;
//Fonctions de Insect

// Renvoie tout les slots dispos sur la map pour poser une pièce
std::vector<vec2i> Insect::setRule( Map &m) const {
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
                    if( m.getInsectAt(*voisin) != nullptr && m.getInsectAt(*voisin)->getColor() != this->getColor()) {
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
bool Insect:: isLinkingHive(Map &m) const {

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
//Fonction former Neighbour pour détecter les anciens voisin à la nouvelle position : POUR BOUGER, IL FAUT STRICTEMENT 1 ANCIEN VOISIN!!
int Insect::getFormerNeighbour(vec2i newPosition, Map &m) {
    int count=0;
    std::list<vec2i> formerNeighbour=m.getNeighbours(getCoordinates());
    std::list<vec2i> newNeighbours=m.getNeighbours(newPosition);
    for (auto it=formerNeighbour.begin();it!=formerNeighbour.end();++it) {
        for (auto itn=newNeighbours.begin();itn!=newNeighbours.end();++itn) {
            if (*it == *itn) {
                count++;
            }
        }
    }
    return count;
}

//Fonctions de Bee

//Fonction qui dit si la reine est encerclée
bool Bee::isCircled(Map &m) {
    std::list<vec2i> neighbors = m.getNeighbours(getCoordinates());
    for (auto it = neighbors.begin(); it != neighbors.end(); ++it) {
        // On itère dans les voisins
        if(m.isSlotFree(*it) == 1)//Quand on trouve une case on renvoie false
        { return false;}
    }
    return true;
}

//Renvoi un vector avec les mouvements possibles de la reine
std::vector<vec2i> Bee:: getPossibleMovements(Map &m){
    std::vector<vec2i> possibleMovements;
    if(!this->isLinkingHive(m)) { //Vérifie que l'insect puisse bouger
        std::list<vec2i> neighbors = m.getNeighbours(getCoordinates());//Récupère les voisins de la case
        std::list<vec2i> neighborsPossibleMovements;
        for (auto it = neighbors.begin(); it != neighbors.end(); ++it) { //Pour chaque voisin, si le slot est free et attaché à au moins un membre dont un (et un seul) est un ancien voisin on l'ajoute à possibleMovement
            if(m.isSlotFree(*it) == 1 ) {
                neighborsPossibleMovements = m.getNeighbours(*it);//On récupère les voisins du voisin
                for (auto itPossibleMovement = neighborsPossibleMovements.begin(); itPossibleMovement != neighborsPossibleMovements.end(); ++itPossibleMovement) {
                    if(m.isSlotFree(*itPossibleMovement) == 0 && *itPossibleMovement != getCoordinates() && getFormerNeighbour(*it, m)==1) { //Si le slot est occupé par une autre pièce que self, et qu'il y a exactement un ancien voisin dans la liste alors on peut bouger
                        possibleMovements.push_back(*it);//Dès qu'on trouve une case
                        }
                    }
                }
            }
        }
    return possibleMovements;
}



// Fonctions de Beetle


std::vector<vec2i> Beetle:: getPossibleMovements(Map &m){
    std::vector<vec2i> possibleMovements;
    int breakCount = 0;
    if(!this->isLinkingHive(m)) { // Si l'insecte ne lie pas la ruche
        std::list<vec2i> neighbours = m.getNeighbours(getCoordinates()); // On récupère la liste des cases voisines
        for (auto it = neighbours.begin(); it != neighbours.end(); ++it) { //On itère dans chaque case voisine
            std::list<vec2i> neighborsPossibleMovements=m.getNeighbours(*it); // On récupère la liste des cases voisines des cases voisines
            for (auto itPossibleMovement=neighborsPossibleMovements.begin();itPossibleMovement != neighborsPossibleMovements.end(); ++itPossibleMovement) { //On itère dans cette liste
                if(*itPossibleMovement!=getCoordinates() && isAboveOf!=nullptr) { //Si le voisin a un autre voisin que l'insecte ET que le scarabée est au dessus d'un insecte
                    if (getFormerNeighbour(*it,m)>1) {
                        possibleMovements.push_back(*it);
                    }
                } else if (*itPossibleMovement!=getCoordinates() && isAboveOf==nullptr) {//Si le voisin a un autre voisin que l'insecte ET que le scarabée n'est pas au dessus d'un insecte
                    if (getFormerNeighbour(*it,m)==1) {//Si la nouvelle position a exactement un ancien voisin
                        possibleMovements.push_back(*it);
                    }
                }
            }
        }
    }
    return possibleMovements;
}



// Fonctions de Grasshoper

std::vector<vec2i> Grasshopper:: getPossibleMovements(Map &m) {
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
                    it2 =  std::vector<vec2i>(m.getNeighbours(it2).begin(),m.getNeighbours(it2).end()).at(i);
                    if(m.isSlotFree(*it)) {
                        possibleMovements.push_back(it2); // On ajoute la position de la case vide dans possibleMovement
                        goOn = false; // On arrête la boucle
                    }
                }
            }
        }
    }
    return possibleMovements; // Retourne le vector des coups possibles
}


//Fonction de Spider
/*std::vector<vec2i> Spider:: getPossibleMovements(Map m) {
    J'ai une idée pour ça : Ca serait possible d'appeler 3 fois getPossibleMovements de Bee ?, pq techniquement c la même chose,
    On veut tous les endroits que l'abeille peut atteindre en 3 coups
}*/

// Fonction de Ant
std::vector<vec2i> Ant:: getPossibleMovements(Map &m) {//L'idée ? faire une boucle auto gérée : On prend une case (la première étant celle de départ) et on prend toutes les cases vides adjacentes dans une liste. On parcourt cette liste, si le potential movement est un possible movement, on ajoute ses cases vides adjacentes à potential movement, puis on l'enleve de potential movement et on l'ajoute à possible movement. Une fois qu'on ne peut plus ajouter, c'est fini !
    std::vector<vec2i> possibleMovements;
    std::vector<vec2i> potentialMovements;
    std::vector<vec2i> impossibleMovements;

    // Initialiser les voisins vides immédiats comme mouvements potentiels
    std::list<vec2i> neighbors = m.getNeighbours(getCoordinates());
    for (const auto &neighbor : neighbors) {
        if (m.isSlotFree(neighbor)) {
            potentialMovements.push_back(neighbor);
        }
    }
    bool valid = false; // Indique si le mouvement est valide
    // Parcourir les mouvements potentiels
    auto it = potentialMovements.begin();// Je fais comme ça parce que chatGPT m'a dit que ct pas bien de faire while (!potentialMovements.empty())
    while (it != potentialMovements.end()) {
        std::list<vec2i> newNeighbours = m.getNeighbours(*it); //On récupère les voisins de la case vide
        for (auto &neighbour : newNeighbours) {
            if (!m.isSlotFree(neighbour) && neighbour != getCoordinates()) {
                // Si la case a un voisin non vide et ce voisin n'est pas Ant
                valid = true;
                // Ajouter les voisins vides de la case non encore traités à potentialMovements
                for (auto &newNeighbour : newNeighbours) {
                    if (m.isSlotFree(newNeighbour)) {
                        if (std::find(potentialMovements.begin(), potentialMovements.end(), newNeighbour) == potentialMovements.end() &&
                            std::find(possibleMovements.begin(), possibleMovements.end(), newNeighbour) == possibleMovements.end() &&
                            std::find(impossibleMovements.begin(), impossibleMovements.end(), newNeighbour) == impossibleMovements.end()) {
                            potentialMovements.push_back(newNeighbour);
                        }
                    }
                }
                break; // Arrêter la vérification dès qu'un voisin valide est trouvé
            }
        }

        if (valid) {//Probleme valid always false ? vérifier getNeighbour et getCoordinate
            // Mouvement validé, le supprimer de potentialMovements
            possibleMovements.push_back(*it); // Ajouter à possibleMovements
            it = potentialMovements.erase(it);
            valid=false;
        } else {
            // Mouvement invalide, l'ajouter à impossibleMovements
            impossibleMovements.push_back(*it);
            it = potentialMovements.erase(it);
        }
    }

    return possibleMovements;
}


//Fonctions de Moustique

std::vector<vec2i> Mosquitoe:: getPossibleMovements(Map &m) {
    std::list<vec2i> neighbours = m.getNeighbours(getCoordinates()); // On récupère la liste des cases voisines
    std::set<vec2i> possibleMovements;
    for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
        //On itère dans chaque case voisine
        if(!m.isSlotFree(*it)){ // Si la case est occupé on détermine le type d'insecte et on applique sa
            // method getPossibleMovement où on ajoute les cases dans l'ensemble
        switch(m.getInsectAt(*it)->getIT()) {
            case bee:
                for (const auto& movement : Bee::getPossibleMovements(m)) {
                    possibleMovements.insert(movement);
                }
                break;
            case ant:
                for (const auto& movement : Ant::getPossibleMovements(m)) {
                    possibleMovements.insert(movement);
                }
                break;
            case grasshopper:
                for (const auto& movement : Grasshopper::getPossibleMovements(m)) {
                    possibleMovements.insert(movement);
                }

                break;
            case spider:
                for (const auto& movement : Spider::getPossibleMovements(m)) {
                    possibleMovements.insert(movement);
                }

                break;
            case beetle:
                for (const auto& movement : Beetle::getPossibleMovements(m)) {
                    possibleMovements.insert(movement);
                }
                break;
        }}
    }
    // Converti l'ensemble en vector
    std::vector<vec2i> possibleMovementsVector(possibleMovements.begin(), possibleMovements.end());
    return possibleMovementsVector;
}










#endif //HIVE_INSECT_CPP
