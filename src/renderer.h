#ifndef HIVE_RENDERER_H
#define HIVE_RENDERER_H

#include <iostream>
#include "features/map.h"
#include "features/inputs.h"
#include "features/player.h"

// Dans tout le renderer, idColor = 0, affiche sans couleur spécifique, idColor = 1, cursor Start avant selec
// idColor = 2, cursor start apres select, idColor = 1, cursor parmi les possibles, idColor = 3, movement possibles
/**
 * @brief Classe pour afficher la carte d'une partie de Hive.
 */
class Renderer {
public:
    /**
     * @brief Constructeur de la classe Renderer.
     * @param map_ Référence constante à la carte à afficher.
     */
    explicit Renderer(const Map &map_,Player* P1_, Player* P2_, int rendered_s_i) : map(map_), P1(P1_), P2(P2_), renderedSideSize(rendered_s_i) {} // A voir comment initialiser l'output

    /**
     * @brief Affiche la carte sur la sortie standard.
     */
    virtual ~Renderer() = default; // Rendre Renderer polymorphe avec un destructeur virtuel
    virtual void displayMap(const Player & currentPlayer_) const=0;
    virtual void render(const Player & currentPlayer_) const = 0;

protected:
    Player *P1;
    Player *P2;
    int renderedSideSize;
    const Map &map;



};

class ConsoleRenderer : public Renderer {
public:
    /**
     * @brief Constructeur de la classe ConsoleRenderer.
     * @param map_ Référence constante à la carte à afficher.
     */
    explicit ConsoleRenderer(const Map &map_,Player* P1_, Player* P2_, int rendered_s_i) : Renderer(map_,P1_,P2_, rendered_s_i) {}


    /**
     * @brief Affiche les indentations pour simuler les décalages de la grille hexagonale.
     * @param rowIndex L'indice de la ligne à afficher.
     */
    static void displayIndentation(size_t rowIndex) {
        // Lignes impaires (indexées 1, 3, 5...) doivent être décalées vers la droite
        if ((rowIndex-1) % 2 == 1) {
            std::cout << "  ";  // Ajoute des espaces pour décaler
        }
    }



    /**
     * @brief Affiche une ligne de cellules avec des insectes ou des cases vides.
     * @param rowIndex L'indice de la ligne à afficher.
     */
    void displayRow(size_t rowIndex, vec2i start) const {

        for (size_t col = 1; col < renderedSideSize+1; ++col) {
            vec2i pos(static_cast<int>(rowIndex), static_cast<int>(col));
            if(pos.getI()==start.getI()+1 && pos.getJ()==start.getJ()+1)
            {
                const Insect *slot = map.getInsectAt(pos);
                if (getSlotContent(slot, 0)==".") {
                    std::cout << " " << getSlotContent(slot,1) << "  ";  // Trois espaces pour espacer les cases
                } else {
                    std::cout << "" << getSlotContent(slot,1) << "  ";// Seulement deux espaces
                }
            }
            else
            {
                const Insect *slot = map.getInsectAt(pos);

                // Affiche le contenu de la cellule sans espace supplémentaire
                if (getSlotContent(slot,0)==".") {
                    std::cout << " " << getSlotContent(slot,0) << "  ";  // Trois espaces pour espacer les cases
                } else {
                    std::cout << "" << getSlotContent(slot,0) << "  ";// Seulement deux espaces
                }
            }
        }

    }

    /**
     * @brief Renvoie le contenu affichable d'une case.
     * @param slot Pointeur vers l'insecte dans la case (ou nullptr si vide).
     * @return Une chaîne de caractères représentant la valeur à afficher.
     */
    static std::string getSlotContent(const Insect *slot, int idColor) {
        if (slot) {
            return slot->getPrintableValue(idColor);  // Valeur de l'insecte sans espace supplémentaire
        }
        if(idColor==1)
        {
            return "\033[92m--\033[37m";
        }
        return "--";  // Affiche un point si la case est vide

    }

    static void displayDeck(Player* P, int indexPlayer) {

        for(int index=0; index< P->getDeck().getInsectNb();index++)
        {
            // si cursor en ligne -1 donc dans le deck du joueur bleu
            if((P->getInputs().getStart().getI()==-1 && indexPlayer==0) && P->getInputs().getStart().getJ()==index)
            {
                std::cout << " " << getSlotContent(P->getDeck().getInsectAt(index),1) << "  ";
            }
                // si cursor en ligne renderedSideSize donc dans le deck joueur rouge
            else if((P->getInputs().getStart().getI()==-1 && indexPlayer==1)&& P->getInputs().getStart().getJ()==index)
            {
                std::cout << " " << getSlotContent(P->getDeck().getInsectAt(index),1) << "  ";
            }
            else
            {
                std::cout << " " << getSlotContent(P->getDeck().getInsectAt(index),0) << "  ";
            }
        }
    }

    void displayMap(const Player & currentPlayer_) const override{
        //system("cls"); // clear console windows
        std::cout << "\033[2J\033[1;1H";  // clear console linux
        displayDeck(P1,0);
        std::cout << "\n" << "\n";

        for (size_t row = 1; row < renderedSideSize+1; ++row) {
            // Affiche les espaces d'indentation pour simuler les décalages
            displayIndentation(row);

            // Affiche la ligne actuelle
            displayRow(row, currentPlayer_.getInputs().getStart());         // A finir

            // Affiche un saut de ligne pour séparer les lignes
            std::cout << "\n";
        }
        std::cout << "\n" << "\n";
        displayDeck(P2,1);
    }

