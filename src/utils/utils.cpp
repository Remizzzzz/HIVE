//
// Created by loic_ on 14/10/2024.
//

#include "utils.h"


vec2i operator+(vec2i const & v1_, vec2i const & v2_){
    return {v1_.getI() + v2_.getI(), v1_.getJ() + v2_.getJ()};
}

vec2i operator-(vec2i const & v1_, vec2i const & v2_){
    return {v1_.getI() - v2_.getI(), v1_.getJ() - v2_.getJ()};
}

vec2i operator*(int const & k, vec2i const & v_){
    return {k * v_.getI(),v_.getJ()};
}

std::ostream & operator<<(std::ostream & f_, vec2i const & v_){
    f_ << '(' << v_.getI() << ',' << v_.getJ() << ')';
    return f_;
}