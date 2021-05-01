#include "main.hpp"

ItemPurpose::ItemPurpose() {
    
}

ItemPurpose::~ItemPurpose() {
    
}

ItemHeal::ItemHeal(int heal_amount): heal_amount(heal_amount) {}

bool ItemHeal::doUse(Entity* target) {
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

ItemDamage::ItemDamage(int damage): damage(damage) {}

bool ItemDamage::doUse(Entity* target) {
    int damage_dealt = target->combat_behavior->doEntityAttacked(damage);
    game.gui->addMessage(TCODColor::green, "%s take %i damage", 
                         target->getName().c_str(), damage_dealt);
    return true;
}

ItemEffectBurn::ItemEffectBurn(int damage, int duration): damage(damage), duration(duration) {}

bool ItemEffectBurn::doUse(Entity *target) {
    Effect *burn_effect = new EffectBurn(target, duration, damage);
    target->all_effect.push(burn_effect);
    return true;
}

ItemEffectConfusion::ItemEffectConfusion(int duration): duration(duration) {}

bool ItemEffectConfusion::doUse(Entity *target) {
    Effect *confusing_effect = new EffectConfusion(target, duration);
    target->all_effect.push(confusing_effect);
    return true;
}

ItemEquipment::ItemEquipment(int attack_point, int defense_point, 
                             float attack_boost, float defense_boost):
                             attack_point(attack_point), defense_point(defense_point),
                             attack_boost(attack_boost), defense_boost(defense_boost) {}


bool ItemEquipment::doUse(Entity *self) {
    CombatBehavior *cbt = self->combat_behavior;
    cbt->setEquipmentAtkPoint(cbt->getEquipmentAtkPoint() + attack_point);
    cbt->setEquipmentDefPoint(cbt->getEquipmentDefPoint() + defense_point);
}