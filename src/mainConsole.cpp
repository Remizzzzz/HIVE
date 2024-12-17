#include "features/map.h"
#include "renderer.h"
#include "features/insect.h"
#include <limits>

#include "hive.h"

#include <conio.h>  // Pour _kbhit() et _getch()
#include <fstream>

int main() {
    bool play = true;

    Hive hive;
    hive.generateAllInsects();
    //hive.getMap().putInsectTo(hive.getInsects()[0], vec2i(15,15));
    //hive.getInsects()[0]->setCoordinates(vec2i(15,15));
    hive.saveGame("../hive_parameters.txt");
    hive.displayMenu();
    hive.getRenderer()->displayMap(*hive.getPlayer1());
    throw HiveException("lala","lala");
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
