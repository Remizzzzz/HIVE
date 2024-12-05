#ifndef HIVE_RENDERER_H
#define HIVE_RENDERER_H

#include <iostream>
#include "features/map.h"
#include "src/features/inputs.h"
#include "src/features/player.h"
//test
/**
 * @brief Classe pour afficher la carte d'une partie de Hive.
 */
class Renderer {
public:
    Inputs *inputs;
    Player *P1;
    Player *P2;
    /**
     * @brief Constructeur de la classe Renderer.
     * @param map_ Référence constante à la carte à afficher.
     */
    explicit Renderer(const Map &map_, Inputs* inputs_, Player* P1_, Player* P2_) : map(map_), inputs(inputs_), P1(P1_), P2(P2_) {} // A voir comment initialiser l'output

    /**
     * @brief Affiche la carte sur la sortie standard.
     */
    virtual ~Renderer() = default; // Rendre Renderer polymorphe avec un destructeur virtuel

    void displayDeck(Player* P) const
    {
        for(int index=0; index< P->getDeck().getInsectNb();index++)
        {
            // si cursor en ligne -1 ou ligne sideSize (et donc dans les decks
            if((inputs->getStart().getI()==-1 || inputs->getStart().getI()==map.getSideSize()) && inputs->getStart().getJ()==index)
            {
                std::cout << " " << getSlotContent(P->getDeck().getInsectAt(index),1) << "  ";
            }
            else
            {
                std::cout << " " << getSlotContent(P->getDeck().getInsectAt(index),0) << "  ";
            }
        }
    }

    void displayMap() const {
        displayDeck(P1);
        std::cout << std::endl << std::endl;
        size_t sideSize = map.getSideSize();

        for (size_t row = 0; row < sideSize; ++row) {
            // Affiche les espaces d'indentation pour simuler les décalages
            displayIndentation(row);

            // Affiche la ligne actuelle
            displayRow(row, inputs->getStart());         // A finir

            // Affiche un saut de ligne pour séparer les lignes
            std::cout << std::endl;
        }
        std::cout << std::endl << std::endl;
        displayDeck(P2);
    }

private:
    const Map &map;

    /**
     * @brief Affiche les indentations pour simuler les décalages de la grille hexagonale.
     * @param rowIndex L'indice de la ligne à afficher.
     */
    void displayIndentation(size_t rowIndex) const {
        // Lignes impaires (indexées 1, 3, 5...) doivent être décalées vers la droite
        if (rowIndex % 2 == 1) {
            std::cout << "  ";  // Ajoute des espaces pour décaler
        }
    }

    /**
     * @brief Affiche une ligne de cellules avec des insectes ou des cases vides.
     * @param rowIndex L'indice de la ligne à afficher.
     */
    void displayRow(size_t rowIndex, vec2i start) const {
        size_t sideSize = map.getSideSize();

        for (size_t col = 0; col < sideSize; ++col) {
            vec2i pos(static_cast<int>(rowIndex), static_cast<int>(col));
            if(pos==start)
            {
                const Insect *slot = map.getInsectAt(pos);
                if (getSlotContent(slot, 0)==".") {
                    std::cout << " " << getSlotContent(slot,1) << "  ";  // Trois espaces pour espacer les cases
                } else {
                    std::cout << "" << getSlotContent(slot,1) << "  ";// Seulement deux espaces
                }
            }
            const Insect *slot = map.getInsectAt(pos);

            // Affiche le contenu de la cellule sans espace supplémentaire
            if (getSlotContent(slot,0)==".") {
                std::cout << " " << getSlotContent(slot,0) << "  ";  // Trois espaces pour espacer les cases
            } else {
                std::cout << "" << getSlotContent(slot,0) << "  ";// Seulement deux espaces
            }
        }
    }

