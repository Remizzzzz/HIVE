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
    vec2i(int i_, int j_) : i(i_), j(j_) {}
    vec2i() : i(0), j(0) {}
    vec2i(vec2i const & v_) = default;

    vec2i & operator=(vec2i const & v_) = default;
    vec2i & operator+=(vec2i const & v_) { i += v_.i; j += v_.j; return *this; }
    bool operator!=(vec2i const & v_) const { return i != v_.i || j != v_.j; }
    bool operator<(const vec2i& other) const {
        // Comparaison lexicographique : d'abord par x, puis par y
        if (i != other.i) return i < other.i;
        return j < other.j;
    }

    [[nodiscard]] const int & getI() const { return i; }
    [[nodiscard]] const int & getJ() const { return j; }

    void setI(const int & i_) { i = i_; }
    void setJ(const int & j_) { j = j_; }
    [[nodiscard]] bool isNull() const { return i == 0 && j == 0; }
};

vec2i operator+(vec2i const & v1_, vec2i const & v2_);
vec2i operator-(vec2i const & v1_, vec2i const & v2_);
vec2i operator*(int const & k, vec2i const & v_);
bool operator==(vec2i const & v1_, vec2i const & v2_);
std::ostream & operator<<(std::ostream & f_, vec2i const & v_);

class Move {
private:
    vec2i startPlacement; // Position de départ
    vec2i endPlacement;   // Position de fin

public:
    Move() : startPlacement(-1, -1), endPlacement(-1, -1) {}
    Move(const vec2i& start, const vec2i& end) : startPlacement(start), endPlacement(end) {}
    Move( const vec2i& place, bool choice) {
        if(choice) {
            startPlacement = vec2i(-1,-1);
            endPlacement = (place);
        } else {
            startPlacement = vec2i(place);
            endPlacement = vec2i(-1,-1);
        }
    }
    Move(const vec2i& place) = delete;

    vec2i getStartPlacement() const { return startPlacement; }
    vec2i getEndPlacement() const { return endPlacement; }

    void setStartPlacement(const vec2i& start) { startPlacement = start; }
    void setEndPlacement(const vec2i& end) { endPlacement = end; }
};


#endif //HIVE_UTILS_H