    void renderDeck1(const Player & currentPlayer_) const{
        std::cout << "\033[31m";
        if (currentPlayer_.getInputs().getStart().getI() == -1){
            std::cout << "caca";
            for(const auto & insect: P1->getDeck()){
                std::cout << insect->getCoordinates();
                if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getStart().getJ()){
                    std::cout << "caca2";
                    if (currentPlayer_.getInputs().isStartSelected()){
                        std::cout << "\033[92m" << insect->getPV() << "\033[31m ";
                    }
                    else{
                        std::cout << "\033[35m" << insect->getPV() << "\033[31m ";
                    }
                }
                else if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getDestination().getJ()){
                    if (!currentPlayer_.getInputs().isDestinationSelected()){
                        std::cout << "\033[35m" << insect->getPV() << "\033[31m ";
                    }
                }
                else std::cout << insect->getPV() << " ";
            }
        }
        else{
            for(const auto & insect: P1->getDeck()){
                std::cout << insect->getPV() <<" ";
            }
        }
        std::cout << '\n';
    }

    void renderDeck2(const Player & currentPlayer_) const{
        std::cout << "\033[34m";
        if (currentPlayer_.getInputs().getStart().getI() == renderedSideSize){
            for(const auto & insect: P2->getDeck()){
                if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getStart().getJ()){
                    if (currentPlayer_.getInputs().isStartSelected()){
                        std::cout << "\033[92m" << insect->getPV() << "\033[34m ";
                    }
                    else{
                        std::cout << "\033[35m" << insect->getPV() << "\033[34m ";
                    }
                }
                else if (insect->getCoordinates().getJ() == currentPlayer_.getInputs().getDestination().getJ()){
                    if (!currentPlayer_.getInputs().isDestinationSelected()){
                        std::cout << "\033[35m" << insect->getPV() << "\033[34m ";
                    }
                }
                else std::cout << insect->getPV() <<" ";
            }
        }
        else{
            for(const auto & insect: P2->getDeck()){
                std::cout << insect->getPV() <<" ";
            }
        }
        std::cout << "\n\033[37m";
    }

    static bool isVec2iInVector(const std::vector<vec2i> & vector_, const vec2i & vec2i_) {
        for(const auto & elt : vector_){
            if (elt == vec2i_) return true;
        }
        return false;
    }

    void renderMap(const Player & currentPlayer_) const{
        std::cout << "\033[37m";
        for (int i = 0; i < renderedSideSize; ++i){
            if (i%2 == 0) {
                std::cout << "\033[37m-";
            }
            else {
                std::cout << "\033[37m  -";
            }

            for (int j = 0; j < renderedSideSize; ++j) {

                const vec2i insectPos{i,j};

                const Insect * insect = map.getInsectAt(insectPos);

                if (insect){
                    if (currentPlayer_.getInputs().getStart() == insectPos){
                        if (currentPlayer_.getInputs().isStartSelected()){
                            std::cout << "\033[92m-" << insect->getPV() << "-\033[37m";
                        }
                        else{
                            std::cout << "\033[35m-" << insect->getPV() << "-\033[37m";
                        }
                    }
                    else if (currentPlayer_.getInputs().getDestination() == insectPos){
                        if (!currentPlayer_.getInputs().isDestinationSelected()){
                            std::cout << "\033[35m-" << insect->getPV() << "-\033[37m";
                        }
                    }
                    else if (isVec2iInVector(currentPlayer_.getInputs().getPossibleDestinations(), insectPos)){
                        std::cout << "\033[33m-" << insect->getPV() << "-\033[37m";
                    }
                    else{
                        if (insect->getColor()) {
                            std::cout << "\033[31m-" << insect->getPV() << "-\033[37m";
                        }
                        else{
                            std::cout << "\033[34m-" << insect->getPV() << "-\033[37m";
                        }
                    }
                }
                else{
                    if (currentPlayer_.getInputs().getStart() == insectPos){
                        if (currentPlayer_.getInputs().isStartSelected()){
                            std::cout << "\033[92m-" << "  " << "-\033[37m";
                        }
                        else{
                            std::cout << "\033[35m-" << "  " << "-\033[37m";
                        }
                    }
                    else if (currentPlayer_.getInputs().getDestination() == insectPos){
                        if (!currentPlayer_.getInputs().isDestinationSelected()){
                            std::cout << "\033[35m-" << "  " << "-\033[37m";
                        }
                    }
                    else if (isVec2iInVector(currentPlayer_.getInputs().getPossibleDestinations(), insectPos)){
                        std::cout << "\033[33m-" << "  " << "-\033[37m";
                    }
                    else{
                        std::cout << "-  -";
                    }
                }

            }

            std::cout << "-\n";

        }

    }

    void render(const Player & currentPlayer_) const override{
        //system("cls");
        std::cout << '\n';
        renderDeck1(currentPlayer_);
        std::cout<<"1";
        renderMap(currentPlayer_);
        std::cout<<"2";
        renderDeck2(currentPlayer_);
        std::cout << '\n';
    }


    /**
     * @brief Affiche les règles de Hive.
     */
    static void displayRules() {
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
    explicit GraphicRenderer(const Map &map_, Player* P1_, Player* P2_, int rendered_s_i) : Renderer(map_,P1_,P2_,rendered_s_i) {}
    void displayMap(const Player & currentPlayer_) const override{}
    void render(const Player & currentPlayer_) const override{}
};

#endif // HIVE_RENDERER_H
