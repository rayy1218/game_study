#ifndef COMBATBEHAVIOR_HPP
#define COMBATBEHAVIOR_HPP

class Equipment;

class CombatBehavior {
protected:
    //Attribute
    Entity *self;
    int max_hp, current_hp, attack_point, defense_point, equipment_attack,  
        equipment_defense;
    Equipment *self_equipment;
    float attack_boost, defense_boost;
public:
    //Constructor
    CombatBehavior( Entity *self, int max_hp, int attack_point, int defense_point );
    
    //Method
    virtual void getEntityDead();
    bool checkEntityDead();
    int doEntityAttack();
    int doEntityAttacked( int damage );
    int doEntityHealed( int heal_amount );
    void updateEquipmentAttribute();
    
    //Accessor
    int getMaxHp();
    int getCurrentHp();
    int getAtkPoint();
    int getDefPoint();
    int getEquipmentAtkPoint();
    int getEquipmentDefPoint();
    float getAtkBoost();
    float getDefBoost();
    void setMaxHp( int input );
    void setCurrentHp( int input );
    void setAtkPoint( int input );
    void setDefPoint( int input );
    void setEquipmentAtkPoint( int input );
    void setEquipmentDefPoint( int input );
    void setAtkBoost( float input );
    void setDefBoost( float input );
};

class PlayerCombatBehavior : public CombatBehavior {
public:
    PlayerCombatBehavior( Entity *self, int max_hp, int attack_point, int defense_point );
    void getEntityDead();
};

class EnemyCombatBehavior : public CombatBehavior {
public:
    EnemyCombatBehavior( Entity *self, int max_hp, int attack_point, int defense_point );
    void getEntityDead();
};

#endif /* COMBATBEHAVIOR_HPP */

