//
// Created by rzong on 22/11/2024.
//
#include <set>
#include <algorithm>
#include "map.h"
/*
void Map::updateBeetlePosition(Beetle *beetle, const vec2i &newPos) {
    if (beetle == nullptr) return;
    Insect *insectBelow = getInsectAt(newPos);

    // Si un insecte est déjà présent à la position cible
    if (insectBelow != nullptr) {
        beetle->setAboveOf(insectBelow); // Mise à jour de l'attribut isAboveOf
    } else {
        beetle->setAboveOf(nullptr); // Aucun insecte en dessous, le scarabée n'est au-dessus de rien
    }

    // Met à jour la position dans la carte
    moveInsect(beetle->getCoordinates(), newPos); // Déplace l'insecte dans la carte
}*/

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
            possiblePlace.push_back(vec2i{getSideSize() / 2, getSideSize() / 2});
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
                    if (getInsectAt(insect)->getColor() == color_insect) {
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
        throw HiveException("Map::setRule", e);
    } catch (const std::exception& e) {
        throw HiveException("Map::setRule", e.what());
    } catch (...) {
        throw HiveException("Map::setRule", "Erreur inattendue lors du calcul des emplacements possibles.");
    }
}




