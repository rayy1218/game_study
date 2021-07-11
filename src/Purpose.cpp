#include "main.hpp"

Purpose::Purpose() {
    
}

Purpose::~Purpose() {
    
}

PurposeHpRestore::PurposeHpRestore(int restore_amount): restore_amount(restore_amount) {}

bool PurposeHpRestore::doUse(Entity* target) {
    int restored = target->combat_behavior->doEntityHealed(restore_amount);
    
    if (restored <= 0) {
        game.gui->addMessage(TCODColor::white, "Heal didn't apply to %s due to HP already full",
                             target->getName().c_str());
        return false;
    }
    game.gui->addMessage(TCODColor::green, "%s was restored for %iHP",
                         target->getName().c_str(), restored);
    return true;
}

PurposeMpRestore::PurposeMpRestore(int restore_amount): restore_amount(restore_amount) {}

bool PurposeMpRestore::doUse(Entity* target) {

    return true;
}

PurposeDamage::PurposeDamage(int damage): damage(damage) {}

bool PurposeDamage::doUse(Entity *target) {
    int damage_dealt = target->combat_behavior->doEntityAttacked(damage);
    TCODColor message_color = (target == game.player) ? TCODColor::red : TCODColor::green; 
    game.gui->addMessage(message_color, "%s take %i - %i damage", 
                         target->getName().c_str(), damage, damage - damage_dealt);
    target->combat_behavior->checkEntityDead();
    return true;
}

PurposeDirectDamage::PurposeDirectDamage(int damage): damage(damage) {}

bool PurposeDirectDamage::doUse(Entity *target) {
    CombatBehavior *cbt = target->combat_behavior;
    cbt->setCurrentHp(cbt->getCurrentHp() - damage);
    TCODColor message_color = (target == game.player) ? TCODColor::red : TCODColor::green; 
    game.gui->addMessage(message_color, "%s take %i damage", target->getName().c_str(), damage);
    target->combat_behavior->checkEntityDead();
    return true;
}

PurposeEffectBurn::PurposeEffectBurn(int damage, int duration): damage(damage), duration(duration) {}

bool PurposeEffectBurn::doUse(Entity *target) {
    Effect *burn_effect = new EffectBurn(target, duration, damage);
    target->all_effect.push(burn_effect);
    return true;
}

PurposeEffectConfusion::PurposeEffectConfusion(int duration): duration(duration) {}

bool PurposeEffectConfusion::doUse(Entity *target) {
    Effect *confusing_effect = new EffectConfusion(target, duration);
    target->all_effect.push(confusing_effect);
    return true;
}

PurposeEffectFrozen::PurposeEffectFrozen(int duration): duration(duration) {}

bool PurposeEffectFrozen::doUse(Entity *target) {
    Effect *frozen_effect = new EffectFrozen(target, duration);
    target->all_effect.push(frozen_effect);
    return true;
}

PurposeEffectPoison::PurposeEffectPoison(int damage, int duration): damage(damage), duration(duration) {}

bool PurposeEffectPoison::doUse(Entity *target) {
    Effect *poison_effect = new EffectPoison(target, duration, damage);
    target->all_effect.push(poison_effect);
    return true;
}

PurposeEffectHpRegen::PurposeEffectHpRegen(int duration, int regen_amount): duration(duration), regen_amount(regen_amount) {}

bool PurposeEffectHpRegen::doUse(Entity *target) {
    Effect *hp_regen = new EffectHpRegen(target, duration, regen_amount);
    target->all_effect.push(hp_regen);
    return true;
}

PurposeEffectMpRegen::PurposeEffectMpRegen(int duration, int regen_amount): duration(duration), regen_amount(regen_amount) {}

bool PurposeEffectMpRegen::doUse(Entity *target) {
    Effect *mp_regen = new EffectHpRegen(target, duration, regen_amount);
    target->all_effect.push(mp_regen);
    return true;
}

PurposeEffectRage::PurposeEffectRage(float atk_boost, int duration): duration(duration), atk_boost(atk_boost) {}

bool PurposeEffectRage::doUse(Entity *target) {
    Effect *rage = new EffectRage(target, duration, atk_boost);
    target->all_effect.push(rage);
    return true;
}

PurposeEffectProtection::PurposeEffectProtection(float def_boost, int duration): duration(duration), def_boost(def_boost) {}

bool PurposeEffectProtection::doUse(Entity *target) {
    Effect *protection = new EffectProtection(target, duration, def_boost);
    target->all_effect.push(protection);
    return true;
}

PurposeItemFood::PurposeItemFood(int hunger_point): hunger_point(hunger_point) {}

bool PurposeItemFood::doUse(Entity *target) {
    game.player_stats->hunger->doEat(hunger_point);
    return true;
}

