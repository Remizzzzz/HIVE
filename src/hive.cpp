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
#include <sstream>




/**
 * @brief Affiche un menu interactif pour l'utilisateur.
 */
void Hive::displayMenu() {
    int choice = 0;
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
        menuPart = false;
        gamePart = true;
        initIfNeeded();
        break;
        case 2:  // Tutorial
            std::cout << "Launching tutorial...\n";
        displayRules();
        break;
        case 3:  // Resume game
            std::cout << "Resuming the last game...\n";

        loadGame("../hive_parameters.txt");
        initIfNeeded();
        break;
        case 4:  // Change parameters
            std::cout << "Changing parameters...\n";
        changeSettings();
        break;

        case 5:  // Save game
            std::cout << "Saving...\n";
        saveGame("../hive_parameters.txt");
        break;
        case 6:  // Leave
            menuPart = false;
        gamePart = false;
        std::cout << "Au revoir !\n";
        break;
        default:
            std::cout << "Option invalide, veuillez réessayer.\n";
        break;
    }
    std::cout << std::endl;
}

void Hive::displayRules() {
    std::cout << "=== Comment jouer? ===\n";
    std::cout << "Une fois la partie lancée, vous pouvez renvenir dans le menu avec la touche echap.\n";
    std::cout << "Pour jouer, vous pouvez utiliser les flèches du clavier pour déplacer le curseur violet.\n";
    std::cout << "Pour séléctionner une case ou une pièce il suffit de taper sur entrer.\n";

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
                    std::cout << "Actuellement il y a "<< rewindNb<<"rewinds ,Entrez le nombre de rewinds autorisés (0 à 10) : ";
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
            case 5: {
                int selectedPlayer = 0;
                std::string newName;

                // Boucle pour s'assurer que le joueur choisi est valide
                while (selectedPlayer != 1 && selectedPlayer != 2) {
                    std::cout << "De quel joueur voulez-vous changer le nom ? : 1 ("
                              << player1.getName() << ") ou 2 (" << player2.getName() << ")\n";
                    std::cin >> selectedPlayer;

                    if (selectedPlayer != 1 && selectedPlayer != 2) {
                        std::cout << "Option invalide. Veuillez entrer 1 ou 2.\n";
                    }
                }

                std::cout << "Quel est le nouveau nom ?\n";
                std::cin >> newName;

                // Mise à jour du nom du joueur
                if (selectedPlayer == 1) {
                    player1.setName(newName);
                    std::cout << "Le nom du joueur 1 a été mis à jour à : " << player1.getName() << "\n";
                } else {
                    player2.setName(newName);
                    std::cout << "Le nom du joueur 2 a été mis à jour à : " << player2.getName() << "\n";
                }

                break;
            }
            default:
                std::cout << "Option invalide, veuillez réessayer.\n";
                break;
        }
    } while (choice != 4);
}


