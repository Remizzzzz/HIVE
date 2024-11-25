//
// Created by loic_ on 25/11/2024.
//

#ifndef HIVE_RENDERER_H
#define HIVE_RENDERER_H

#include "features/map.h"
#include "features/inputs.h"

enum Version{console, graphic};

class Renderer{
private:
    Map & map;
public:
    explicit Renderer(Map & map_) : map(map_) {};

    virtual void render(const Inputs & inputs_) = 0;
};

class ConsoleRenderer : Renderer{

public:

    explicit ConsoleRenderer(Map & map_) : Renderer(map_){};

    void render(const Inputs & inputs_) override{

    }
};

class GraphicRenderer : Renderer{

public:

    explicit GraphicRenderer(Map & map_) : Renderer(map_){};

    void render(const Inputs & inputs_) override{

    }
};



#endif //HIVE_RENDERER_H
