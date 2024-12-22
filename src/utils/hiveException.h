//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_EXEPTION_H
#define HIVE_EXEPTION_H

#include <iostream>
#include <string>

class HiveException : std::exception{
private:
    std::string location;
    std::string content;

public:
    explicit HiveException(std::string location_, std::string content_) : location(location_), content(content_) {}

    const std::string & getLocation() const { return location; }
    const std::string & getContent() const { return content; }
    std::string getInfos() const { return location + ":" + content; }
};

#endif //HIVE_EXEPTION_H
