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
    game.gui->addMessage(message_color, "%s is burned for %i - %i damage", self->getName().c_str(), damage - burn_damage, burn_damage);
    return Effect::doUpdate();
}

EffectConfusion::EffectConfusion(Entity *self, int duration): Effect(self, duration),
                                                              orig(self->control) {

    self->control = new ConfusedControl(self);
    TCODColor message_color = (self == game.player) ? TCODColor::red : TCODColor::green;
    game.gui->addMessage(message_color, "%s is confused", self->getName().c_str());
}

bool EffectConfusion::doUpdate() {
    if (duration - 1 <= 0) {
        TCODColor message_color = (self == game.player) ? TCODColor::green : TCODColor::red;
        game.gui->addMessage(message_color, "%s is not longer confused", self->getName().c_str());
        Control *temp = self->control;
        self->control = orig;
        delete temp;
    }
    return Effect::doUpdate();
}