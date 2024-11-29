#include "src/features/map.h"
#include "src/renderer.h"
#include "src/features/insect.h"
#include <limits>

int main() {
    int sideSize = 8; // Taille de la carte
    int rewindLimit = 5;
    Map gameMap(sideSize, rewindLimit);

    // Ajouter des insectes pour tester
    Ant Ant1(true);
    Bee Bee1(true);
    Beetle Beetle1(true);
    // Passer les pointeurs des objets à putInsectTo
    gameMap.putInsectTo(&Ant1, vec2i(3, 4));  // Passez une référence à Ant1
    gameMap.putInsectTo(&Bee1, vec2i(2, 2));  // Passez une référence à Bee1
    gameMap.putInsectTo(&Beetle1, vec2i(3, 5));

    // Afficher la carte
    Renderer renderer(gameMap);
    renderer.displayMap();

    std::cout << "Appuyez sur Entree pour quitter...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return 0;
}
