//
// Created by loic_ on 21/12/2024.
//

#ifndef AI_H
#define AI_H
#include "map.h"
#include "player.h"
#include "../utils/random.hpp"

class AI
{

public:
     void operator()(Map & map, Player & player_, Inputs & inputs_, lv::Random & random, bool Qt=false, bool inputT=false)
     {

         if (player_.getDeck().isEmpty() && player_.getActiveInsects().empty()){
             throw HiveException("inputsManager.h:InputsManager:updateAIInputs","deck and activeInsects are empty");
             return;
         }

         int randomValue = std::max(0,random.getRandomInt(0,2));

         std::cout << "randomValue: " << randomValue;

         const Insect * selectedInsect;


         if (player_.getActiveInsects().empty() && !player_.getDeck().isEmpty())
         {
             int deckIndex=player_.getDeck().returnIndex(bee); //On place bee pour pas avoir de problèmes de mouvements impossibles
             selectedInsect = player_.getDeck().getInsectAt(deckIndex);

             if (player_.getId() == 1)
             {
                 inputs_.setStart({-1,deckIndex});
             }
             else
             {
                 inputs_.setStart({30,deckIndex});
             }
             inputs_.selectStart();

             inputs_.setPossibleDestinations(map.setRule(player_.getId() % 2));

             const int destinationsIndex = random.getRandomInt(0,static_cast<int>(inputs_.getPossibleDestinations().size()));
             inputs_.setDestinationIndex(destinationsIndex);

             inputs_.selectDestination();

             randomValue = -1;
         }

         if (randomValue == 0)
         {
             if (!player_.getActiveInsects().empty())
             {
                 std::cout << "map to map ";
                 const int fromIndex = random.getRandomInt(0,static_cast<int>(player_.getActiveInsects().size()));

                 selectedInsect = player_.getActiveInsects()[fromIndex];

                 std::cout << "selectedInsect: " << selectedInsect->getPV();

                 inputs_.setStart(selectedInsect->getCoordinates());
                 inputs_.selectStart();

                 inputs_.setPossibleDestinations(selectedInsect->getPossibleMovements(map));

                 const int destinationsIndex = random.getRandomInt(0,static_cast<int>(inputs_.getPossibleDestinations().size()));
                 inputs_.setDestinationIndex(destinationsIndex);

                 inputs_.selectDestination();

                 std::cout << "->" << inputs_;
             }
             else
             {
                randomValue++;
             }

         }

         if (randomValue == 1 && player_.getDeck().getInsectNb() > 0) //Deck
         {
             std::cout << "deck to map ";

             const int deckIndex = random.getRandomInt(0,player_.getDeck().getInsectNb());
             selectedInsect = player_.getDeck().getInsectAt(deckIndex);

             if (player_.getId() == 1 || Qt)
             {
                 inputs_.setStart({-1,deckIndex});
             }
             else
             {
                 inputs_.setStart({30,deckIndex});
             }
             inputs_.selectStart();

             inputs_.setPossibleDestinations(map.setRule(player_.getId() % 2));

             const int destinationsIndex = random.getRandomInt(0,static_cast<int>(inputs_.getPossibleDestinations().size()));
             inputs_.setDestinationIndex(destinationsIndex);

             inputs_.selectDestination();
         }


         /*if (player_.getActiveInsects().empty() && !player_.getDeck().isEmpty()) {
             int index=player_.getDeck().returnIndex(bee); //On place bee pour pas avoir de problèmes de mouvements impossibles
             selectedInsect = player_.getDeck().getInsectAt(index);
         } else if (randomValue == 0 && !player_.getDeck().isEmpty()){
             selectedInsect = player_.getDeck().getInsectAt(random.getRandomInt(0,int(player_.getDeck().getInsectNb())));
         } else {
             selectedInsect = player_.getActiveInsects()[random.getRandomInt(0,int(player_.getActiveInsects().size()))];
             inputs.setStart(selectedInsect->getCoordinates());
             vec2i startPos = selectedInsect->getCoordinates();
             inputs.setStart(startPos);
             inputs.needPossibleDestinationsUpdate();
             break;
         }




        int cursorId=0;
        if (!Qt) cursorId = inputs.isStartSelected() + 1;
        else {
            if (inputT) cursorId=1; //Si c'est la première sélection
            else cursorId = 2; //Si c'est la deuxième sélection
        }

         std::cout << "----------------\n:" << cursorId << " " << Qt << std::endl;

        int randomValue = random.getRandomInt(0,4);
        const Insect * selectedInsect;

        switch (cursorId){
            case 1:


                if (player_.getActiveInsects().empty() && !player_.getDeck().isEmpty()) {
                    int index=player_.getDeck().returnIndex(bee); //On place bee pour pas avoir de problèmes de mouvements impossibles
                    selectedInsect = player_.getDeck().getInsectAt(index);
                } else if (randomValue == 0 && !player_.getDeck().isEmpty()){
                    selectedInsect = player_.getDeck().getInsectAt(random.getRandomInt(0,int(player_.getDeck().getInsectNb())));
                } else {
                    selectedInsect = player_.getActiveInsects()[random.getRandomInt(0,int(player_.getActiveInsects().size()))];
                    inputs.setStart(selectedInsect->getCoordinates());
                    vec2i startPos = selectedInsect->getCoordinates();
                    inputs.setStart(startPos);
                    inputs.needPossibleDestinationsUpdate();
                    break;
                }
                if (!Qt) {//Si on est pas dans Qt
                    inputs.setStart(selectedInsect->getCoordinates());
                } else {
                    vec2i startPos = selectedInsect->getCoordinates();
                    startPos.setI(-1);
                    inputs.setStart(startPos);
                }

                inputs.needPossibleDestinationsUpdate();

                break;
            case 2:
                if (!Qt) {
                    if (!inputs.isPossibleDestinationsEmpty()) {
                        inputs.setDestinationIndex(random.getRandomInt(0,int (inputs.getPossibleDestinationsNumber())));
                    }
                } else {
                    if (inputs.getStart().getI()==-1) {
                        inputs.setPossibleDestinations(map.setRule(false));
                        if (!inputs.getPossibleDestinations().empty()) {
                            inputs.setDestinationIndex(random.getRandomInt(0,inputs.getPossibleDestinationsNumber()));
                        } else {//Si l'IA ne peut pas poser d'insectes, elle réupdate ses inputs (recursif est pas optimisé, mais moins long à écrire pour l'instant)
                            operator()(map, player_, inputs, random, Qt, !inputT);
                            operator()(map, player_, inputs, random, Qt, inputT);
                        }
                    } else {
                        inputs.setPossibleDestinations(map.getInsectAt(inputs.getStart())->getPossibleMovements(map));
                        if (!inputs.getPossibleDestinations().empty()) {
                            inputs.setDestinationIndex(random.getRandomInt(0,inputs.getPossibleDestinationsNumber()));
                        } else {//Si l'IA ne peut pas déplacer d'insectes, elle réupdate ses inputs (recursif est pas optimisé, mais moins long à écrire pour l'instant)
                            operator()(map, player_, inputs, random, Qt, !inputT);
                            operator()(map, player_, inputs, random, Qt, inputT);
                        }
                    }
                }
            break;
            default:
                throw HiveException("inputsManager.h:InputsManager:moveCursor", "cursorId_ invalid");
            break;
        }*/
     }

};

#endif//AI_H
