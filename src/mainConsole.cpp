#include <fstream>

#include "features/insect.h"
#include "hive.h"



int mainConsole() {
    bool play = true;
    Hive hive;
    while (play) {
        try {
            play = hive.run();
        }
        catch (const HiveException & HE_) {
            std::cout << HE_.getInfos() << '\n';
        }
    }
    return 0;
}
