#include "src/features/map.h"
#include "src/renderer.h"
#include "src/features/insect.h"
int main() {
    int sideSize = 8; // Taille de la carte
    int rewindLimit = 5;
    Map gameMap(sideSize, rewindLimit);

    // Ajouter des insectes pour tester


    gameMap.putInsectTo(&Ant, vec2i(3, 4));
    gameMap.putInsectTo(&Bee, vec2i(2, 2));

    // Afficher la carte
    Renderer renderer(gameMap);
    renderer.displayMap();

    return 0;
}
