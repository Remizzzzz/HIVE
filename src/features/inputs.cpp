//
// Created by loic_ on 29/11/2024.
//

#include "inputs.h"


std::ostream & operator<<(std::ostream & f_, const Inputs & inputs_){
    f_ << inputs_.getStart()<< ' ' << inputs_.isStartSelected() << '\n' << inputs_.getDestination() << ' ' << inputs_.isDestinationSelected() << '\n' << inputs_.getMessage() << '\n';
    return f_;
}