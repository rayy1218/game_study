#ifndef EFFECT_HPP
#define EFFECT_HPP

class Effect {
protected:
    Entity *self;
    int duration;
public:
    Effect(Entity *self, int duration);
    virtual ~Effect();
    
    virtual bool doUpdate();
};

class EffectBurn : public Effect{
private:
    int damage;
public:
    EffectBurn(Entity *self, int duration, int damage);
    bool doUpdate();
};

class EffectConfusion : public Effect{
private:
    Control *orig;
public:
    EffectConfusion(Entity *self, int duration);
    bool doUpdate();
};

class EffectPoison : public Effect {
private:
    int damage;
public:
    EffectPoison(Entity *self, int duration, int damage);
    bool doUpdate();
};

class EffectFrozen : public Effect {
private:
    Control *orig;
public:
    EffectFrozen(Entity *self, int duration);
    bool doUpdate();
};
#endif /* EFFECT_HPP */