void Hive::saveGame(const std::string& filename) const{
    std::ofstream file(filename, std::ios::trunc);
    Player player1=this->player1;
    Player player2=this->player2;
    Map map=this->map;
    if (!file.is_open()) {
        throw HiveException("saveGame", "Impossible d'ouvrir le fichier de sauvegarde.");
    }

    // Sauvegarder les informations de base (exemple : mode, version)
    file << "Mode:"<< std::endl << static_cast<int>(mode) << std::endl;
    file << "Turn:"<< std::endl << static_cast<int>(solver.getTurn()) << std::endl;
    file << "trueMapSideSize:"<< std::endl << trueMapSideSize << std::endl;
    file << "currentPlayer:"<< std::endl << (currentPlayer == &player1 ? 1:2 )<< std::endl;
    file << "renderedMapSideSize:"<< std::endl << renderedMapSideSize << std::endl;
    file << "RewindNb:"<< std::endl << rewindNb << std::endl;

    // Sauvegarder l'état de la carte (Map)
    file << "Map:" << std::endl;
    file << "Debut_Historic:" << std::endl;
    for (auto& movement : map.getHistoric()) {
        file << movement.getFrom().getI() << " " <<movement.getFrom().getJ() << " "  <<
            movement.getTo().getI() << " " <<movement.getTo().getJ() << std::endl;
    }
    file << "Fin_Historic:" << std::endl;
    file << map.getRelativePos().getI() << std::endl <<map.getRelativePos().getJ() << std::endl;
    file << map.getSideSize() << std::endl;
    file << map.getRewind() << std::endl;
    file << "Fin_Map:"<< std::endl;


    file << "counter:"<< std::endl<< Insect::get_counter() << std::endl;

    file << "Extensions:" << std::endl;
    for (const auto& extension : extensions) {
        file << extension << std::endl;
    }
    file << "Fin_Extensions:" << std::endl;



    file << "Joueur1:" << std::endl;
    // Sauvegarder les joueurs
    file << "ID:"<< std::endl << player1.getId() << std::endl;
    file << "isHuman:" << std::endl<< player1.isHuman << std::endl;
    file << "Player Name:"<< std::endl << player1.getName() << std::endl;

    // Sauvegarder les éléments du Deck du Player
    file << "Deck Size:"<< std::endl << player1.deck.getInsects()->size() << std::endl;


    // Sauvegarder de Input
    const Inputs input = player1.getInputs();
    file << "Start Position:"<< std::endl << input.getStart().getI() << ", " << input.getStart().getJ()<< std::endl;
    for (const auto& dest : input.getPossibleDestinations()) {
        file  << dest.getI() << "," << dest.getJ() << std::endl;
    }
    file << "startSelected:" << std::endl<< input.getStartSelected() << std::endl;
    file << "destinationSelected:"<< std::endl << input.getDestinationSelected() << std::endl;
    file << "destinationIndex:"<< std::endl << input.getDestinationIndex() << std::endl;
    file << "needPossibleDestinations:"<< std::endl << input.getNeedPossibleDestinations() << std::endl;
    file << "message:" << std::endl<< input.getMessage() << std::endl;
    file << "Fin_Joueur1:"<< std::endl << std::endl;


    file << "Joueur2:" << std::endl;
    // Sauvegarder les joueurs
    file << "ID:" << std::endl << player2.getId() << std::endl;
    file << "isHuman:" << std::endl << player2.isHuman << std::endl;
    file << "Player Name:" << std::endl << player2.getName() << std::endl;

    // Sauvegarder les éléments du Deck du Player
    file << "Deck Size:" << std::endl << player2.getDeck().getInsects()->size() << std::endl;



    // Sauvegarder de Input
    const Inputs input2 = player2.getInputs();
    file << "Start Position:" << std::endl << input2.getStart().getI() << ", " << input2.getStart().getJ() << std::endl;
    for (const auto& dest : input2.getPossibleDestinations()) {
        file << dest.getI() << "," << dest.getJ() << std::endl;
    }
    file << "startSelected:" << std::endl << input2.getStartSelected() << std::endl;
    file << "destinationSelected:" << std::endl << input2.getDestinationSelected() << std::endl;
    file << "destinationIndex:" << std::endl << input2.getDestinationIndex() << std::endl;
    file << "needPossibleDestinations:" << std::endl << input2.getNeedPossibleDestinations() << std::endl;
    file << "message:" << std::endl << input2.getMessage() << std::endl;
    file << "Fin_Joueur2:" << std::endl << std::endl;

    file << "Current_Player:"<< std::endl << std::endl;
    file << "ID:"<< std::endl << player2.getId() << std::endl;
    file << "Fin_Current_Player:"<< std::endl << std::endl;
    file << "InputManager:"<< std::endl << std::endl;
    file << "InputManager_Fin:"<< std::endl << std::endl;

    // Sauvegarder insects
    file << "Insects_Hive:" << std::endl;
    for (const auto& insect : insects) {
        file << insect->getID() << " " << insect->getIT() << " " << insect->getCoordinates().getI()
        << " "<< insect->getCoordinates().getJ()<< " "<< insect->getColor()<< std::endl;
    }
    file << "Fin_Insects_Hive:"<< std::endl;

    file << "FinFichier" << std::endl;
    file.close();
    std::cout << "Partie sauvegardée avec succès dans " << filename << std::endl;
}
//void Hive::loadGame(const std::string& filename) {}




