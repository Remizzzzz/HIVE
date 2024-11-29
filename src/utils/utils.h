//
// Created by loic_ on 14/10/2024.
//

#ifndef HIVE_UTILS_H
#define HIVE_UTILS_H

#include <iostream>

class vec2i{
private:
    int i;
    int j;

public:
    /** Constructeurs */
    vec2i(int i_, int j_) : i(i_), j(j_){}
    vec2i() : i(0), j(0){}

    /** Constructeur de copie : par défaut */
    vec2i(vec2i const & v_) = default;

    /** Opérateur d'affectation : par défaut */
    vec2i & operator=(vec2i const & v_) = default;

    vec2i & operator+=(vec2i const & v_){
        i+= v_.i;
        j+= v_.j;
        return *this;
    };

    /** Comparaison entre vecteurs */
    bool operator==(vec2i const & v_) const {
        return i == v_.i && j == v_.j;
    };
    bool operator!=(vec2i const & v_) const {
        return i != v_.i || j != v_.j;
    }
    bool operator<(const vec2i& other) const {
        // Comparaison lexicographique : d'abord par x, puis par y
        if (i != other.i) {
            return i < other.i;
        }
        return j < other.j;
    }

    /** @brief Renvoi la composante i de vec2i.*/
    [[nodiscard]] const int & getI() const{return i;}
    /** @brief Renvoi la composante j de vec2i.*/
    [[nodiscard]] const int & getJ() const{return j;}

    /** @brief Affecte i_ à la composante i de vec2i.*/
    void setI(const int & i_) {i = i_;}
    /** @brief Affecte j_ à la composante j de vec2i.*/
    void setJ(const int & j_) {j = j_;}

    /** @brief Renvoi vrai sur si le vecteur en (0,0) et faut sinon.*/
    [[nodiscard]] bool isNull() const {
        return i == 0 && j == 0;
    }

};

/** Opérations mathématiqes de base */
vec2i operator+(vec2i const & v1_, vec2i const & v2_);
vec2i operator-(vec2i const & v1_, vec2i const & v2_);
vec2i operator*(int const & k, vec2i const & v_);
std::ostream & operator<<(std::ostream & f_, vec2i const & v_);

#endif //HIVE_UTILS_H
