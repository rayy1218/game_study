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
    if (getCurrentTension() > (getMaxTension() / 5 * 3) && game.global_rng->getInt(1, 100) >= 95) {
        switch (game.global_rng->getInt(1, 3)) {
            case 1: {
                //drop weapon
                Entity *to_drop;
                if (game.player->equipment->isPrimaryHand()) {
                    to_drop = game.player->equipment->getEquipment(equipment_slot_type::primary_hand);
                }
                else {
                    to_drop = game.player->equipment->getEquipment(equipment_slot_type::secondary_hand);
                }
                game.player->inventory->dropItem(to_drop);
                break;
            }

            case 2: {
                //slipped and stunned
                const int slipped_damage = 5;
                int damaged = game.player->combat_behavior->doEntityAttacked(slipped_damage);
                game.gui->addMessage(TCODColor::red, "%s slipped and take %i - %i damage",
                                     game.player->getName().c_str(), slipped_damage, damaged - slipped_damage);
                Effect *effect = new EffectStun(game.player, 3);
                game.player->all_effect.push(effect);
                break;
            }

            case 3: {
                //confused
                Effect *effect = new EffectConfusion(game.player, 5);
                game.player->all_effect.push(effect);
                break;
            }
        }
    }
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