//
// Created by rzong on 22/11/2024.
//

#include "map.h"

void Map::updateBeetlePosition(Beetle *beetle, const vec2i &newPos) {
    if (beetle == nullptr) return;
    const Insect *insectBelow = getInsectAt(newPos);

    // Si un insecte est déjà présent à la position cible
    if (insectBelow != nullptr) {
        beetle->setAboveOf(insectBelow); // Mise à jour de l'attribut isAboveOf
    } else {
        beetle->setAboveOf(nullptr); // Aucun insecte en dessous, le scarabée n'est au-dessus de rien
    }

    // Met à jour la position dans la carte
    moveInsect(beetle->getCoordinates(), newPos); // Déplace l'insecte dans la carte
}
