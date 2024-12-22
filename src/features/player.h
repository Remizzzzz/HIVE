//
// Created by loic_ on 30/11/2024.
//

#ifndef HIVE_PLAYER_H
#define HIVE_PLAYER_H

#include "deck.h"
#include "inputs.h"

#include <algorithm>

class Hive;
class InputsManager;
class Solver;

class Player {
private:
    int id;
    bool isHuman;
    std::string name;
    Deck deck;
    std::vector<Insect *> activeInsects;
    Inputs inputs;

    friend class Hive;
    friend class InputsManager;
    friend class Solver;
public:
    explicit Player(const int & id_) : id(id_), isHuman(true), deck(), activeInsects(), inputs() {}
    explicit Player(const int & id_, const std::string &newname) : id(id_), isHuman(true), name(newname), deck(), activeInsects(), inputs() {}

    Player(int id, bool isHuman, const std::string& name, const Deck& deck, const std::vector<Insect*>& activeInsects)
        : id(id), isHuman(isHuman), name(name), deck(deck), activeInsects(activeInsects) {}

    const int & getId() const { return id; }
    const Inputs & getInputs() const { return inputs; }7
    Deck& getDeck () { return deck; }
    const std::string getName() const { return name; }
    const std::vector<Insect *> & getActiveInsects() const { return activeInsects; }

    void setName(const std::string& newName) { name = newName; }
    void setHumanity(const bool & isHuman_) { isHuman = isHuman_; }

    bool addActiveInsect(Insect* insect) {
        if (insect == nullptr) {
            throw  HiveException("Player::addActiveInsect" ,"Erreur : l'insecte est nul.\n");
        }

        // Vérifie si l'insecte est déjà actif
        if (std::find(activeInsects.begin(), activeInsects.end(), insect) != activeInsects.end()) {
            std::cerr << "Erreur : l'insecte est déjà dans la liste des insectes actifs.\n";
            return false; // Échec de l'ajout
        }

        // Ajoute l'insecte
        activeInsects.push_back(insect);
        return true; // Succès de l'ajout
    }

    void removeActiveInsect(Insect* i) {
        int id=0;
        for (auto it : activeInsects) {
            if (it == i) activeInsects.erase(activeInsects.begin()+id);
            id++;
        }
    }

    void addActiveInsectsFromDeck(const int & deckIndex_) {
        Insect * insect = deck.insects.at(deckIndex_);
        activeInsects.push_back(insect);
    }

    bool lostGame(Map &m) {
        for (auto it : activeInsects) {
            if (it->getIT()==bee) {
                Bee b=dynamic_cast<Bee*>(it);
                b.setCoordinates(it->getCoordinates());
                return b.isCircled(m);
            }
        }
        return false;
    }
};

#endif //HIVE_PLAYER_H
