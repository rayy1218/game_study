#ifndef ITEMPURPOSE_HPP
#define ITEMPURPOSE_HPP

class Entity;

class Purpose {
public:
    Purpose();
    virtual ~Purpose();
    
    virtual bool doUse(Entity *target) = 0;
};

class PurposeHpRestore : public Purpose {
private:
    int restore_amount;
public:
    PurposeHpRestore(int restore_amount);
    
    bool doUse(Entity *target);
};

class PurposeMpRestore : public Purpose {
private:
    int restore_amount;
public:
    PurposeMpRestore(int restore_amount);

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

class PurposeTensionRestore : public Purpose {
private:
    int restore_amount;
public:
    PurposeTensionRestore(int restore_amount);

    bool doUse(Entity *target);
};

class PurposeEffectHpRegen : public Purpose {
private:
    int regen_amount, duration;
public:
    PurposeEffectHpRegen(int regen_amount, int duration);

    bool doUse(Entity *target);
};

class PurposeEffectMpRegen : public Purpose {
private:
    int regen_amount, duration ;
public:
    PurposeEffectMpRegen(int duration, int regen_amount);

    bool doUse(Entity *target);
};

class PurposeEffectRage : public Purpose {
private:
    int duration;
    float atk_boost;
public:
    PurposeEffectRage(float atk_boost, int duration);

    bool doUse(Entity *target);
};

class PurposeEffectProtection: public Purpose {
private:
    int duration;
    float def_boost;
public:
    PurposeEffectProtection(float def_boost, int duration);

    bool doUse(Entity *target);
};

class PurposeUnlockCasting: public Purpose {
    int casting;
public:
    PurposeUnlockCasting(int casting);

    bool doUse(Entity *target);
};

#endif /* ITEMPURPOSE_HPP */

