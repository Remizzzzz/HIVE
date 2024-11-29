#ifndef HIVE_RENDERER_H
#define HIVE_RENDERER_H

#include <iostream>
#include "features/map.h"
//test
/**
 * @brief Classe pour afficher la carte d'une partie de Hive.
 */
class Renderer {
public:
    /**
     * @brief Constructeur de la classe Renderer.
     * @param map_ Référence constante à la carte à afficher.
     */
    explicit Renderer(const Map &map_) : map(map_) {}

    /**
     * @brief Affiche la carte sur la sortie standard.
     */
    void displayMap() const {
        size_t sideSize = map.getSideSize();

        for (size_t row = 0; row < sideSize; ++row) {
            // Affiche les espaces d'indentation pour simuler les décalages
            displayIndentation(row);

            // Affiche la ligne actuelle
            displayRow(row);

            // Affiche un saut de ligne pour séparer les lignes
            std::cout << std::endl;
        }
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
    void displayRow(size_t rowIndex) const {
        size_t sideSize = map.getSideSize();

        for (size_t col = 0; col < sideSize; ++col) {
            vec2i pos(static_cast<int>(rowIndex), static_cast<int>(col));
            const Insect *slot = map.getInsectAt(pos);

            // Affiche le contenu de la cellule sans espace supplémentaire
            if (getSlotContent(slot)==".") {
                std::cout << " "<< getSlotContent(slot) << "  ";  // Trois espaces pour espacer les cases
            } else {
                std::cout << "" << getSlotContent(slot) << "  ";// Seulement deux espaces
            }
        }
    }

    /**
     * @brief Renvoie le contenu affichable d'une case.
     * @param slot Pointeur vers l'insecte dans la case (ou nullptr si vide).
     * @return Une chaîne de caractères représentant la valeur à afficher.
     */
    std::string getSlotContent(const Insect *slot) const {
        if (slot) {
            return slot->getPrintableValue();  // Valeur de l'insecte sans espace supplémentaire
        }
        return "--";  // Affiche un point si la case est vide
    }
};

#endif // HIVE_RENDERER_H
