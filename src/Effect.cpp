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

EffectFrozen::EffectFrozen(Entity *self, int duration): Effect(self, duration), orig(self->control) {
    self->control = new StunnedControl(self);
    TCODColor message_color = (self == game.player) ? TCODColor::red : TCODColor::green;
    if (game.map->isInFov(self->getX(), self->getY())) {
        game.gui->addMessage(message_color, "%s is frezzed", self->getName().c_str());
    }
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