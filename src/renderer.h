#ifndef HIVE_RENDERER_H
#define HIVE_RENDERER_H

#include <iostream>
#include <QDebug>

#include "features/map.h"
#include "features/player.h"



// Dans tout le renderer, idColor = 0, affiche sans couleur spécifique, idColor = 1, cursor Start avant selec
// idColor = 2, cursor start apres select, idColor = 1, cursor parmi les possibles, idColor = 3, movement possibles

//Classe pour afficher la carte d'une partie de Hive.
class Renderer {
public:
    explicit Renderer(Map &map_,Player* P1_, Player* P2_, int rendered_s_i, const int offset_) :
        map(map_), P1(P1_), P2(P2_), renderedSideSize(rendered_s_i), offset(offset_) {} // à voir comment initialiser l'output
    virtual ~Renderer() = default; // Rendre Renderer polymorphe avec un destructeur virtuel
    virtual void render(const Player & currentPlayer_) const = 0;

protected:
    Player *P1;
    Player *P2;
    int renderedSideSize;
    const int offset;
    Map &map;

public:
    void setPlayer1(Player *player) { P1 = player; }
    void setPlayer2(Player *player) { P2 = player; }
    void setMap(Map map1) { map = map1; }
};


class ConsoleRenderer : public Renderer {
public:
    explicit ConsoleRenderer(Map &map_,Player* P1_, Player* P2_, int rendered_s_i, const int offset_) :
    Renderer(map_,P1_,P2_, rendered_s_i, offset_) {}

    void renderDeck1(const Player & currentPlayer_) const;
    void renderDeck2(const Player & currentPlayer_) const;
    void renderMap(const Player & currentPlayer_) const;

    static bool isVec2iInVector(const std::vector<vec2i> & vector_, const vec2i & vec2i_) {
        for (const auto & elt : vector_) if (elt == vec2i_) return true;
        return false;
    }

    void render(const Player & currentPlayer_) const override {
        //system("cls");
        std::cout << '\n';
        renderDeck1(currentPlayer_);
        renderMap(currentPlayer_);
        renderDeck2(currentPlayer_);
    }

};

#endif // HIVE_RENDERER_H
