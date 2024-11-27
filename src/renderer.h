//
// Created by loic_ on 26/11/2024.
//

#ifndef HIVE_RENDERER_H
#define HIVE_RENDERER_H

#include <iostream>
#include "features/map.h"

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
            // Affiche le séparateur horizontal si ce n'est pas la première ligne
            if (row > 0) {
                displaySeparator(row);
            }

            // Affiche la ligne actuelle
            displayRow(row);
        }
    }

private:
    const Map &map;

    /**
     * @brief Affiche une ligne de cellules avec des insectes ou des cases vides.
     * @param rowIndex L'indice de la ligne à afficher.
     */
    void displayRow(size_t rowIndex) const {
        size_t sideSize = map.getSideSize();

        // Indentation pour la grille hexagonale
        if (rowIndex % 2 == 1) {
            std::cout << "  ";
        }

        for (size_t col = 0; col < sideSize; ++col) {
            vec2i pos(static_cast<int>(rowIndex), static_cast<int>(col));
            const Insect *slot = map.getInsect(pos);

            // Affiche le contenu de la cellule
            std::cout << "| " << getSlotContent(slot) << " ";
        }

        std::cout << "|" << std::endl; // Termine la ligne
    }

    /**
     * @brief Affiche les séparateurs horizontaux entre les lignes.
     * @param rowIndex L'indice de la ligne.
     */
    void displaySeparator(size_t rowIndex) const {
        size_t sideSize = map.getSideSize();

        if (rowIndex % 2 == 1) {
            std::cout << "  ";
        }

        for (size_t col = 0; col < sideSize; ++col) {
            std::cout << "\\ / ";
        }

        std::cout << "\\" << std::endl;
    }

    /**
     * @brief Renvoie le contenu affichable d'une case.
     * @param slot Pointeur vers l'insecte dans la case (ou nullptr si vide).
     * @return Une chaîne de caractères représentant la valeur à afficher.
     */
    std::string getSlotContent(const Insect *slot) const {
        if (slot) {
            return slot->getPrintableValue(); // Valeur de l'insecte
        }
        return "  "; // Case vide
    }
};

#endif //HIVE_RENDERER_H
