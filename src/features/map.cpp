//
// Created by rzong on 22/11/2024.
//
#include <set>
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

std::vector<vec2i> Map::setRule(bool color) const {
    try {
        std::vector<vec2i> possiblePlace;

        // Trouver un insecte sur la carte
        vec2i foundInsect(-1, -1);
        for (int i = 0; i < sideSize && foundInsect.getI() == -1; ++i) {
            for (int j = 0; j < sideSize; ++j) {
                if (!isSlotFree(vec2i{i, j})) {
                    foundInsect = vec2i{i, j};
                    break;
                }
            }
        }

        // Si aucun insecte n'est trouvé, proposer le centre de la carte
        if (foundInsect.getI() == -1) {
            possiblePlace.push_back(vec2i{sideSize / 2, sideSize / 2});
            return possiblePlace;
        }

        // Découvrir tous les insectes connectés
        std::set<vec2i> insectSet = {foundInsect};  // Utilisation d'un set pour éviter les doublons
        std::set<vec2i> toCheck;                    // Cases vides adjacentes à vérifier

        // Parcours dynamique avec `for` et itérateurs
        for (auto it = insectSet.begin(); it != insectSet.end(); ++it) {
            vec2i current = *it;

            // Récupérer les voisins de l'insecte courant
            for (const auto& neighbor : getNeighbours(current)) {
                if (!isSlotFree(neighbor)) {
                    insectSet.insert(neighbor);  // Ajouter sans vérification préalable
                } else {
                    toCheck.insert(neighbor);
                }
            }
        }

        // Si un seul insecte est présent, proposer ses six voisins
        if (insectSet.size() == 1) {
            for (const auto& neighbor : getNeighbours(*insectSet.begin())) {
                if (isSlotFree(neighbor)) {
                    possiblePlace.push_back(neighbor);
                }
            }
            return possiblePlace;
        }

        // Vérifier les cases vides adjacentes dans `toCheck`
        for (const auto& slot : toCheck) {
            bool isValid = true;

            for (const auto& neighbor : getNeighbours(slot)) {
                const Insect* insect = getInsectAt(neighbor);
                if (isSlotFree(neighbor) && insect && insect->getColor() != color) {
                    isValid = false;
                    break;
                }
            }

            if (isValid) {
                possiblePlace.push_back(slot);
            }
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



