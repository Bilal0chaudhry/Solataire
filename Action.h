#pragma once
#include <vector>
#include "Card.h"

enum class ActionType {
    DRAW_CARD,
    MOVE_CARD,
    MOVE_WASTE_TO_TABLEAU,
    MOVE_WASTE_TO_FOUNDATION,
    MOVE_TABLEAU_TO_FOUNDATION,
};

class Action {
public:

    Action(ActionType type, const Card& card, int sourceIndex, int targetIndex,
        bool wasSourceFaceDown = false, bool wasTargetFaceDown = false)
        : type(type), card(card), cards({ card }), sourceIndex(sourceIndex), targetIndex(targetIndex),
        wasSourceCardFaceDown(wasSourceFaceDown), wasTargetCardFaceDown(wasTargetFaceDown) {}

    Action(ActionType type, const std::vector<Card>& cards, int sourceIndex, int targetIndex,
        bool wasSourceFaceDown, bool wasTargetFaceDown)
        : type(type), cards(cards), sourceIndex(sourceIndex), targetIndex(targetIndex),
        wasSourceCardFaceDown(wasSourceFaceDown), wasTargetCardFaceDown(wasTargetFaceDown) {}

    ActionType type;
    Card card;                   
    std::vector<Card> cards;      
    int sourceIndex;             
    int targetIndex;             
    bool wasSourceCardFaceDown;   
    bool wasTargetCardFaceDown;   
};