void Hive::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    int counter = 0;
    if (!file.is_open()) {
        throw HiveException("loadGame", "Impossible d'ouvrir le fichier de sauvegarde.");
    }
    bool mode_done = false;
    bool historic_done = false;
    bool rewind_done = false;
    bool trueMapSideSize_done = false;
    bool renderedMapSideSize_done = false;
    bool Turn_done = false;
    bool insects_done = false;
    bool currentPlayer_done = false;
    bool extensions_done = false;
    bool joueur1_done = false;
    bool joueur2_done = false;
    int count = 0;
    int currentPlayeR;
    std::string line;
    // Charger les informations de base
    while (counter < 11) {
        if(line.find("FinFichier") != std::string::npos) {
            file.clear();  // Réinitialise les indicateurs (EOF)
            file.seekg(0, std::ios::beg);
        }  // Repositionne le curseur au début

        if(count++ > 50)break;
        std::getline(file, line);



        if (line.find("Mode:") != std::string::npos && !mode_done) {
            counter++;mode_done = true;
            int modeValue;
            file >> modeValue;
            mode = static_cast<Mode>(modeValue);
        }

        else if (line.find("Turn:") != std::string::npos && !Turn_done) {
            counter++;Turn_done = true ;
            int Turnvalue;
            file >> Turnvalue;
            solver.setTurn(Turnvalue) ;
        }

        else if (line.find("RewindNb:") == 0  && !rewind_done )  {  // Vérifie si la ligne commence par "RewindNb:"
            counter++;rewind_done= true;
            file >> rewindNb;  // Lire la valeur entière dans rewindNb
        }
        else if (line.find("trueMapSideSize:") != std::string::npos  && !trueMapSideSize_done) {
            counter++;trueMapSideSize_done= true;
            file >> trueMapSideSize;

        }
        else if (line.find("currentPlayer:") != std::string::npos  && !currentPlayer_done) {
            counter++;currentPlayer_done= true;
            file >> currentPlayeR;

        }
        else if (line.find("renderedMapSideSize:") != std::string::npos && !renderedMapSideSize_done) {
            counter++;renderedMapSideSize_done= true;
            file >> renderedMapSideSize;


        }

        else if (line.find("Debut_Historic:") != std::string::npos  && !historic_done ) {
            counter++;historic_done= true; //Extension probleme
            map.getHistoric().clear();
            int startI,startj,endI,endJ;
            while (std::getline(file, line) && line != "Fin_Historic:" && !line.empty()){
                //int num = std::stoi(line);
                std::istringstream iss(line);
                iss >> startI>>startj>>endI>>endJ;
                map.getHistoric().emplace_front(vec2i(startI,startj),vec2i(endI,endJ));
                //insert(Map::movement(vec2i(startI,startj),vec2i(endI,endJ)));
            }
        }

        // Charger les Extensions
        else if (line.find("Extensions:") != std::string::npos  && !extensions_done ) {
            counter++;extensions_done= true; //Extension probleme
            extensions.clear();
            int extension;
            while (std::getline(file, line) && line != "Fin_Extensions:" && !line.empty()){
                extension = std::stoi(line);
                extensions.insert(static_cast<insectType> (extension));  // Ajouter l'extension au set
            }
        }






        // Charger les informations des joueurs
        else if (line == "Joueur1:" && !joueur1_done ) {
            counter++;joueur1_done= true;
            int id = 0;
            bool isHuman = false;
            std::string name;
            std::vector<Insect*> activeInsects1;  // Liste des insectes actifs à charger

            while (std::getline(file, line) && line != "Fin_Joueur1:") {
                if (line.find("ID:") != std::string::npos) {
                    file >> id;  // Lire l'ID du joueur
                }
                else if (line.find("isHuman:") != std::string::npos) {
                    file >> isHuman;  // Lire si le joueur est humain
                }
                else if (line.find("Player Name:") != std::string::npos) {
                    file >> name;  // Lire le nom du joueur
                }
                else if (line.find("Deck Size:") != std::string::npos) {
                    int deckSize;
                    file >> deckSize;
                }
            }

            player1 = Player(id, isHuman, name, Deck(), activeInsects1);
            //renderer->setPlayer1(&player1);

        }
        // Charger les informations de Joueur2 de manière similaire à Joueur1
        // Charger les informations des joueurs
        else if (line == "Joueur2:"  && !joueur2_done) {
            counter++;joueur2_done= true;
            int id = 0;
            bool isHuman = false;
            std::string name;
            std::vector<Insect*> activeInsects2;  // Liste des insectes actifs à charger

            while (std::getline(file, line) && line != "Fin_Joueur2:") {
                if (line.find("ID:") != std::string::npos) {
                    file >> id;  // Lire l'ID du joueur
                }
                else if (line.find("isHuman:") != std::string::npos) {
                    file >> isHuman;  // Lire si le joueur est humain
                }
                else if (line.find("Player Name:") != std::string::npos) {
                    file >> name;  // Lire le nom du joueur
                }
                else if (line.find("Deck Size:") != std::string::npos) {
                    int deckSize;
                    file >> deckSize;
                }
            }
            player2 = Player(id, isHuman, name, Deck(), activeInsects2);
            //renderer->setPlayer2(&player2);

        }
        else if (line.find("Insects_Hive:") != std::string::npos  && !insects_done && joueur1_done && joueur2_done) {
            for(int i = 0; i < trueMapSideSize * trueMapSideSize; i++){
                if(map.getSlot(i) != nullptr)
                map.setSlot(i, nullptr);

            }
            for(auto& insect : insects)
            {delete insect;insect= nullptr;}
            insects.clear();
            Insect::setCounter(0);
            //generateAllInsects();
            counter++;insects_done= true;
            while (std::getline(file, line) && line != "Fin_Insects_Hive:") {

                int id, it, i, j, color;

                std::istringstream stream(line);

                stream >> id >> it >> i >> j >> color;
                // Création des insectes
                generateSingleInsect(it,color, {i,j});
            }
        }

    }
    currentPlayer =  (currentPlayeR == 2) ? &player2 : &player1;

    file.close();
    std::cout << "Partie chargée avec succès depuis " << filename << std::endl;
}


Insect* Hive::generateSingleInsect(int type, bool color, vec2i vec) {
    Insect* insect = nullptr;
    // Création de l'insecte en fonction du type
    switch (type) {

        case 0: insect = new Ant(color);break;
        case 1: insect = new Beetle(color);break;
        case 2: insect = new Grasshopper(color);break;
        case 3: insect = new Bee(color);break;
        case 4: insect = new Spider(color);break;
        case 5: insect = new Mosquitoe(color);break;
        case 6: insect = new Ladybug(color);break;
        default:
            throw HiveException("Hive::generateSingleInsect", "Insecte de type inconnu");
    }
    insect->setCoordinates(vec);
    // Définition des coordonnées et ajout dans le deck
    if (color) {
        if(vec.getI() == -1)player1.deck.addInsect(insect);
        else {
            map.putInsectTo(insect,vec);
            player1.addActiveInsect(insect);
        }
    } else {
        if(vec.getI() == 30)player2.deck.addInsect(insect);
        else {
            map.putInsectTo(insect,vec);
            player2.addActiveInsect(insect);
        }
    }

    // Ajout à la liste globale
    insects.push_back(insect);

    return insect;
}









