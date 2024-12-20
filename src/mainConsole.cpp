#include "features/map.h"
#include "renderer.h"
#include "features/insect.h"
#include <limits>

#include "hive.h"

#include <conio.h>  // Pour _kbhit() et _getch()
#include <fstream>

int main() {

    try
        {
        bool play = true;

        Hive hive;

        while(play){
            play = hive.run();
        }

    }
    catch (const HiveException & HE_)
    {
        std::cout << HE_.getInfos() << '\n';
    }


    return 0;
}
