//
// Created by joanc on 22/12/2024.
//

#include <iostream>

#include "features/map.h"
#include "features/inputs.h"
#include "features/player.h"
#include "renderer.h"


void ConsoleRenderer::renderDeck1(const Player & currentPlayer_) const {
    std::cout << "\033[31m";
    if (currentPlayer_.getInputs().getStart().getI() == -1) {
        for (const auto & insect: P1->getDeck()) {
            std::cout << insect->getCoordinates();
            if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getStart().getJ()){
                if (currentPlayer_.getInputs().isStartSelected()) {
                    std::cout << "\033[92m" << insect->getPV() << "\033[31m ";
                }
                else {
                    std::cout << "\033[35m" << insect->getPV() << "\033[31m ";
                }
            } else if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getDestination().getJ()){
                if (!currentPlayer_.getInputs().isDestinationSelected()) {
                    std::cout << "\033[35m" << insect->getPV() << "\033[31m ";
                }
            } else std::cout << insect->getPV() << " ";
        }
    } else {
        for (const auto & insect: P1->getDeck()) std::cout << insect->getPV() <<" ";
    }
    std::cout << '\n';
}


void ConsoleRenderer::renderDeck2(const Player & currentPlayer_) const {
    std::cout << "\033[34m";
    if (currentPlayer_.getInputs().getStart().getI() == renderedSideSize) {
        for (const auto & insect: P2->getDeck()) {
            if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getStart().getJ()) {
                if (currentPlayer_.getInputs().isStartSelected()) {
                    std::cout << "\033[92m" << insect->getPV() << "\033[34m ";
                } else {
                    std::cout << "\033[35m" << insect->getPV() << "\033[34m ";
                }
            } else if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getDestination().getJ()) {
                if (!currentPlayer_.getInputs().isDestinationSelected()) {
                    std::cout << "\033[35m" << insect->getPV() << "\033[34m ";
                }
            } else std::cout << insect->getPV() <<" ";
        }
    } else {
        for (const auto & insect: P2->getDeck()) std::cout << insect->getPV() <<" ";
    }
    std::cout << "\n\033[37m";
}


void ConsoleRenderer::renderMap(const Player & currentPlayer_) const{
    vec2i start = currentPlayer_.getInputs().getStart() + vec2i{offset,offset};

    std::cout << "\033[37m";
    for (int i = offset; i < offset + renderedSideSize ; ++i) {
        //std::cout << '[' << i << ']';
        if (i%2 == 0) std::cout << "\033[37m-";
        else std::cout << "\033[37m  -";

        for (int j = offset; j < offset + renderedSideSize; ++j) {
            const vec2i insectPos{i,j};
            const Insect * insect = map.getInsectAt(insectPos);

            if (insect) {
                if (start == insectPos) {
                    if (currentPlayer_.getInputs().isStartSelected()) {
                        std::cout << "\033[92m-" << insect->getPV() << "-\033[37m";
                    } else {
                        std::cout << "\033[35m-" << insect->getPV() << "-\033[37m";
                    }
                } else if (currentPlayer_.getInputs().getDestination() == insectPos) {
                    if (!currentPlayer_.getInputs().isDestinationSelected()) {
                        std::cout << "\033[35m-" << insect->getPV() << "-\033[37m";
                    }
                } else if (isVec2iInVector(currentPlayer_.getInputs().getPossibleDestinations(), insectPos)) {
                    std::cout << "\033[33m-" << insect->getPV() << "-\033[37m";
                } else {
                    if (insect->getColor()) {
                        std::cout << "\033[31m-" << insect->getPV() << "-\033[37m";
                    } else {
                        std::cout << "\033[34m-" << insect->getPV() << "-\033[37m";
                    }
                }
            } else {
                if (start == insectPos) {
                    if (currentPlayer_.getInputs().isStartSelected()) {
                        std::cout << "\033[92m-" << "  " << "-\033[37m";
                    } else {
                        std::cout << "\033[35m-" << "  " << "-\033[37m";
                    }
                } else if (currentPlayer_.getInputs().getDestination() == insectPos) {
                    if (!currentPlayer_.getInputs().isDestinationSelected()) {
                        std::cout << "\033[35m-" << "  " << "-\033[37m";
                    }
                } else if (isVec2iInVector(currentPlayer_.getInputs().getPossibleDestinations(), insectPos)) {
                    std::cout << "\033[33m-" << "  " << "-\033[37m";
                } else {
                    std::cout << "-  -";
                }
            }
        }

        std::cout << "-\n";
    }
}

