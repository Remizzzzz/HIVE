//
// Created by Thiba on 29/10/2024.
//
#include <map>
#include <set>
#ifndef HIVE_INSECT_CPP
#define HIVE_INSECT_CPP

#include "insect.h"
#include "map.h"
#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <set>
#include "../utils/hiveException.h"

int Insect::counter = 0;

// Méthodes de Insect
// Renvoie tous les slots dispos sur la map pour poser une pièce
std::vector<vec2i> Insect::setRule(Map &m, bool color_insect) const {
    //bool found = color;
    try {
        std::vector<vec2i> possiblePlace;

        // Trouver un insecte sur la carte
        vec2i foundInsect(-1, -1);
        for (int i = 0; i < m.getSideSize() && foundInsect.getI() == -1; ++i) {
            for (int j = 0; j < m.getSideSize(); ++j) {
                if (!m.isSlotFree(vec2i{i, j})) {
                    foundInsect = vec2i{i, j};
                    break;
                }
            }
        }

        // Si aucun insecte n'est trouvé, proposer le centre de la carte
        if (foundInsect.getI() == -1) {
            possiblePlace.push_back(vec2i{m.getSideSize() / 2, m.getSideSize() / 2});
            return possiblePlace;
        }

        // Découvrir tous les insectes connectés
        std::vector<vec2i> insectSet = {foundInsect}; // Liste des insectes connectés
        std::set<vec2i> visited;                      // Ensemble pour éviter les doublons
        std::set<vec2i> toCheck;                      // Cases vides adjacentes à vérifier

        // Parcours dynamique avec `while`
        size_t index = 0; // Indice pour parcourir `insectSet`
        while (index < insectSet.size()) {
            vec2i current = insectSet[index];
            visited.insert(current);

            // Récupérer les voisins de l'insecte courant
            for (const auto& neighbor : m.getNeighbours(current)) {
                if (!m.isSlotFree(neighbor)) {
                    // Si un insecte voisin est trouvé, l'ajouter à `insectSet` s'il n'a pas encore été visité
                    if (visited.find(neighbor) == visited.end() &&
                        std::find(insectSet.begin(), insectSet.end(), neighbor) == insectSet.end()) {
                        insectSet.push_back(neighbor);
                    }
                } else {
                    // Ajouter les cases vides à `toCheck`
                    toCheck.insert(neighbor);
                }
            }

            ++index; // Avancer l'indice pour explorer le prochain insecte
        }

        // Si un seul insecte est présent, proposer ses six voisins
        if (insectSet.size() == 1) {
            for (const auto& neighbor : m.getNeighbours(insectSet[0])) {
                if (m.isSlotFree(neighbor)) {
                    possiblePlace.push_back(neighbor);
                }
            }
            return possiblePlace;
        }

        // Cas avec 2 insectes ou plus
        std::set<vec2i> sameColorSet;  // Insectes de la même couleur
        std::set<vec2i> differentColorSet;  // Insectes de couleur différente

        // Itérer sur tous les insectes dans l'ensemble
        for (const auto& insect : insectSet) {
            for (const auto& neighbor : m.getNeighbours(insect)) {

                if (m.isSlotFree(neighbor)) {
                    // Vérifier la couleur de l'insecte à ce voisin
                        if (m.getInsectAt(insect)->getColor() == color_insect) {
                            sameColorSet.insert(neighbor);
                        } else {
                            differentColorSet.insert(neighbor);
                        }
                    } else {
                        // Ajouter les cases vides à `toCheck`
                        toCheck.insert(neighbor);
                    }
                }
            }

        // Faire la différence entre les ensembles
        std::set<vec2i> validPositions = sameColorSet;
        for (const auto& slot : differentColorSet) {
            validPositions.erase(slot);
        }

        // Convertir l'ensemble des positions valides en vecteur
        possiblePlace.assign(validPositions.begin(), validPositions.end());

        return possiblePlace;

    } catch (const std::string& e) {
        throw HiveException("Insect::setRule", e);
    } catch (const std::exception& e) {
        throw HiveException("Insect::setRule", e.what());
    } catch (...) {
        throw HiveException("Insect::setRule", "Erreur inattendue lors du calcul des emplacements possibles.");
    }
}