    /**
     * @brief Renvoie le contenu affichable d'une case.
     * @param slot Pointeur vers l'insecte dans la case (ou nullptr si vide).
     * @return Une chaîne de caractères représentant la valeur à afficher.
     */
    std::string getSlotContent(const Insect *slot, int idColor) const {
        if (slot) {
            return slot->getPrintableValue(idColor);  // Valeur de l'insecte sans espace supplémentaire
        }
        return "--";  // Affiche un point si la case est vide

    }
};

class ConsoleRenderer : public Renderer {
public:
    /**
     * @brief Constructeur de la classe ConsoleRenderer.
     * @param map_ Référence constante à la carte à afficher.
     */
    explicit ConsoleRenderer(const Map &map_, Inputs *inputs_,Player* P1_, Player* P2_) : Renderer(map_,inputs_, P1_,P2_) {}




    /**
     * @brief Affiche les règles de Hive.
     */
    void displayRules() const {
    std::cout << "\n=== Règles du jeu Hive ===\n\n";

    std::cout << "Objectif du jeu :\n";
    std::cout << "-----------------\n";
    std::cout << "Chaque joueur doit utiliser ses insectes pour encercler complètement l'abeille reine de son adversaire. "
              << "Le premier joueur à capturer l'abeille ennemie remporte la partie.\n\n";

    std::cout << "Règles générales :\n";
    std::cout << "-------------------\n";
    std::cout << "1. Les pièces doivent toujours rester connectées. Vous ne pouvez pas diviser la ruche en deux parties.\n";
    std::cout << "2. Chaque joueur doit poser son abeille reine sur le plateau dans les 4 premiers tours.\n";
    std::cout << "3. Les mouvements des insectes doivent respecter les limites physiques de la ruche :\n";
    std::cout << "   - Un insecte ne peut pas passer dans un espace trop étroit (règle du \"glissement\").\n";
    std::cout << "   - Certains insectes ont des mouvements uniques, décrits ci-dessous.\n\n";

    std::cout << "Caractéristiques des insectes :\n";
    std::cout << "--------------------------------\n";
    std::cout << "- Reine (Abeille) :\n";
    std::cout << "  • Se déplace d'une seule case par tour.\n";
    std::cout << "  • Elle est la pièce la plus importante. Si elle est complètement encerclée, vous perdez.\n\n";

    std::cout << "- Fourmi :\n";
    std::cout << "  • Peut se déplacer d'autant de cases que souhaité autour de la ruche.\n";
    std::cout << "  • Très utile pour la mobilité et la stratégie.\n\n";

    std::cout << "- Scarabée :\n";
    std::cout << "  • Se déplace d'une case comme la reine, mais peut grimper sur d'autres pièces.\n";
    std::cout << "  • Lorsqu'il grimpe, il bloque la pièce en dessous et la rend inutilisable.\n\n";

    std::cout << "- Sauterelle :\n";
    std::cout << "  • Saute en ligne droite au-dessus d'une ou plusieurs pièces adjacentes.\n";
    std::cout << "  • Ne peut pas s'arrêter sur des pièces, seulement sur des espaces vides.\n\n";

    std::cout << "- Araignée :\n";
    std::cout << "  • Doit se déplacer exactement de trois cases.\n";
    std::cout << "  • Ne peut pas revenir en arrière ou changer de direction avant d'avoir atteint trois cases.\n\n";

    std::cout << "Conseils :\n";
    std::cout << "----------\n";
    std::cout << "• Positionnez vos pièces stratégiquement pour défendre votre reine et attaquer celle de l'adversaire.\n";
    std::cout << "• Utilisez les mouvements uniques de vos insectes pour bloquer ou déstabiliser votre adversaire.\n";
    std::cout << "• Faites attention à ne pas isoler vos pièces, car cela pourrait limiter vos options de mouvement.\n\n";

    std::cout << "Bon jeu !\n";
}

};


class GraphicRenderer : public Renderer {
public:
    explicit GraphicRenderer(const Map &map_, Inputs *inputs_,Player* P1_, Player* P2_) : Renderer(map_,inputs_, P1_,P2_) {}
};

#endif // HIVE_RENDERER_H
