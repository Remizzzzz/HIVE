//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_INSECT_H
#define HIVE_INSECT_H

#include <sstream>
#include <utility>
#include <vector>

#include "../utils/utils.h"
#include "src/utils/hiveException.h"
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


public:
    Insect( bool col, insectType type) : id(counter++), iT(type), color(col),coordinates({-1,-1}) {};//Ici -1 c'est pour NULL, mais la valeur doit être int

    int  getID() const { return id; }
    int  getIT() const { return iT; }
    bool  getColor() const { return color; }
    vec2i getCoordinates() const { return coordinates; }
    void setCoordinates(const vec2i & coordinates_){coordinates = coordinates_;}
    //static int  get_counter() { return counter; }// A voir si utile
    //static int  get_max_instance() { return max_instance; }// A voir si utile


    [[nodiscard]] virtual std::string getPrintableValue(int idColor) const
    {
        if(idColor == 0)
        {
            std::string color;
            if(getColor() == true ) {
                color = "\033[34m";
            }else {
                color = "\033[31m";
            }
            return color;
        }
        else
        {
            return "\033[92m";
        }

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
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
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
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "QB" << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "QB" << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "QB" << "\033[0m";
            return s.str();
        }
        throw HiveException("Bee: Unknown color", "Insect.h : Insect::Bee::getPrintableValue");
    }
};


class Beetle : public virtual Insect {

    // Pointe vers l'insecte sur lequel le scarabée est placé
    // nullptr par défaut, car le scarabée ne peut pas être placé au-dessus d'une pièce
    const Insect* isAboveOf = nullptr;

public:
    Beetle(const bool col) : Insect(col, beetle) {}
    const Insect* getInsectUnder() const {
        return isAboveOf;
    }
    void setAboveOf(const Insect *insect) {
        isAboveOf = insect;
    }

    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
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
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "B" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "B" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "B" << getID() << "\033[0m";
            return s.str();
        }
        throw HiveException("Beettle: Unknown color", "Insect.h : Insect::Beettle::getPrintableValue");
    }
};


class Grasshopper : public virtual Insect {
public:
    Grasshopper(bool col) : Insect(col, grasshopper) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
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
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "G" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "G" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "G" << getID() << "\033[0m";
            return s.str();
        }
        throw HiveException("Grasshopper: Unknown color", "Insect.h : Insect::Grasshopper::getPrintableValue");
    }
};


class Spider : public virtual Insect {
public:
    Spider(bool col) : Insect(col, spider) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
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
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "S" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "S" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "S" << getID() << "\033[0m";
            return s.str();
        }
        throw HiveException("Spider: Unknown color", "Insect.h : Insect::Spider::getPrintableValue");
    }
};


class Ant : public virtual Insect {
public:
    Ant(bool col) : Insect(col, ant) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
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
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "A" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "A" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "A" << getID() << "\033[0m";
            return s.str();
        }
        throw HiveException("Ant: Unknown color", "Insect.h : Insect::Ant::getPrintableValue");
    }
};


class Mosquitoe : public virtual Insect, public virtual Ant, public virtual Bee, public virtual Grasshopper,
public virtual Beetle, public virtual Spider  {
public:
    Mosquitoe(bool col)
        : Insect(col, mosquitoe), Ant(col), Bee(col), Grasshopper(col), Beetle(col), Spider(col) {}

    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
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
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "M" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "M" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "M" << getID() << "\033[0m";
            return s.str();
        }
        throw HiveException("Mosquitoe: Unknown color", "Insect.h : Insect::Mosquitoe::getPrintableValue");

    }
};

class Ladybug : public virtual Insect {
public:
    Ladybug(bool col) : Insect(col, ladybug) {}
    std::vector<vec2i> getPossibleMovements(Map &m) const override;
    [[nodiscard]] std::string getPrintableValue(int idColor) const override
    {
        if(idColor == 0)
        {
            std::string color;
            std::stringstream s;
            if(getColor() == true ) {
                color = "\033[34m";
            }else {
                color = "\033[31m";
            }
            s << color<< "L" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 1)
        {
            std::string color="\033[92m";
            std::stringstream s;
            s << color << "L" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 2)
        {
            std::string color="\033[32m";
            std::stringstream s;
            s << color << "L" << getID() << "\033[0m";
            return s.str();
        }
        else if(idColor == 3)
        {
            std::string color="\033[35m";
            std::stringstream s;
            s << color << "L" << getID() << "\033[0m";
            return s.str();
        }
        throw HiveException("Ladybug: Unknown color", "Insect.h : Insect::Ladybug::getPrintableValue");
    }
};


#endif //HIVE_INSECT_H
