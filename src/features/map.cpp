//
// Created by rzong on 22/11/2024.
//
#include <set>
#include <algorithm>

#include "map.h"


std::vector<vec2i> Map::setRule(bool color_insect) {
    try {
        std::vector<vec2i> possiblePlace;

        // Trouver un insecte sur la carte
        vec2i foundInsect(-1, -1);
        for (int i = 0; i < getSideSize() && foundInsect.getI() == -1; ++i) {
            for (int j = 0; j < getSideSize(); ++j) {
                if (!isSlotFree(vec2i{i, j})) {
                    foundInsect = vec2i{i, j};
                    break;
                }
            }
        }

        // Si aucun insecte n'est trouvé, proposer le centre de la carte
        if (foundInsect.getI() == -1) {
            possiblePlace.push_back(vec2i{(getSideSize() / 2), (getSideSize() / 2)});
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
            for (const auto& neighbor : getNeighbours(current)) {
                if (!isSlotFree(neighbor)) {
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
            for (const auto& neighbor : getNeighbours(insectSet[0])) {
                if (isSlotFree(neighbor)) {
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
            for (const auto& neighbor : getNeighbours(insect)) {

                if (isSlotFree(neighbor)) {
                    // Vérifier la couleur de l'insecte à ce voisin
                    if (getInsectAt(insect)->getColor() == color_insect) sameColorSet.insert(neighbor);
                    else differentColorSet.insert(neighbor);
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
        throw HiveException("Map::setRule", e);
    } catch (const std::exception& e) {
        throw HiveException("Map::setRule", e.what());
    } catch (...) {
        throw HiveException("Map::setRule", "Erreur inattendue lors du calcul des emplacements possibles.");
    }
}


void Map::moveInsect(const vec2i & pos1_, const vec2i & pos2_) {

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
            if (targetInsect != nullptr) beetlePointer->setAboveOf(targetInsect);
            else beetlePointer->setAboveOf(nullptr);

            // Déplacer le scarabée ou moustique à la position cible
            putInsectTo(movingInsect, pos2_);

        } else if (movingInsect->getIT() == mosquitoe) {
            // Cast dynamique pour obtenir un pointeur de type Beetle
            Mosquitoe* mosqPointer = dynamic_cast<Mosquitoe*>(movingInsect);

            if (mosqPointer == nullptr) {
                throw HiveException("Map::moveInsect", "Erreur du dynamic_cast pour le scarabée.");
            }

            // Si le scarabée ou moustique a un insecte en dessous
            if (mosqPointer->getInsectUnder() != nullptr) {
                // Remettre l'insecte en dessous à la position actuelle
                putInsectTo(mosqPointer->getInsectUnder(), pos1_);
            } else {
                // Pas d'insecte en dessous, retirer l'insecte de la position initiale
                removeInsectAt(pos1_);
            }

            // Si un insecte est présent à la position cible, positionner le scarabée ou moustique au-dessus
            if (targetInsect != nullptr) mosqPointer->setAboveOf(targetInsect);
            else mosqPointer->setAboveOf(nullptr);

            // Déplacer le scarabée ou moustique à la position cible
            putInsectTo(movingInsect, pos2_);
        } else {
            // Cas général pour les autres types d'insectes
            putInsectTo(movingInsect, pos2_);
            movingInsect->setCoordinates(pos2_);
            removeInsectAt(pos1_);
        }
    }


std::list<vec2i> Map::getNeighbours(const vec2i & pos_) const {
    std::list<vec2i> neighbours{};
    if (pos_.getI() % 2 == 0) {
        neighbours.push_back(pos_ - vec2i(1,1));
        neighbours.push_back( pos_ - vec2i(1,0));
        neighbours.push_back( pos_ - vec2i(0,1));
        neighbours.push_back( pos_ - vec2i(0,-1));
        neighbours.push_back( pos_ - vec2i(-1,1));
        neighbours.push_back( pos_ - vec2i(-1,0));
    } else {
        neighbours.push_back( pos_ - vec2i(1,0));
        neighbours.push_back( pos_ - vec2i(1,-1));
        neighbours.push_back( pos_ - vec2i(0,1));
        neighbours.push_back( pos_ - vec2i(0,-1));
        neighbours.push_back( pos_ - vec2i(-1,0));
        neighbours.push_back( pos_ - vec2i(-1,-1));
    }

    return neighbours;
}


void Map::translateMap(const vec2i & tVec) {
        //Translation vers le haut donc parcours de haut en bas
        if (tVec.getI() <= 0) {
            //Translation vers la gauche donc parcours de gauche à droite
            if (tVec.getJ() <= 0) {
                for (int i = 0; i < sideSize; i++) {
                    for (int j = 0; j < sideSize; j++) {
                        vec2i curr = vec2i(i,j);
                        if (!isSlotFree(curr)) moveInsect(curr,curr + tVec);
                    }
                }
            } else {  //Translation vers la droite donc parcours de droite à gauche
                for (int i = 0; i < sideSize; i++) {
                    for (int j = sideSize - 1; j >= 0; j--) {
                        vec2i curr = vec2i(i,j);
                        if (!isSlotFree(curr)) moveInsect(curr,curr + tVec);
                    }
                }
            }
        } else {  // Translation vers le bas donc parcours de bas en haut
            // Translation vers la gauche donc parcours de gauche à droite
            if (tVec.getJ() <= 0) {
                for (int i = sideSize - 1; i >= 0; i--) {
                    for(int j = 0; j < sideSize; j++){
                        vec2i curr = vec2i(i,j);
                        if (!isSlotFree(curr)) moveInsect(curr,curr + tVec);
                    }
                }
            } else {  //Translation vers la droite donc parcours de droite à gauche
                for (int i = sideSize - 1; i >= 0; i--) {
                    for (int j = sideSize - 1; j >= 0; j--) {
                        vec2i curr = vec2i(i,j);
                        if(!isSlotFree(curr)) moveInsect(curr,curr + tVec);
                    }
                }
            }
        }
    }

