//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_HIVE_H
#define HIVE_HIVE_H

#include <vector>

#include "utils/hiveException.h"

#include "features/insect.h"
#include "features/deck.h"

#include "inputsManager.h"
#include "solver.h"
#include "renderer.h"

//enum Mode{PvP,PvAI};
enum Version{console, graphic};

class Hive{

    Mode mode;
    Version version;

    std::vector<Insect> insects;

    const int trueMapSideSize{32};
    const int renderedMapSideSize{30};
    int rewindNb;
    Map map;

    Player player1;
    Player player2;

    Player * currentPlayer;

    InputsManager inputsManager;

    Solver solver;

    Renderer * renderer;
    /*~Hive() {
        delete renderer;
    }*/

private:

    void switchPlayer(){
        if (currentPlayer == &player1){
            currentPlayer = &player2;
        }
        else{
            currentPlayer = &player1;
        }
    }

    void resetInputs(Player & player_){
        player_.inputs.reset();
    }

public:
    Hive()
    : mode(PvP), version(console),
                insects(),
                rewindNb(1), map(trueMapSideSize,rewindNb),
                player1(1), player2(2), currentPlayer(&player1),
                inputsManager(mode, renderedMapSideSize, map),
                solver(map, trueMapSideSize),renderer( nullptr)
{}
    Hive(Mode mode_, Version version_, int rewindNb_ = 3)
            :   mode(mode_), version(version_),
                insects(),
                rewindNb(rewindNb_), map(trueMapSideSize,rewindNb),
                player1(1), player2(2), currentPlayer(&player1),
                inputsManager(mode, renderedMapSideSize, map),
                solver(map, trueMapSideSize)
    {


        switch (version) {
            case console:
                //renderer = new ConsoleRenderer(map);
                break;
            case graphic:
                //renderer = new GraphicRenderer(map);
                break;
            default:
                renderer = nullptr;
                throw HiveException("hive.h:Hive", "invalid version");
        }
    };

    void run(){



        inputsManager.updateAIInputs(*currentPlayer);
        std::cout << currentPlayer->inputs;

        switch (solver.update(*currentPlayer)) {
            case 0:
                //le mouvement est pas bon
                resetInputs(*currentPlayer);
            case 1:
                //Le travail est en cours
            case 2:
                //mouvement fait
                resetInputs(*currentPlayer);
                switchPlayer();
        }


        inputsManager.updateAIInputs(*currentPlayer);
        std::cout << currentPlayer->inputs;
        //renderer->render(inputs);
    }

    /**
     * @brief Affiche un menu interactif pour l'utilisateur.
     */
    void displayMenu() {
        int versionChoice = 0;
        do {
            std::cout << "=== Choisissez le type d'interface ===\n";
            std::cout << "1. Interface Console (texte)\n";
            std::cout << "2. Interface Graphique\n";
            std::cout << "Choisissez une option (1 ou 2) : ";
            std::cin >> versionChoice;

            if (versionChoice != 1 && versionChoice != 2) {
                std::cout << "Option invalide, veuillez réessayer.\n";
            }
        } while (versionChoice != 1 && versionChoice != 2);

        if (versionChoice == 1) {
            // Allocation dynamique pour ConsoleRenderer
            renderer = new ConsoleRenderer(map, new Inputs());  // initialiser l'objet inputs correctement
            version = console;
        } else if (versionChoice == 2) {
            // Allocation dynamique pour GraphicRenderer
            renderer = new GraphicRenderer(map, new Inputs());  // initialiser l'objet inputs correctement
            version = graphic;
        }
        if (version == console) {
            // mode de jeu
            int modeChoice = 0;
            do {
                std::cout << "=== Configuration du jeu Hive ===\n";
                std::cout << "1. Joueur vs Joueur (PvP)\n";
                std::cout << "2. Joueur vs IA (PvAI)\n";
                std::cout << "Choisissez un mode de jeu (1 ou 2) : ";
                std::cin >> modeChoice;

                if (modeChoice != 1 && modeChoice != 2) {
                    std::cout << "Option invalide, veuillez réessayer.\n";
                }
            } while (modeChoice != 1 && modeChoice != 2);

            mode = (modeChoice == 1) ? PvP : PvAI;

            //  nombre de rewinds

            do {
                std::cout << "Combien de rewinds autorisez-vous pendant la partie ? (0 à 10) : ";
                std::cin >> rewindNb;

                if (rewindNb < 0 || rewindNb > 10) {
                    std::cout << "Nombre invalide, veuillez entrer une valeur entre 0 et 10.\n";
                }
            } while (rewindNb < 0 || rewindNb > 10);


            // affichage du menu principal
            int choice = 0;
            do {
                std::cout << "\n=== Menu Hive ===\n";
                std::cout << "1. Afficher la carte\n";
                std::cout << "2. Afficher les règles\n";
                std::cout << "3. Lancer le jeu\n";
                std::cout << "4. Quitter\n";
                std::cout << "Choisissez une option : ";
                std::cin >> choice;

                // Gestion des choix
                switch (choice) {
                    case 1:
                        renderer->displayMap();
                    break;
                    case 2:
                        if (auto* consoleRenderer = dynamic_cast<ConsoleRenderer*>(renderer)) {
                            consoleRenderer->displayRules(); // Appel à une méthode spécifique à ConsoleRenderer
                        } else {
                            std::cout << "Affichage des règles indisponible dans ce mode.\n";
                        }
                    break;
                    case 3:
                        std::cout << "Début de la partie...\n";
                    run(); // Lancement du jeu
                    break;
                    case 4:
                        std::cout << "Au revoir !\n";
                    break;
                    default:
                        std::cout << "Option invalide, veuillez réessayer.\n";
                    break;
                }
                std::cout << std::endl;
            } while (choice != 4 && choice != 3);
            delete renderer;
        }
        else {
            //code pour lancer dans qt
        }
    }

};


#endif //HIVE_HIVE_H
