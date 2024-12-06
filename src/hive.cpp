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
#include "Qt/main.cpp"



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
        renderer = new ConsoleRenderer(map, &player1, &player2, 30);
        version = console;
    } else {
        renderer = new GraphicRenderer(map, &player1, &player2, 30);
        version = graphic;
    }

    int choice = 0;
    do {
        std::cout << "\n=== Menu Hive ===\n";
        std::cout << "1. Start a game\n";
        std::cout << "2. Tutorial\n";
        std::cout << "3. Resume last game\n";
        std::cout << "4. Change parameters\n";
        std::cout << "5. Save current game\n";
        std::cout << "6. Leave\n";
        std::cout << "Choose an option : ";
        std::cin >> choice;

        switch (choice) {
            case 1:  // Start a game
                std::cout << "Starting a new game...\n";
                return 1;

            case 2:  // Tutorial
                std::cout << "Launching tutorial...\n";
                displayRules();
                break;

            case 3:  // Resume game
                std::cout << "Resuming the last game...\n";
                // Insérez ici le code pour reprendre une partie
                break;

            case 4:  // Change parameters
                std::cout << "Changing parameters...\n";
                changeSettings();
                break;

            case 5:  // Save game
                std::cout << "Saving...\n";
                break;
            case 6:  // Leave
                std::cout << "Au revoir !\n";
            break;
            default:
                std::cout << "Option invalide, veuillez réessayer.\n";
                break;
        }
        std::cout << std::endl;

    } while (choice != 5);

    return 0;
}

void Hive::displayRules() {
        std::cout << "=== Règles du jeu Hive ===\n";
        std::cout << "1. Hive est un jeu de stratégie abstrait où deux joueurs s'affrontent.\n";
        std::cout << "2. Le but du jeu est de capturer complètement la reine de l'adversaire en l'entourant.\n";
        std::cout << "3. Chaque joueur possède un ensemble d'insectes avec des mouvements spécifiques :\n";
        std::cout << "   - La Reine (Queen Bee) : doit être posée dans les 4 premiers tours. Elle bouge d'une case à la fois.\n";
        std::cout << "   - Les Fourmis (Ants) : peuvent se déplacer n'importe où autour de la ruche.\n";
        std::cout << "   - Les Araignées (Spiders) : se déplacent exactement de 3 cases.\n";
        std::cout << "   - Les Scarabées (Beetles) : peuvent grimper sur d'autres pièces.\n";
        std::cout << "   - Les Sauterelles (Grasshoppers) : sautent en ligne droite par-dessus des pièces.\n";
        std::cout << "4. Les pièces doivent toujours rester connectées pour former une seule ruche.\n";
        std::cout << "5. Aucun mouvement ne peut séparer la ruche en plusieurs parties.\n";
        std::cout << "6. Le premier joueur à entourer complètement la reine de l'adversaire gagne la partie.\n";
        std::cout << "7. Si les deux reines sont entourées au même moment, c'est une égalité.\n";
        std::cout << "8. Les joueurs jouent à tour de rôle, en posant une nouvelle pièce ou en déplaçant une pièce déjà posée.\n";
        std::cout << "\nAmusez-vous bien et bonne chance !\n";
    }


void Hive::changeSettings() {
    int choice = 0;

    do {
        std::cout << "\n=== Modifier les paramètres du jeu ===\n";
        std::cout << "1. Activer/Désactiver les extensions\n";
        std::cout << "2. Modifier le nombre de rewinds\n";
        std::cout << "3. Choisir le mode de jeu (IA vs Humain ou Humain vs Humain)\n";
        std::cout << "4. Quitter le menu des paramètres\n";
        std::cout << "Choisissez une option : ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                char ladybugChoice, mosquitoeChoice;
                std::cout << "Activer l'extension Ladybug ? (o/n) : ";
                std::cin >> ladybugChoice;
                std::cout << "Activer l'extension Mosquitoe ? (o/n) : ";
                std::cin >> mosquitoeChoice;

                // Traitement des choix pour les extensions
                if (ladybugChoice == 'o' || ladybugChoice == 'O') {
                    std::cout << "Extension Ladybug activée.\n";
                    extensions.insert(ladybug);
                } else {
                    std::cout << "Extension Ladybug désactivée.\n";
                    extensions.erase(ladybug);
                }

                if (mosquitoeChoice == 'o' || mosquitoeChoice == 'O') {
                    std::cout << "Extension Mosquitoe activée.\n";
                    extensions.insert(mosquitoe);
                } else {
                    std::cout << "Extension Mosquitoe désactivée.\n";
                    extensions.erase(mosquitoe);
                }
                break;
            }
            case 2: {
                int newRewindNb;
                do {
                    std::cout << "Entrez le nombre de rewinds autorisés (0 à 10) : ";
                    std::cin >> newRewindNb;

                    if (newRewindNb < 0 || newRewindNb > 10) {
                        std::cout << "Valeur invalide, veuillez réessayer.\n";
                    }
                } while (newRewindNb < 0 || newRewindNb > 10);

                rewindNb = newRewindNb;
                std::cout << "Nombre de rewinds mis à jour à : " << rewindNb << "\n";
                break;
            }
            case 3: {
                int modeChoice;
                do {
                    std::cout << "Choisissez un mode de jeu :\n";
                    std::cout << "1. Joueur vs Joueur (PvP)\n";
                    std::cout << "2. Joueur vs IA (PvAI)\n";
                    std::cout << "Votre choix : ";
                    std::cin >> modeChoice;

                    if (modeChoice != 1 && modeChoice != 2) {
                        std::cout << "Option invalide, veuillez réessayer.\n";
                    }
                } while (modeChoice != 1 && modeChoice != 2);

                mode = (modeChoice == 1) ? PvP : PvAI;
                std::cout << "Mode de jeu mis à jour à : " << (mode == PvP ? "Joueur vs Joueur" : "Joueur vs IA") << "\n";
                break;
            }
            case 4:
                std::cout << "Sortie du menu des paramètres.\n";
                break;
            default:
                std::cout << "Option invalide, veuillez réessayer.\n";
                break;
        }
    } while (choice != 4);
}
void saveGame(){}
void loadGame(){}


