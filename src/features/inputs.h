//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_INPUTS_H
#define HIVE_INPUTS_H

#include <string>
#include <vector>

#include "../utils/utils.h"
#include "../utils/hiveException.h"

class Inputs{
private:
    //le joueur a validé son action
    bool startSelected{};
    //Position apres la premiere validation
    vec2i start;

    //le joueur a validé son action
    bool destinationSelected{};
    //Position apres la deuxieme validation
    int destinationIndex{};
    bool needPossibleDestinations{false};
    std::vector<vec2i> possibleDestinations;

    std::string message;

public:

    Inputs() = default;

    bool movementNeeded() const{
        return startSelected && destinationSelected;
    }

    const vec2i & getStart() const {
        return start;
    }

    const int & getDestinationIndex() const{
        return destinationIndex;
    }

    const vec2i & getDestination() const{
        if(destinationIndex >= possibleDestinations.size()) throw HiveException("A remplir", "");
        return possibleDestinations[destinationIndex];
    }

    const std::vector<vec2i> & getPossibleDestinations() const
    {
        return possibleDestinations;
    }

    const int getPossibleDestinationsNumber() const{
        return int (possibleDestinations.size());
    }

    void setStart(vec2i start_){
        start = start_;
    }

    void setDestinationIndex(int destinationIndex_){
        destinationIndex = destinationIndex_;
    }

    void setPossibleDestinations(const std::vector<vec2i> & possibleDestinations_){
        possibleDestinations = possibleDestinations_;
    }

    bool isPossibleDestinationsEmpty() const{
        return possibleDestinations.empty();
    }

    bool isStartSelected() const{
        return startSelected;
    }

    bool isDestinationSelected() const{
        return destinationSelected;
    }

    void selectStart(){
        startSelected = true;
    }

    void selectDestination(){
        destinationSelected = true;
    }

    void needPossibleDestinationsUpdate(){
        needPossibleDestinations = true;
    }

    void noNeedForPossibleDestinationsUpdate(){
        needPossibleDestinations = false;
    }

    bool isPossibleDestinationsNeeded() const{
        return needPossibleDestinations;
    }

    void setMessage(const std::string & message_){
        message = message_;
    }

    const std::string & getMessage() const{
        return message;
    }

    void reset(){
        startSelected = false;
        destinationSelected = false;
        needPossibleDestinations = false;
        start = {0,0};
        possibleDestinations.clear();
        destinationIndex = 0;
    }
    void resetQt() {
        vec2i reset(-1,-1);
        start=reset;
        while (!possibleDestinations.empty()) {
            possibleDestinations.clear();
        }
    }

};

std::ostream & operator<<(std::ostream & f_, const Inputs & inputs_);

#endif //HIVE_INPUTS_H
