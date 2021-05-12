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

class PurposeItemEquipment : public Purpose {
private:
    int attack_point, defense_point;
    float attack_boost, defense_boost;
public:
    PurposeItemEquipment(int attack_point, int defense_point, float attack_boost = 0, float defense_boost = 0);
    
    bool doUse(Entity *self);
};

class PurposeItemWeapon : public Purpose {
private:
    int attack_point;
    float speed;
public:
    PurposeItemWeapon(int attack_point, float speed);
    
    bool doUse(Entity *self);
};

#endif /* ITEMPURPOSE_HPP */

