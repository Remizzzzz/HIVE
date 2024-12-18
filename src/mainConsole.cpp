#include "features/map.h"
#include "renderer.h"
#include "features/insect.h"
#include <limits>

#include "hive.h"

#include <conio.h>  // Pour _kbhit() et _getch()
#include <fstream>
#include <unistd.h>
int main() {
    bool play = true;

    Hive hive;
    hive.generateAllInsects();
    //hive.getMap().putInsectTo(hive.getInsects()[0], vec2i(15,15));
    //hive.getInsects()[0]->setCoordinates(vec2i(15,15));
    //hive.saveGame("../hive_parameters.txt");
    //hive.displayMenu();
    for( auto insect : hive.getInsects()) {
        std::cout << insect->getCoordinates().getI() << std::endl;
    }
    //hive.displayMenu();
    hive.loadGame("../hive_parameters.txt");
    std::cout << std::endl;

    std::cout << "isInit: " << hive.initIfNeeded() << std::endl;

    std::cout << "RewindUsed: " << hive.getRewindUsed() << std::endl;
    std::cout << "RewindMax: " << hive.getRewindMax() << std::endl;

    // Affichage de l'ensemble des insectes
    std::cout << "Insects: ";
    for (auto insect : hive.getInsects()) {
        std::cout << insect << " ";  // Afficher l'adresse de chaque insecte
    }
    std::cout << std::endl;

    // Affichage de l'état du joueur actuel
    std::cout << "Current Player: " << (hive.getCurrentPlayer() ? "Player 1" : "Player 2") << std::endl;

    // Affichage des coordonnées du joueur
    std::cout << "Player 1: " << hive.getPlayer1() << std::endl;
    std::cout << "Player 2: " << hive.getPlayer2() << std::endl;

    // Affichage de l'état du joueur actuel
    std::cout << "renderer: " << hive.getRenderer() << std::endl;
    std::cout << "map: " << hive.getMap().getRewind() << std::endl;
    // Affichage de l'état du joueur actuel




    std::cout << hive.getInsects().at(0)->getCoordinates().getI() << ",";
    std::cout << hive.getInsects().at(0)->getCoordinates().getI() << std::endl;
    hive.run();
    std::cout <<"llaaaa";
    //sleep(10);
    //hive.getRenderer()->displayMap(*hive.getPlayer1());
    //throw HiveException("lala","lala");
    while(play){

        try{
            play = hive.run();
        }
        catch (const HiveException & HE_){
            std::cout << HE_.getInfos() << '\n';
        }
    }





    return 0;
}
