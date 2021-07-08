#ifndef ITEMPURPOSE_HPP
#define ITEMPURPOSE_HPP

class Entity;

class Purpose {
public:
    Purpose();
    virtual ~Purpose();
    
    virtual bool doUse(Entity *target) = 0;
};

class PurposeHeal : public Purpose {
private:
    int heal_amount;
public:
    PurposeHeal(int heal_amount);
    
    bool doUse(Entity *target);
};

class PurposeDamage : public Purpose {
private:
    int damage;
public:
    PurposeDamage(int damage);

    bool doUse(Entity *target);
};

class PurposeDirectDamage : public Purpose {
private:
    int damage;
public:
    PurposeDirectDamage(int damage);
    bool doUse(Entity *target);
};

class PurposeEffectBurn : public Purpose {
private:
    int damage, duration;
public:
    PurposeEffectBurn(int damage, int duration);
    
    bool doUse(Entity *target);
};

class PurposeEffectConfusion : public Purpose {
private:
    int duration;
public:
    PurposeEffectConfusion(int duration);
    
    bool doUse(Entity *target);
};

class PurposeEffectFrozen : public Purpose {
private:
    int duration;
public:
    PurposeEffectFrozen(int duration);
    
    bool doUse(Entity *target);
};

class PurposeEffectPoison : public Purpose {
private:
    int damage, duration;
public:
    PurposeEffectPoison(int damage, int duration);
    
    bool doUse(Entity *target);
};

class PurposeItemFood : public Purpose {
private:
    int hunger_point;
public:
    PurposeItemFood(int hunger_point);
    
    bool doUse(Entity *target);
};

#endif /* ITEMPURPOSE_HPP */

