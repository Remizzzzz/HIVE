//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_INSECT_H
#define HIVE_INSECT_H

#include <sstream>
#include <utility>
#include <vector>

#include "../utils/utils.h"
class Map;
enum insectType{bee,ant,grasshopper,spider,beetle, mosquitoe};
class Insect{
    int id;
    insectType iT;
    vec2i coordinates;
    bool color; // 0 = bleu, 1 = rouge
    //bool relie_ruche   sûr qu'on en ai besoin?
    static int counter; // pour définir les identifiants à init dans le .cpp
    //static int const max_instance = MI; // A voir si utile


public:
    Insect( bool col, insectType type) : id(counter++), iT(type), color(col),coordinates({-1,-1}) {};//Ici -1 c'est pour NULL, mais la valeur doit être int

    int  getID() const { return id; }
    int  getIT() const { return iT; }
    bool  getColor() const { return color; }
    vec2i getCoordinates() const { return coordinates; }
    void setCoordinates(const vec2i & coordinates_){coordinates = coordinates_;}
    //static int  get_counter() { return counter; }// A voir si utile
    //static int  get_max_instance() { return max_instance; }// A voir si utile


    [[nodiscard]] virtual std::string getPrintableValue() const
    {
        std::string color;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        return color;
    }
    int getFormerNeighbour(vec2i newPosition, Map &m) const; //Fonctions pour detecter les anciens voisins à la nouvelle position
    virtual std::vector<vec2i> getPossibleMovements(Map &m) const = 0;
    std::vector<vec2i> setRule(Map &m) const;
    bool isLinkingHive(Map &m) const;
};


class Bee : public virtual Insect {
public://test
    Bee(bool col) : Insect(col, bee) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    bool isCircled(Map &m);
    [[nodiscard]] std::string getPrintableValue() const override
    {
        std::string color;
        std::stringstream s;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        s << "\033[33mQ" << color << "B" << "\033[0m"; // Q affiché en jaune, B avec la couleur du joueur (pour Queen Bee)
        return s.str();
    }
};


class Beetle : public virtual Insect {

    Insect* isAboveOf;

public:
    Beetle(bool col) : Insect(col, beetle) {}
    Insect* getInsectUnder() {
        return isAboveOf;
    }
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue() const override
    {
        std::string color;
        std::stringstream s;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        s << color<< "B" << getID() << "\033[0m";
        return s.str();
    }
};


class Grasshopper : public virtual Insect {
public:
    Grasshopper(bool col) : Insect(col, grasshopper) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue() const override
    {
        std::string color;
        std::stringstream s;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        s << color<< "G" << getID() << "\033[0m";
        return s.str();
    }
};


class Spider : public virtual Insect {
public:
    Spider(bool col) : Insect(col, spider) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue() const override
    {
        std::string color;
        std::stringstream s;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        s << color<< "S" << getID() << "\033[0m";
        return s.str();
    }
};


class Ant : public virtual Insect {
public:
    Ant(bool col) : Insect(col, ant) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue() const override
    {
        std::string color;
        std::stringstream s;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        s << color<< "A" << getID() << "\033[0m";
        return s.str();
    }
};


class Mosquitoe : public virtual Insect, public virtual Ant, public virtual Bee, public virtual Grasshopper,
public virtual Beetle, public virtual Spider  {
public:
    Mosquitoe(bool col)
        : Insect(col, mosquitoe), Ant(col), Bee(col), Grasshopper(col), Beetle(col), Spider(col) {}

    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue() const override
    {
        std::string color;
        std::stringstream s;
        if(getColor() == true ) {
            color = "\033[34m";
        }else {
            color = "\033[31m";
        }
        s << color<< "M" << getID() << "\033[0m";
        return s.str();
    }
};




#endif //HIVE_INSECT_H
