#ifndef ITEMPURPOSE_HPP
#define ITEMPURPOSE_HPP

class Entity;

class ItemPurpose {
public:
    ItemPurpose();
    virtual ~ItemPurpose();
    
    virtual bool doUse(Entity *target) = 0;
};

class ItemHeal : public ItemPurpose {
private:
    int heal_amount;
public:
    ItemHeal(int heal_amount);
    
    bool doUse(Entity *target);
};

class ItemDamage : public ItemPurpose {
private:
    int damage;
public:
    ItemDamage(int damage);

    bool doUse(Entity *target);
};

class ItemEffectBurn : public ItemPurpose {
private:
    int damage, duration;
public:
    ItemEffectBurn(int damage, int duration);
    
    bool doUse(Entity *target);
};

class ItemEffectConfusion : public ItemPurpose {
private:
    int duration;
public:
    ItemEffectConfusion(int duration);
    
    bool doUse(Entity *target);
};

class ItemEquipment : public ItemPurpose {
private:
    int attack_point, defense_point;
    float attack_boost, defense_boost;
public:
    ItemEquipment(int attack_point, int defense_point, float attack_boost = 0, float defense_boost = 0);
    
    bool doUse(Entity *self);
};

class ItemWeapon : public ItemPurpose {
private:
    int attack_point;
    float speed;
public:
    ItemWeapon(int attack_point, float speed);
    
    bool doUse(Entity *self);
};

#endif /* ITEMPURPOSE_HPP */

