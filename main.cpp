#include "src/features/map.h"
#include "src/renderer.h"
#include "src/features/insect.h"

int main() {
    int sideSize = 8; // Taille de la carte
    int rewindLimit = 5;
    Map gameMap(sideSize, rewindLimit);

    // Ajouter des insectes pour tester
    Ant Ant1(true); // Instanciation correcte de l'objet Ant
    Bee Bee1(true);  // Instanciation correcte de l'objet Bee

    // Passer les pointeurs des objets à putInsectTo
    gameMap.putInsectTo(&Ant1, vec2i(3, 4));  // Passez une référence à Ant1
    gameMap.putInsectTo(&Bee1, vec2i(2, 2));  // Passez une référence à Bee1

    // Afficher la carte
    Renderer renderer(gameMap);
    renderer.displayMap();

    return 0;
}
