//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_INSECT_H
#define HIVE_INSECT_H

#include <sstream>
#include <utility>
#include <vector>

#include "../utils/utils.h"
#include "../utils/hiveException.h"
class Map;
enum insectType{ant,beetle, grasshopper,bee,spider, mosquitoe, ladybug,none};
class Insect{
    int id;
    insectType iT;
    vec2i coordinates;
    bool color; // 0 = bleu, 1 = rouge
    //bool relie_ruche   sûr qu'on en ai besoin?
    static int counter; // pour définir les identifiants à init dans le .cpp
    //static int const max_instance = MI; // A voir si utile


    //static int const max_instance = MI; // A voir si utile
    std::string PV;


public:
    Insect( bool col, insectType type, std::string PV_) : id(counter++), iT(type), color(col),coordinates({-1,-1}), PV(PV_) {};//Ici -1 c'est pour NULL, mais la valeur doit être int

    virtual ~Insect() = default;

    int  getID() const { return id; }
    insectType  getIT() const { return iT; }
    bool  getColor() const { return color; }
    vec2i getCoordinates() const { return coordinates; }
    void setCoordinates(const vec2i & coordinates_){coordinates = coordinates_;}
    static int  get_counter() { return counter; }// A voir si utile
    //static int  get_max_instance() { return max_instance; }// A voir si utile
    // Setter pour 'id'
    void setId(int newId) {
        id = newId;
    }
    void setColor(bool newcolor) {
        id = newcolor;
    }
    static void setCounter(int compt) {
        counter = compt;
    }

    // Setter pour 'iT'
    void setType(insectType newType) {
        iT = newType;
    }


    const std::string & getPV() const {
        return PV;
    }

    int getFormerNeighbour(vec2i oldPosition, vec2i newPosition, Map &m) const; //Fonctions pour detecter les anciens voisins à la nouvelle position
    virtual std::vector<vec2i> getPossibleMovements(Map &m) const = 0;
    std::vector<vec2i> setRule(Map &m, bool color) const;
    bool isLinkingHive(Map &m) const;
    void setAboveOf(Insect * insect){};
    Insect* getInsectUnder() const {
        return nullptr;
    }
};


class Bee : public virtual Insect {
public://test
    Bee(bool col) : Insect(col, bee, "Qe") {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    bool isCircled(Map &m) const;
};


class Beetle : public virtual Insect {

    // Pointe vers l'insecte sur lequel le scarabée est placé
    // nullptr par défaut, car le scarabée ne peut pas être placé au-dessus d'une pièce
    Insect* isAboveOf = nullptr;

public:
    Beetle(const bool col) : Insect(col, beetle,"Be") {}
    Insect* getInsectUnder() const {
        return isAboveOf;
    }
    void setAboveOf(Insect *insect) {
        isAboveOf = insect;
    }

    std::vector<vec2i> getPossibleMovements(Map &m) const override;
};


class Grasshopper : public virtual Insect {
public:

    Grasshopper(bool col) : Insect(col, grasshopper,"Gr") {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
};


class Spider : public virtual Insect {
public:
    Spider(bool col) : Insect(col, spider,"Sp") {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
};


class Ant : public virtual Insect {
public:
    Ant(bool col) : Insect(col, ant,"An") {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
};

class Ladybug : public virtual Insect {
public:
    Ladybug(bool col) : Insect(col, ladybug,"La") {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
};

class Mosquitoe : public virtual Insect, public virtual Ant, public virtual Bee, public virtual Grasshopper,
public virtual Beetle, public virtual Spider, public virtual Ladybug  {
public:
    Mosquitoe(bool col)
        : Insect(col, mosquitoe,"Mo"), Ant(col), Bee(col), Grasshopper(col), Beetle(col), Spider(col), Ladybug(col) {}

    std::vector<vec2i> getPossibleMovements(Map &m) const override;
};


#endif //HIVE_INSECT_H
