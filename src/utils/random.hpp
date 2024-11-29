//
// Created by loic_ on 05/08/2024.
//

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "iostream"
#include "random"
#include "cassert"

namespace lv{

    class Random{

    public:

        Random(){
            gen.seed(rd());
            dis = std::uniform_real_distribution<>(0.f,1.f);
        };

        /// <summary>
        /// Give a random int in the range [a_,b_[
        /// </summary>
        int getRandomInt(int a_, int b_){
            assert(a_ <= b_ && "lv : random : getRandomInt : a_ is greater than b_");
            return int(dis(gen) * (b_ - a_) + a_);
        }

        /// <summary>
        /// Give a random float in the range [a_,b_[
        /// </summary>
        float getRandomFloat(float a_, float b_){
            assert(a_ <= b_ && "lv : random : getRandomFloat : a_ is greater than b_");
            return float(dis(gen) * (b_ - a_) + a_);
        }

        /// <summary>
        /// Give a random double in the range [a_,b_[
        /// </summary>
        double getRandomDouble(double a_, double b_){
            assert(a_ <= b_ && "lv : random : getRandomDouble : a_ is greater than b_");
            return (dis(gen) * (b_ - a_) + a_);
        }

    private:
        std::random_device rd{};
        std::mt19937 gen;
        std::uniform_real_distribution<> dis{};
    };

}

#endif //RANDOM_HPP
