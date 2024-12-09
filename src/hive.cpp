//
// Created by Thiba on 04/12/2024.
//

#include <vector>
#include <fstream>
#include <iostream>

#include <string>
#include <stdexcept>
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
                loadGame("hive_parameters");
                break;

            case 4:  // Change parameters
                std::cout << "Changing parameters...\n";
                changeSettings();
                break;

            case 5:  // Save game
                std::cout << "Saving...\n";
                saveGame("hive_parameters");
                break;
            case 6:  // Leave
                std::cout << "Au revoir !\n";
            break;
            default:
                std::cout << "Option invalide, veuillez réessayer.\n";
                break;
        }
        std::cout << std::endl;

    } while (choice != 6);

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


void Hive::saveGame(const std::string& filename) const {
        std::ofstream file(filename, std::ios::trunc);

        if (!file.is_open()) {
            throw HiveException("saveGame", "Impossible d'ouvrir le fichier de sauvegarde.");
        }

        // Sauvegarder les informations de base (exemple : mode, version)
        file << "Mode: " << static_cast<int>(mode) << std::endl;
        file << "Version: " << static_cast<int>(version) << std::endl;
        file << "isInit: " << static_cast<bool>(isInit) << std::endl;
        file << "trueMapSideSize: " << trueMapSideSize << std::endl;
        file << "renderedMapSideSize: " << renderedMapSideSize << std::endl;
        // Sauvegarder insects
        file << "Insects_Hive:" << std::endl;
        for (const auto& insect : insects) {
            file << insect->getID() << " " << insect->getIT() << " " << insect->getCoordinates().getI()
            << ","<< insect->getCoordinates().getJ()<< " "<< insect->getColor()<< std::endl;
        }
        file << "counter:"<< Insect::get_counter() << std::endl;
        file << "Fin_Insects_Hive:" << std::endl;
        file << "Extensions:" << std::endl;
        for (const auto& extension : extensions) {
            file << extension << std::endl;
        }
        file << "Fin_Extensions:" << std::endl;

        // Sauvegarder l'état de la carte (Map)
        file << "Map:" << std::endl;
        for (const auto& movement : map.getHistoric()) {
            file << movement.getFrom().getI() << "," <<movement.getFrom().getJ() << " "  <<
                movement.getTo().getI() << "," <<movement.getTo().getJ() << std::endl;
        }
        file << map.getRelativePos().getI() << "," <<map.getRelativePos().getJ() << std::endl;
        file << "sideSize "<< map.getSideSize() << std::endl;
        file << "rewind "<< map.getRewind() << std::endl;
        file << "Fin_Map: " << std::endl;

        file << "Joueur1:" << std::endl;
        // Sauvegarder les joueurs
        file << "ID: " << player1.getId() << std::endl;
        file << "isHuman: " << player1.isHuman << std::endl;
        file << "Player Name: " << player1.getName() << std::endl;

        // Sauvegarder les éléments du Deck du Player
        file << "Deck Size: " << player1.getDeck().getInsects()->size() << std::endl;

        // Sauvegarder les id des Insects actifs du Player
        file << "Active Insects Count: " << player1.getActiveInsects().size() << std::endl;
        for (Insect* insect : player1.getActiveInsects()) {
            file << "Insect ID: " << insect->getID() << std::endl;
        }

        // Sauvegarder de Input
        const Inputs input = player1.getInputs();
        file << "Start Position: " << input.getStart().getI() << ", " << input.getStart().getJ()<< std::endl;
        for (const auto& dest : input.getPossibleDestinations()) {
            file  << dest.getI() << "," << dest.getJ() << std::endl;
        }
        file << "startSelected: " << input.getStartSelected() << std::endl;
        file << "destinationSelected: " << input.getDestinationSelected() << std::endl;
        file << "destinationIndex: " << input.getDestinationIndex() << std::endl;
        file << "needPossibleDestinations: " << input.getNeedPossibleDestinations() << std::endl;
        file << "message: " << input.getMessage() << std::endl;
        file << "Fin_Joueur1: " << std::endl;


        file << "Joueur2:" << std::endl;
        // Sauvegarder les joueurs
        file << "ID: " << player2.getId() << std::endl;
        file << "isHuman: " << player2.isHuman << std::endl;
        file << "Player Name: " << player2.getName() << std::endl;

        // Sauvegarder les éléments du Deck du Player
        file << "Deck Size: " << player2.getDeck().getInsects()->size() << std::endl;

        // Sauvegarder les id des Insects actifs du Player
        file << "Active Insects Count: " << player2.getActiveInsects().size() << std::endl;
        for (Insect* insect : player2.getActiveInsects()) {
            file << "Insect ID: " << insect->getID() << std::endl;
        }

        // Sauvegarder de Input
        const Inputs input2 = player2.getInputs();
        file << "Start Position: " << input2.getStart().getI() << ", " << input2.getStart().getJ() << std::endl;
        for (const auto& dest : input2.getPossibleDestinations()) {
            file << dest.getI() << "," << dest.getJ() << std::endl;
        }
        file << "startSelected: " << input2.getStartSelected() << std::endl;
        file << "destinationSelected: " << input2.getDestinationSelected() << std::endl;
        file << "destinationIndex: " << input2.getDestinationIndex() << std::endl;
        file << "needPossibleDestinations: " << input2.getNeedPossibleDestinations() << std::endl;
        file << "message: " << input2.getMessage() << std::endl;
        file << "Fin_Joueur2: " << std::endl;
        file << "Current_Player: " << std::endl;
        file << "ID: " << player2.getId() << std::endl;
        file << "Fin_Current_Player: " << std::endl;
        file << "InputManager: " << std::endl;
        file << "InputManager_Fin: " << std::endl;







        file.close();
        std::cout << "Partie sauvegardée avec succès dans " << filename << std::endl;
    }
void Hive::loadGame(const std::string& filename){}