//Fonction qui détermine si en enlevant un insecte la ruche est toujours lié (si l'insecte à le droit de bouger)
bool Insect:: isLinkingHive(Map &m) const {
    try{
    // On parcourt toute la map pour compter son nombre d'insecte et garder une valeur d'insect
    int found_i = -1, found_j, nbInsect = 0;
    for(int i = 0; i < m.getSideSize(); i++) {
        for(int j = 0; j < m.getSideSize(); j++) {
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
    while (insectSet.size() > i_insect) // On itère dans insectSet tant qu'il y a des elements ;
    {
        neighbors = m.getNeighbours(insectSet[i_insect]);
        for (auto it : neighbors) {// On itère dans les voisins
            if(m.isSlotFree(it) == 0) // Quand on trouve un voisin insect de l'elem on l'ajoute dans la liste insectset si pas encore dedans sauf si c'est l'insect sur lequel on a appelé la fonction
            {
                auto search=std::find(insectSet.begin(), insectSet.end(), it);
                if (search==insectSet.end() && it != getCoordinates()) {
                    // L'élément n'est pas trouvé, donc on l'ajoute
                    insectSet.push_back(it);
                }
            }
        }
        i_insect++;
    }
    return i_insect != nbInsect;
    } catch (const std::string& e) {
        throw HiveException("Insect::isLinkingHive", e);
    }
    catch (...) {
        throw HiveException("Insect::isLinkingHive", "Erreur dans isLinkingHive");
    }
}
//Fonction former Neighbour pour détecter les anciens voisin à la nouvelle position : POUR BOUGER, IL FAUT STRICTEMENT 1 ANCIEN VOISIN!!
int Insect::getFormerNeighbour(vec2i oldPosition, vec2i newPosition, Map &m) const{
    try{
    int count=0;
    std::list<vec2i> formerNeighbour=m.getNeighbours(oldPosition);
    std::list<vec2i> newNeighbours=m.getNeighbours(newPosition);
    for (auto it : formerNeighbour) {
        for (auto itn :newNeighbours) {
            if(!m.isSlotFree(it) && !m.isSlotFree(itn)) {
                if (it == itn) {
                    count++;
                }
            }
        }
    }
    return count;
    } catch (const std::string& e) {
        throw HiveException("Insect::getFormerNeighbor", e);
    }
    catch (...) {
        throw HiveException("Insect::getFormerNeighbor", "Erreur dans getFormerNeighbor");
    }
}



// Méthodes de Bee
// Fonction qui dit si la reine abeille est encerclée (partie terminée)
bool Bee::isCircled(Map &m) {
    try{
    std::list<vec2i> neighbors = m.getNeighbours(this->getCoordinates());

    /*qDebug() << "Voisins trouvés :";
    for (const auto& n : neighbors) {
        qDebug() << "  Voisin :" << n;
    }*/

    for (auto it : neighbors) { // On itère dans les voisins
        // Quand on trouve une case vide, on renvoie false. La reine n'est pas encerclée
        if(m.isSlotFree(it)) return false;
    }
    // La reine est encerclée.
    return true;
    } catch (const std::string& e) {
        throw HiveException("Bee::isCircled", e);
    }
    catch (...) {
        throw HiveException("Bee::isCircled", "Erreur dans isCircled de Bee");
    }
}

// Renvoi un vector avec les mouvements possibles de la reine abeille
std::vector<vec2i> Bee::getPossibleMovements(Map &m) const {
    try{
    std::vector<vec2i> possibleMovements;
    // Vérifie que l'abeille puisse bouger sans casser la ruche en deux
    if(!this->isLinkingHive(m) && m.isSlotUsable(getCoordinates())) {
        std::list<vec2i> neighbors = m.getNeighbours(getCoordinates()); // Récupère les voisins de la case
        // Parcourt chaque voisin
        for (auto neighbor : neighbors) {
            // Si le slot est libre ET qu'il y a exactement un ancien voisin parmi les nouveaux
            if(m.isSlotFree(neighbor) == true  && getFormerNeighbour(getCoordinates(),neighbor, m) == 1) {
                possibleMovements.push_back(neighbor); // Ajoute la case valide
            }
        }
    }
    return possibleMovements;
    } catch (const std::string& e) {
        throw HiveException("Bee::getPossibleMovements", e);
    }
    catch (...) {
        throw HiveException("Bee::getPossibleMovements", "Erreur dans la fonction pour récupérer les mouvements de Bee");
    }
}



// Méthodes de Beetle
std::vector<vec2i> Beetle:: getPossibleMovements(Map &m) const{
    try{
    std::vector<vec2i> possibleMovements;
    std::list<vec2i> neighbors = m.getNeighbours(getCoordinates()); // Récupère la liste des cases voisines

    // Si le scarabée est au-dessus d'un autre insecte
    if (isAboveOf != nullptr) {
        // Alors, on ajoute tous les voisins directement
        possibleMovements.insert(possibleMovements.end(), neighbors.begin(), neighbors.end());
    } else if (!this->isLinkingHive(m)) { // Sinon si le scarabée peut bouger sans casser la ruche en deux
        // Alors, on boucle sur tous les voisins
        for (const auto &neighbor : neighbors) {
            // Et si le scarabée est au-dessus d'une pièce ou s'il y a exactement un ancien voisin parmi les nouveaux
            if (!m.isSlotFree(neighbor) == 1  || getFormerNeighbour(getCoordinates(), neighbor, m) == 1) {
                // Alors, on ajoute le voisin en question
                possibleMovements.push_back(neighbor);
            }
        }
    }
    return possibleMovements;
    } catch (const std::string& e) {
        throw HiveException("Beetle::getPossibleMovements", e);
    }catch (...) {
        throw HiveException("Beetle::getPossibleMovements", "Erreur dans la fonction pour récupérer les mouvements de Beetle");
    }
}



//Grasshoper
std::vector<vec2i> Grasshopper::getPossibleMovements(Map &m) const {
    std::vector<vec2i> possibleMovements;

    try {
        // Si le déplacement casse la continuité de la ruche, aucun mouvement n'est possible
        if (this->isLinkingHive(m)) {
            return possibleMovements; // Retourne une liste vide
        }

        // Récupérer les voisins hexagonaux directs
        std::list<vec2i> neighboursList = m.getNeighbours(getCoordinates());
        std::list<vec2i> neighbours;
        int count = 0;
        for (const auto& neighbour : neighboursList) {

            // Vérifie si le voisin contient une pièce
            if (!m.isSlotFree(neighbour)) {
                vec2i current = neighbour;

                //vec2i direction = current - getCoordinates(); // Calculer la direction du mouvement

                // Avancer dans la direction jusqu'à trouver une case libre ou sortir des limites
                while ( !m.isSlotFree(current)) {
                    neighbours = m.getNeighbours(current);
                    auto it = std::begin(neighbours);
                    std::advance(it, count); // Avance l'itérateur jusqu'à l'indexe count
                    current = *it;
                }

                // Ajouter la case libre trouvée si elle est valide
                if ( m.isSlotFree(current)) {
                    possibleMovements.push_back(current);
                }
            }
            count++;
        }
    } catch (const std::string& e) {
        throw HiveException("Grasshopper::getPossibleMovements", e);
    } catch (...) {
        throw HiveException("Grasshopper::getPossibleMovements", "Erreur inattendue lors du calcul des mouvements possibles.");
    }

    return possibleMovements; // Retourner la liste finale
}




// Méthodes de Ant
std::vector<vec2i> Ant:: getPossibleMovements(Map &m) const {
    std::vector<vec2i> possibleMovements;
    try {

        std::vector<vec2i> possiblePlace;
        if(!isLinkingHive(m) && m.isSlotUsable(getCoordinates())) {
            // Découvrir tous les insectes connectés
            std::vector<vec2i> insectSet = {this->getCoordinates()}; // Liste des insectes connectés
            std::set<vec2i> visited;                      // Ensemble pour éviter les doublons
            std::set<vec2i> toCheck;                      // Cases vides adjacentes à vérifier

            // Parcours dynamique avec `while`
            size_t index = 0; // Indice pour parcourir `insectSet`
            while (index < insectSet.size()) {
                vec2i current = insectSet[index];
                visited.insert(current);

                // Récupérer les voisins de l'insecte courant
                for (const auto& neighbor : m.getNeighbours(current)) {
                    if (!m.isSlotFree(neighbor)) {
                        // Si un insecte voisin est trouvé, l'ajouter à `insectSet` s'il n'a pas encore été visité
                        if (visited.find(neighbor) == visited.end() &&
                            std::find(insectSet.begin(), insectSet.end(), neighbor) == insectSet.end()) {
                            insectSet.push_back(neighbor);
                            }
                    }
                }
                ++index; // Avancer l'indice pour explorer le prochain insecte
            }
            //const vec2i& current = this->getCoordinates();
            visited.erase(getCoordinates());
            // Convertir l'ensemble des positions valides en vecteur



            // Itérer sur tous les insectes dans l'ensemble
            for (const auto& insect : visited) {
                for (const auto& neighbor : m.getNeighbours(insect)) {

                    if (m.isSlotFree(neighbor)) {
                        // Ajouter les cases vides à `toCheck`
                        toCheck.insert(neighbor);
                    }
                }
            }

            /*for (const auto& slot : insectSet) {
                toCheck.erase(slot);
            }*/
            for (const auto& slot : toCheck) {
                if(!m.isSlotUsable(slot)) toCheck.erase(slot);
            }
            // Convertir l'ensemble des positions valides en vecteur
            possiblePlace.assign(toCheck.begin(), toCheck.end());


        }
        return possiblePlace;

    } catch (const std::string& e) {
        throw HiveException("Map::setRule", e);
    } catch (const std::exception& e) {
        throw HiveException("Map::setRule", e.what());
    } catch (...) {
        throw HiveException("Map::setRule", "Erreur inattendue lors du calcul des emplacements possibles.");
    }

}

// Méthodes de spider
std::vector<vec2i> Spider::getPossibleMovements(Map &m) const {
    try{
        std::vector<vec2i> possibleMovements;

        // Vérifie que l'insecte puisse bouger
        if (!this->isLinkingHive(m) && m.isSlotUsable(getCoordinates())) {
            // 1er niveau de voisins
            std::list<vec2i> firstLevel = m.getNeighbours(getCoordinates());

            for (const auto &level1 : firstLevel) {
                // Si le slot est libre ET qu'il y a exactement un ancien voisin parmi les nouveaux
                if (m.isSlotFree(level1) == 1 && getFormerNeighbour(getCoordinates(),level1, m) == 1) {
                    // 2e niveau de voisins
                    std::list<vec2i> secondLevel = m.getNeighbours(level1);

                    for (const auto &level2 : secondLevel) {
                        // Idem level1 + vérification que le voisin2 ne soit pas la position de départ
                        if (m.isSlotFree(level2) && level2 != getCoordinates() && getFormerNeighbour(level1, level2, m) == 1) {
                            // 3e niveau de voisins
                            std::list<vec2i> thirdLevel = m.getNeighbours(level2);

                            for (const auto &level3 : thirdLevel) {
                                // Idem level1 + vérification que le voisin3 ne soit pas la position level1
                                if (m.isSlotFree(level3) && level3 != level1 && getFormerNeighbour(level2,level3, m) == 1) {
                                    // Test qu'il n'y ait pas la position de départ dans la liste des voisins de la position d'arrivée
                                    std::list<vec2i> fourthLevel = m.getNeighbours(level3);
                                    bool test = true;
                                    for (const auto &level4 : fourthLevel) {
                                        if (level4 == getCoordinates()) {
                                            test = false;
                                        }
                                    }
                                    // Si le test est vérifié, ajout de la position
                                    if (test == true) {
                                        possibleMovements.push_back(level3);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        return possibleMovements;
    } catch (const std::string& e) {
        throw HiveException("Spider::getPossibleMovements", e);
    }catch (...) {
        throw HiveException("Spider::getPossibleMovements", "Erreur dans la fonction pour récupérer les mouvements de Spider");
    }
}


std::vector<vec2i> Ladybug::getPossibleMovements(Map &m) const {
    try{
        std::vector<vec2i> possibleMovements;
        // Vérifie que l'insecte puisse bouger
        if (!this->isLinkingHive(m)) {
            // 1er niveau de voisins
            std::list<vec2i> firstLevel = m.getNeighbours(getCoordinates());

            for (const auto &level1 : firstLevel) {
                // Si le slot contient un pion, on peut monter dessus

                if (!m.isSlotFree(level1)) {
                    // 2e niveau de voisins
                    std::list<vec2i> secondLevel = m.getNeighbours(level1);

                    for (const auto &level2 : secondLevel) {
                        //on verifie quye les cases du niveau 2 contiennent un pion et q'on ne retourne pas à la case d'origine
                        if (!m.isSlotFree(level2) && level2 != getCoordinates()) {
                            // 3e niveau de voisins
                            std::list<vec2i> thirdLevel = m.getNeighbours(level2);

                            for (const auto &level3 : thirdLevel) {
                                // on redescend de la ruche en dernier mouvement, dans une case vide
                                if (m.isSlotFree(level3) && level3 != getCoordinates()) {
                                    possibleMovements.push_back(level3);
                                }
                            }
                        }
                    }
                }
            }
        }
        return possibleMovements;
    } catch (const std::string& e) {
        throw HiveException("Spider::getPossibleMovements", e);
    }
    catch (...) {
        throw HiveException("Spider::getPossibleMovements", "Erreur dans lafonction pour récupérer les mouvements de Spider");
    }
}

// Méthodes de mosquito
std::vector<vec2i> Mosquitoe:: getPossibleMovements(Map &m) const{
    try{
        if (!this->isLinkingHive(m)) {
            std::list<vec2i> neighbours = m.getNeighbours(getCoordinates()); // On récupère la liste des cases voisines
            std::set<vec2i> possibleMovements;
            for (auto it = neighbours.begin(); it != neighbours.end(); ++it) {
                //On itère dans chaque case voisine
                if(!m.isSlotFree(*it)){ // Si la case est occupé on détermine le type d'insecte et on applique sa
                    // method getPossibleMovement où on ajoute les cases dans l'ensemble
                    switch(m.getInsectAt(*it)->getIT()) {
                        case bee:
                            if( m.isSlotUsable(getCoordinates())){
                            for (const auto& movement : Bee::getPossibleMovements(m)) {
                                possibleMovements.insert(movement);
                            }

                            }
                        break;
                        case ant:
                            if( m.isSlotUsable(getCoordinates())) {
                                for (const auto& movement : Ant::getPossibleMovements(m)) {
                                    possibleMovements.insert(movement);
                                }
                            }
                        break;
                        case grasshopper:
                            for (const auto& movement : Grasshopper::getPossibleMovements(m)) {
                                possibleMovements.insert(movement);
                            }

                        break;
                        case spider:
                            if( m.isSlotUsable(getCoordinates())) {
                                for (const auto& movement : Spider::getPossibleMovements(m)) {
                                    possibleMovements.insert(movement);
                                }
                            }

                        break;
                        case beetle:
                            for (const auto& movement : Beetle::getPossibleMovements(m)) {
                                possibleMovements.insert(movement);
                            }
                        break;
                        case ladybug:
                            for (const auto& movement : Ladybug::getPossibleMovements(m)) {
                                possibleMovements.insert(movement);
                            }
                        break;
                    }}
            }

            // Converti l'ensemble en vector
            std::vector<vec2i> possibleMovementsVector(possibleMovements.begin(), possibleMovements.end());
            return possibleMovementsVector;
        }
    } catch (const std::string& e) {
        throw HiveException("Mosquitoe::getPossibleMovements", e);
    }
    catch (...) {
        throw HiveException("Mosquitoe::getPossibleMovements", "Erreur dans la fonction pour récupérer les mouvements de Mosquitoe");
    }
}





#endif //HIVE_INSECT_CPP
