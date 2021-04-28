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

#endif /* ITEMPURPOSE_HPP */

