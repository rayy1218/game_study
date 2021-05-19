#include "main.hpp"

Hunger::Hunger(int max_hunger_point): max_hunger_point(max_hunger_point), 
                                      current_hunger_point(max_hunger_point) {}

int Hunger::doEat(int eat_hunger_point) {
    setCurrentHungerPoint(getCurrentHungerPoint() + eat_hunger_point);
    if (getCurrentHungerPoint() > getMaxHungerPoint()) {
        return eat_hunger_point + (getMaxHungerPoint() - getCurrentHungerPoint());
    }
    return eat_hunger_point;
}

void Hunger::doHungerDrop(int drop_hunger_point) {
    setCurrentHungerPoint(getCurrentHungerPoint() - drop_hunger_point);
    if (getCurrentHungerPoint() <= 0) {setCurrentHungerPoint(0);}
}

void Hunger::doUpdateHunger() {
    doHungerDrop(1);
}

void Hunger::doUpdateHungerEffect() {
    float hunger_percentage = ((float)getCurrentHungerPoint() / getMaxHungerPoint());
    game.player->combat_behavior->setAtkBoost(1);
    game.player->inventory->setBurdenBoost(1);
    
    if (current_hunger_point == 0) {
        game.player->combat_behavior->setCurrentHp(game.player->combat_behavior->getCurrentHp() - 1);
        game.gui->addMessage(TCODColor::red, "%s take 1 damage due to stravation", game.player->getName().c_str());
    }
    
    game.player->combat_behavior->setAtkBoost(game.player->combat_behavior->getAtkBoost() + (hunger_percentage - 0.5));
    game.player->inventory->setBurdenBoost(game.player->inventory->getBurdenBoost() + (hunger_percentage - 0.5));
}

int Hunger::getCurrentHungerPoint() {return current_hunger_point;}
int Hunger::getMaxHungerPoint() {return max_hunger_point;}
void Hunger::setCurrentHungerPoint(int input) {current_hunger_point = input;}
void Hunger::setMaxHungerPoint(int input) {max_hunger_point = input;}
