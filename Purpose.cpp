#include "main.hpp"

Purpose::Purpose() {
    
}

Purpose::~Purpose() {
    
}

PurposeHeal::PurposeHeal(int heal_amount): heal_amount(heal_amount) {}

bool PurposeHeal::doUse(Entity* target) {
    int healed = target->combat_behavior->doEntityHealed(heal_amount);
    
    if (healed <= 0) {
        game.gui->addMessage(TCODColor::white, "Heal didn't apply to %s due to HP already full",
                             target->getName().c_str());
        return false;
    }
    game.gui->addMessage(TCODColor::green, "%s was healed for %iHP", 
                         target->getName().c_str(), healed);
    return true;
}

PurposeDamage::PurposeDamage(int damage): damage(damage) {}

bool PurposeDamage::doUse(Entity *target) {
    int damage_dealt = target->combat_behavior->doEntityAttacked(damage);
    TCODColor message_color = (target == game.player) ? TCODColor::red : TCODColor::green; 
    game.gui->addMessage(message_color, "%s take %i - idamage", 
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

PurposeItemEquipment::PurposeItemEquipment(int attack_point, int defense_point, 
                             float attack_boost, float defense_boost):
                             attack_point(attack_point), defense_point(defense_point),
                             attack_boost(attack_boost), defense_boost(defense_boost) {}

bool PurposeItemEquipment::doUse(Entity *self) {
    CombatBehavior *cbt = self->combat_behavior;
    cbt->setEquipmentDefPoint(cbt->getEquipmentDefPoint() + defense_point);
    return true;
}

PurposeItemWeapon::PurposeItemWeapon(int attack_point, float speed):
                          attack_point(attack_point), speed(speed) {}

bool PurposeItemWeapon::doUse(Entity *self) {
    CombatBehavior *cbt = self->combat_behavior;
    cbt->setEquipmentAtkPoint(cbt->getEquipmentAtkPoint() + attack_point);
    cbt->setSpeed(speed);
    return true;
}

PurposeItemFood::PurposeItemFood(int hunger_point): hunger_point(hunger_point) {}

bool PurposeItemFood::doUse(Entity *target) {
    game.player_stats->hunger->doEat(hunger_point);
    return true;
}

