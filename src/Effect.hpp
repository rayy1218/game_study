#ifndef EFFECT_HPP
#define EFFECT_HPP

enum effect_id {
    none, burn, confusion, poison, frozen, hp_regen, mp_regen, rage, protection
};

class Effect {
protected:
    Entity *self;
    int duration;
public:
    Effect(Entity *self, int duration);
    virtual ~Effect();
    
    virtual bool doUpdate();
    virtual int getEffectID();
};

class EffectBurn : public Effect{
private:
    int damage;
public:
    EffectBurn(Entity *self, int duration, int damage);
    bool doUpdate();
    int getEffectID();
};

class EffectConfusion : public Effect{
private:
    Control *orig;
public:
    EffectConfusion(Entity *self, int duration);
    bool doUpdate();
    int getEffectID();
};

class EffectPoison : public Effect {
private:
    int damage;
public:
    EffectPoison(Entity *self, int duration, int damage);
    bool doUpdate();
    int getEffectID();
};

class EffectFrozen : public Effect {
private:
    Control *orig;
public:
    EffectFrozen(Entity *self, int duration);
    bool doUpdate();
    int getEffectID();
};

class EffectHpRegen : public Effect {
private:
    int regen_amount;
public:
    EffectHpRegen(Entity *self, int duration, int regen_amount);
    bool doUpdate();
    int getEffectID();
};

class EffectMpRegen : public Effect {
private:
    int regen_amount;
public:
    EffectMpRegen(Entity *self, int duration, int regen_amount);
    bool doUpdate();
    int getEffectID();
};

class EffectRage : public Effect {
private:
    float atk_boost;
public:
    EffectRage(Entity *self, int duration, float atk_boost);
    bool doUpdate();
    int getEffectID();
};

class EffectProtection : public Effect {
private:
    float def_boost;
public:
    EffectProtection(Entity *self, int duration, float def_boost);
    bool doUpdate();
    int getEffectID();
};
#endif /* EFFECT_HPP */

