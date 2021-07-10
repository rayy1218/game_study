#ifndef COMBATBEHAVIOR_HPP
#define COMBATBEHAVIOR_HPP

class Equipment;

class CombatBehavior {
protected:
    //Attribute
    Entity *self;
    int max_hp, current_hp, attack_point, defense_point, accuracy, agility,
        equipment_attack, equipment_defense, equipment_agility,
        effect_attack, effect_defense, effect_agility;
    float attack_boost, defense_boost, agility_boost;
public:
    //Constructor
    CombatBehavior(Entity *self, int max_hp, int attack_point, int defense_point, 
                   int accuracy, int agility);
    
    //Method
    virtual void getEntityDead();
    bool checkEntityDead();
    int doEntityAttack();
    int doEntityAttacked(int damage);
    int doEntityHealed(int heal_amount);
    void updateEquipmentAttribute(bool is_primary);
    
    //Accessor
    int getMaxHp();
    int getCurrentHp();
    int getAtkPoint();
    int getDefPoint();
    int getAccuracy();
    int getAgility();
    int getEquipmentAtkPoint();
    int getEquipmentDefPoint();
    int getEquipmentAgPoint();
    int getEffectAtkPoint();
    int getEffectDefPoint();
    int getEffectAgPoint();
    float getAtkBoost();
    float getDefBoost();
    float getAgBoost();
    int getTotalAtk();
    int getTotalDef();
    int getTotalAg();
    void setMaxHp(int input);
    void setCurrentHp(int input);
    void setAtkPoint(int input);
    void setDefPoint(int input);
    void setAccuracy(int input);
    void setAgility(int input);
    void setEquipmentAtkPoint(int input);
    void setEquipmentDefPoint(int input);
    void setEquipmentAgPoint(int input);
    void setEffectAtkPoint(int input);
    void setEffectDefPoint(int input);
    void setEffectAgPoint(int input);
    void setAtkBoost(float input);
    void setDefBoost(float input);
    void setAgBoost(float input);
};

class PlayerCombatBehavior : public CombatBehavior {
public:
    PlayerCombatBehavior(Entity *self, int max_hp, int attack_point, int defense_point, 
                         int accuracy, int agility);
    int doEntityAttack(bool is_primary);
    void getEntityDead();
};

class EnemyCombatBehavior : public CombatBehavior {
public:
    EnemyCombatBehavior(Entity *self, int max_hp, int attack_point, int defense_point, 
                        int accuracy, int agility);
    void getEntityDead();
};

#endif /* COMBATBEHAVIOR_HPP */

