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
                saveGame("hive_game");
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
//void saveGame(){}
//void Hive::loadGame(){}

/*void Hive::loadGame(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);

    if (!inFile.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour le chargement : " + filename);
    }
        // Chargement des attributs de Hive
    inFile.read(reinterpret_cast<char *>(&version), sizeof(version));
    inFile.read(reinterpret_cast<char *>(&rewindNb), sizeof(rewindNb));

    // Chargement des joueurs
    for (Player& player : {player1, player2}) {
        inFile.read(reinterpret_cast<char*>(&player.deck), sizeof(player.deck));
        inFile.read(reinterpret_cast<char*>(&player.id), sizeof(player.id));
        inFile.read(reinterpret_cast<char*>(&player.isHuman), sizeof(player.isHuman));
        inFile.read(reinterpret_cast<char*>(&player.name), sizeof(player.name));

        // Chargement du deck
        size_t deckSize;
        inFile.read(reinterpret_cast<char*>(&deckSize), sizeof(deckSize));

        // Chargement des insectes du deck
        for (size_t i = 0; i < deckSize; ++i) {
            int insectId;
            inFile.read(reinterpret_cast<char*>(&insectId), sizeof(insectId));
            // Utilisez l'ID pour récupérer l'insecte dans votre collection d'insectes
            // player.getDeck().addInsect(insectId); // Si vous avez une méthode pour ajouter l'insecte au deck
        }

        // Chargement des insectes actifs
        size_t activeInsectsCount;
        inFile.read(reinterpret_cast<char*>(&activeInsectsCount), sizeof(activeInsectsCount));
        for (size_t i = 0; i < activeInsectsCount; ++i) {
            int insectId;
            inFile.read(reinterpret_cast<char*>(&insectId), sizeof(insectId));
            // Ajoutez l'insecte actif en utilisant l'ID (si vous avez une méthode pour cela)
            // player.addActiveInsect(insectId);
        }
    }

    // Chargement des insectes globaux
    size_t insectCount;
    inFile.read(reinterpret_cast<char*>(&insectCount), sizeof(insectCount));
    for (size_t i = 0; i < insectCount; ++i) {
        int id2, coorI, coorJ;
        bool col;
        inFile.read(reinterpret_cast<char*>(&id2), sizeof(id2));
        inFile.read(reinterpret_cast<char*>(&col), sizeof(col));
        inFile.read(reinterpret_cast<char*>(&coorI), sizeof(coorI));
        inFile.read(reinterpret_cast<char*>(&coorJ), sizeof(coorJ));

        // Créez et ajoutez l'insecte avec les informations lues
        // Insect* insect = createInsect(id2, col, coorI, coorJ); // Exemple de création d'un insecte
        // insects.push_back(insect); // Ajoutez l'insecte au conteneur
    }

    // Chargement de la carte
    size_t slotCount;
    inFile.read(reinterpret_cast<char*>(&slotCount), sizeof(slotCount));
    for (size_t i = 0; i < slotCount; ++i) {
        int insectId;
        inFile.read(reinterpret_cast<char*>(&insectId), sizeof(insectId));
        // Ajoutez l'insecte dans le slot correspondant (ou affectez l'ID de l'insecte)
        // map.addInsectToSlot(insectId, i); // Exemple de méthode pour ajouter un insecte à une case de la carte
    }

    // Chargement de la position relative de la carte
    vec2i relativePos;
    inFile.read(reinterpret_cast<char*>(&relativePos), sizeof(relativePos));

    // Chargement de la taille de la carte
    int sideSize;
    inFile.read(reinterpret_cast<char*>(&sideSize), sizeof(sideSize));

    // Si vous avez des fonctions pour définir la carte avec ces valeurs :
    // map.setRelativePos(relativePos);
    // map.setSideSize(sideSize);

    inFile.close();
}

*/



void Hive::saveGame(const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::binary);

    if (!outFile.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier pour la sauvegarde : " + filename);
    }

    // Sauvegarde des attributs de Hive
    outFile.write(reinterpret_cast<const char*>(&mode), sizeof(mode));
    outFile.write(reinterpret_cast<const char*>(&version), sizeof(version));
    outFile.write(reinterpret_cast<const char*>(&rewindNb), sizeof(rewindNb));

    // Sauvegarde des joueurs
    for (const Player& player : {player1, player2}) {;
        outFile.write(reinterpret_cast<const char*>(&player.deck), sizeof(player.deck));
        outFile.write(reinterpret_cast<const char*>(&player.id), sizeof(player.id));
        outFile.write(reinterpret_cast<const char*>(&player.isHuman), sizeof(player.isHuman));
        outFile.write(reinterpret_cast<const char*>(&player.name), sizeof(player.name));


        // Sauvegarde du deck
        size_t deckSize = player.getDeck().getInsects()->size();
        outFile.write(reinterpret_cast<const char*>(&deckSize), sizeof(deckSize));
        for (const Insect* insect : *player.getDeck().getInsects()) {
            int insectId = insect ? insect->getID() : -1; // Sauvegarde uniquement les IDs des insectes
            outFile.write(reinterpret_cast<const char*>(&insectId), sizeof(insectId));
        }

        // Sauvegarde des insects actifs
        size_t activeInsectsCount = player.getActiveInsects().size();
        outFile.write(reinterpret_cast<const char*>(&activeInsectsCount), sizeof(activeInsectsCount));
        for (const Insect* insect : player.getActiveInsects()) {
            int insectId = insect ? insect->getID() : -1;
            outFile.write(reinterpret_cast<const char*>(&insectId), sizeof(insectId));
        }
    }

    // Sauvegarde des insects globaux
    size_t insectCount = insects.size();
        int id2, coorI, coorJ;
        bool col;

    outFile.write(reinterpret_cast<const char*>(&insectCount), sizeof(insectCount));
    for (const auto& insect : insects) {
        id2 = insect->getID();
        col = insect->getColor();
        coorI = insect->getCoordinates().getI();
        coorJ = insect->getCoordinates().getJ();
        outFile.write(reinterpret_cast<const char*>(&id2), sizeof(id2));
        outFile.write(reinterpret_cast<const char*>(&col), sizeof(col));
        outFile.write(reinterpret_cast<const char*>(&coorI), sizeof(coorI));
        outFile.write(reinterpret_cast<const char*>(&coorJ), sizeof(coorJ));
    }

    // Sauvegarde de la carte
    size_t slotCount = map.getSlots().size();
    outFile.write(reinterpret_cast<const char*>(&slotCount), sizeof(slotCount));
    for (const auto& slot : map.getSlots()) {
        int insectId = slot ? slot->getID() : -1;
        outFile.write(reinterpret_cast<const char*>(&insectId), sizeof(insectId));
    }
        vec2i relativePos = map.getRelativePos();
    outFile.write(reinterpret_cast<const char*>(&relativePos), sizeof(relativePos));
    outFile.write(reinterpret_cast<const char*>(&map.getSideSize()), sizeof(map.getSideSize()));

    outFile.close();
}






