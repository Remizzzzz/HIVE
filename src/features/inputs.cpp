//
// Created by loic_ on 29/11/2024.
//

#include "inputs.h"


std::ostream & operator<<(std::ostream & f_, Inputs & inputs_){
    f_ << inputs_.cursor1 << ' ' << inputs_.cursor1selected << '\n' << inputs_.cursor2 << ' ' << inputs_.cursor2selected << '\n' << inputs_.message << '\n';
    return f_;
}