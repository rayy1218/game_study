#include <cmath>

#include "main.hpp"

Tension::Tension(int max_tension): max_tension(max_tension), current_tension(0) {}

int Tension::doDropTension(int tension_drop) {
    if (getCurrentTension() - tension_drop < 0) {
        tension_drop = getCurrentTension();
        setCurrentTension(0);
        return tension_drop;
    }
    setCurrentTension(getCurrentTension() - tension_drop);
    return tension_drop;
}

int Tension::doRiseTension(int tension_rise) {
    if (getCurrentTension() + tension_rise > getMaxTension()) {
        tension_rise = getMaxTension() - getCurrentTension();
        setCurrentTension(getMaxTension());
        return tension_rise;
    }
    setCurrentTension(getCurrentTension() + tension_rise);
    return tension_rise;
}

void Tension::doUpdateTension() {
    int monster_num = getMonsterNumInFov();
    
    if (monster_num == 0) {
        doDropTension(1);
        return;
    }
    
    doRiseTension(pow(monster_num, 2));
}

void Tension::doUpdateTensionEffect() {
    
}

int Tension::getMonsterNumInFov() {
    int monster_count = 0;
    for (Entity *character : game.all_character) {
        if (character == game.player || 
            !game.map->isInFov(character->getX(), character->getY())) {continue;}
        monster_count++;
    }
    return monster_count;
}

int Tension::getCurrentTension() {return current_tension;}

int Tension::getMaxTension() {return max_tension;}

void Tension::setCurrentTension(int input) {current_tension = input;}

void Tension::setMaxTension(int input) {max_tension = input;}