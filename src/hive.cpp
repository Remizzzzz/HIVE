//
// Created by Thiba on 04/12/2024.
//

#include <vector>

#include "utils/hiveException.h"

#include "features/insect.h"
#include "features/deck.h"

#include "inputsManager.h"
#include "hive.h"
#include "solver.h"
#include "renderer.h"



    /**
     * @brief Affiche un menu interactif pour l'utilisateur.
     */
    int Hive::displayMenu() {
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
            renderer = new ConsoleRenderer(map, &player1, &player2);  // initialiser l'objet inputs correctement
            version = console;
        } else if (versionChoice == 2) {
            // Allocation dynamique pour GraphicRenderer
            renderer = new GraphicRenderer(map, &player1, &player2);  // initialiser l'objet inputs correctement
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
                std::cout << "1. Afficher les règles\n";
                std::cout << "2. Lancer le jeu\n";
                std::cout << "3. Quitter\n";
                std::cout << "Choisissez une option : ";
                std::cin >> choice;

                // Gestion des choix
                switch (choice) {
                    case 1:
                        if (auto* consoleRenderer = dynamic_cast<ConsoleRenderer*>(renderer)) {
                            consoleRenderer->displayRules(); // Appel à une méthode spécifique à ConsoleRenderer
                        } else {
                            std::cout << "Affichage des règles indisponible dans ce mode.\n";
                        }
                    break;
                    case 2:
                        std::cout << "Début de la partie...\n";
                    //run(); // Lancement du jeu
                        return 1;
                    break;
                    case 3:
                        std::cout << "Au revoir !\n";
                    break;
                    default:
                        std::cout << "Option invalide, veuillez réessayer.\n";
                    break;
                }
                std::cout << std::endl;
            } while (choice != 2 && choice != 3);
        }
        else {
            //code pour lancer dans qt
        }

        return 0;
    }