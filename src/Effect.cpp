#include "main.hpp"

Effect::Effect(Entity *self, int duration): self(self), duration(duration) {
}

Effect::~Effect() {
}

bool Effect::doUpdate() {
    duration--;
    if (duration <= 0) {
        self->all_effect.remove(this);
        delete this;
    }
    return (duration <= 0);
}

int Effect::getEffectID() {
    return effect_id::none;
}

EffectBurn::EffectBurn(Entity *self, int duration, int damage): Effect(self, duration),
                                                                damage(damage) {}

bool EffectBurn::doUpdate() {
    int burn_damage = self->combat_behavior->doEntityAttacked(damage);
    TCODColor message_color = (self == game.player) ? TCODColor::red : TCODColor::green;
    if (game.map->isInFov(self->getX(), self->getY())) {
        game.gui->addMessage(message_color, "%s is burned for %i - %i damage", 
                             self->getName().c_str(), burn_damage, damage - burn_damage);
    }
    self->combat_behavior->checkEntityDead();
    return Effect::doUpdate();
}

int EffectBurn::getEffectID() {
    return effect_id::burn;
}

EffectConfusion::EffectConfusion(Entity *self, int duration): Effect(self, duration),
                                                              orig(self->control) {

    self->control = new ConfusedControl(self);
    TCODColor message_color = (self == game.player) ? TCODColor::red : TCODColor::green;
    if (game.map->isInFov(self->getX(), self->getY())) {
        game.gui->addMessage(message_color, "%s is confused", self->getName().c_str());
    }
}

bool EffectConfusion::doUpdate() {
    if (duration - 1 <= 0) {
        TCODColor message_color = (self == game.player) ? TCODColor::green : TCODColor::red;
        if (game.map->isInFov(self->getX(), self->getY())) {
            game.gui->addMessage(message_color, "%s is not longer confused", self->getName().c_str());
        }
        Control *temp = self->control;
        self->control = orig;
        delete temp;
    }
    return Effect::doUpdate();
}

int EffectConfusion::getEffectID() {
    return effect_id::confusion;
}

EffectPoison::EffectPoison(Entity *self, int duration, int damage): Effect(self, duration),
                                                                    damage(damage) {}
                                                                
bool EffectPoison::doUpdate() {
    TCODColor message_color = (self == game.player) ? TCODColor::red : TCODColor::green;
    self->combat_behavior->setCurrentHp(self->combat_behavior->getCurrentHp() - damage);
    if (game.map->isInFov(self->getX(), self->getY())) {
        game.gui->addMessage(message_color, "%s take %i damage because of poison",
                             self->getName().c_str(), damage);
    }
    self->combat_behavior->checkEntityDead();
    return Effect::doUpdate();
}

int EffectPoison::getEffectID() {
    return effect_id::poison;
}

EffectFrozen::EffectFrozen(Entity *self, int duration): Effect(self, duration), orig(self->control) {
    self->control = new StunnedControl(self);
    TCODColor message_color = (self == game.player) ? TCODColor::red : TCODColor::green;
    if (game.map->isInFov(self->getX(), self->getY())) {
        game.gui->addMessage(message_color, "%s is frezzed", self->getName().c_str());
    }
}

int EffectFrozen::getEffectID() {
    return effect_id::frozen;
}

bool EffectFrozen::doUpdate() {
    if (duration - 1 <= 0) {
        TCODColor message_color = (self == game.player) ? TCODColor::green : TCODColor::red;
        if (game.map->isInFov(self->getX(), self->getY())) {
            game.gui->addMessage(message_color, "%s is not longer freezed", self->getName().c_str());
        }
        Control *temp = self->control;
        self->control = orig;
        delete temp;
    }
    
    return Effect::doUpdate();
}

EffectHpRegen::EffectHpRegen(Entity *self, int duration, int regen_amount): Effect(self, duration),
                                                                            regen_amount(regen_amount) {}

bool EffectHpRegen::doUpdate() {
    int add_amount = self->combat_behavior->doEntityHealed(regen_amount);
    if (add_amount <= 0 || self != game.player) {return false;}
    game.gui->addMessage(TCODColor::green, "%s heal for %i hp", self->getName().c_str(), add_amount);

    return Effect::doUpdate();
}

int EffectHpRegen::getEffectID() {
    return effect_id::hp_regen;
}

EffectMpRegen::EffectMpRegen(Entity *self, int duration, int regen_amount): Effect(self, duration),
                                                                            regen_amount(regen_amount) {}

bool EffectMpRegen::doUpdate() {
    int add_amount = 0;
    game.gui->addMessage(TCODColor::green, "%s regenerate for %i mp", self->getName().c_str(), add_amount);

    return Effect::doUpdate();
}

int EffectMpRegen::getEffectID() {
    return effect_id::mp_regen;
}

EffectRage::EffectRage(Entity *self, int duration, float atk_boost): Effect(self, duration),
                                                                     atk_boost(atk_boost) {

    self->combat_behavior->setAtkBoost(self->combat_behavior->getAtkBoost() + atk_boost);
}

bool EffectRage::doUpdate() {
    if (duration - 1 == 0) {
        self->combat_behavior->setAtkBoost(self->combat_behavior->getAtkBoost() - atk_boost);
    }
    return Effect::doUpdate();
}

int EffectRage::getEffectID() {
    return effect_id::rage;
}

EffectProtection::EffectProtection(Entity *self, int duration, float def_boost): Effect(self, duration),
                                                                     def_boost(def_boost) {

    self->combat_behavior->setAtkBoost(self->combat_behavior->getAtkBoost() + def_boost);
}

bool EffectProtection::doUpdate() {
    if (duration - 1 == 0) {
        self->combat_behavior->setAtkBoost(self->combat_behavior->getAtkBoost() - def_boost);
    }
    return Effect::doUpdate();
}

int EffectProtection::getEffectID() {
    return effect_id::protection;
}