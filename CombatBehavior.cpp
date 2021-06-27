#include "main.hpp"

CombatBehavior::CombatBehavior(Entity *self, int max_hp, int attack_point, 
                               int defense_point, int accuracy, int agility):
                               self(self), max_hp(max_hp), current_hp(max_hp), 
                               attack_point(attack_point), defense_point(defense_point), 
                               attack_boost(1), equipment_attack(0), 
                               defense_boost(1), equipment_defense(0), equipment_agility(0),
                               accuracy(accuracy), agility(agility) {}

void CombatBehavior::updateEquipmentAttribute(bool is_primary) {
    equipment_defense = 0;
    equipment_attack = 0;
    equipment_agility = 0;
    self->equipment->getAllEquipmentAttribute(is_primary);
}

int CombatBehavior::doEntityAttack() {
    return (attack_point + equipment_attack) * attack_boost;
}

int CombatBehavior::doEntityAttacked(int damage) {
    int defense = ( defense_point + equipment_defense ) * defense_boost;
    damage -= defense;
    if (damage <= 0) {damage = 0;}
    
    current_hp -= damage;
    
    return damage;
}

int CombatBehavior::doEntityHealed(int heal_amount) {
    current_hp += heal_amount;
    if (current_hp > max_hp) {
        heal_amount = max_hp - (current_hp - heal_amount);
        current_hp = max_hp;
    }
    return heal_amount;
}

void CombatBehavior::getEntityDead() {
    self->setAsciiChar('%');
    self->setAsciiColor(TCODColor::darkRed);
    self->setName(self->getName() + "'s corpse");
    
    game.all_character.remove(self);
    game.all_corpse.push(self);
    
    game.gui->doResetFocusedEnemy();
    
    delete self->control;
    self->control = new CorpseControl;
}

bool CombatBehavior::checkEntityDead() {
    if (current_hp <= 0) {
        getEntityDead();
    }
    return (current_hp <= 0);
}

PlayerCombatBehavior::PlayerCombatBehavior(Entity *self, int max_hp, 
                                           int attack_point, int defense_point, 
                                           int accuracy, int agility): 
                      CombatBehavior(self, max_hp, attack_point, defense_point, 
                                     accuracy, agility) {}

void PlayerCombatBehavior::getEntityDead() {
    game.setStatus(status::DEFEAT);
    CombatBehavior::getEntityDead();
}

int PlayerCombatBehavior::doEntityAttack(bool is_primary) {
    return (attack_point + equipment_attack) * attack_boost;
}

EnemyCombatBehavior::EnemyCombatBehavior(Entity *self, int max_hp, 
                                         int attack_point, int defense_point, 
                                         int accuracy, int agility):
                     CombatBehavior(self, max_hp, attack_point, defense_point, 
                                    accuracy, agility) {}

void EnemyCombatBehavior::getEntityDead() {
    game.gui->addMessage(TCODColor::green, "%s is dead", self->getName().c_str());
    CombatBehavior::getEntityDead();
}

int CombatBehavior::getMaxHp() {return max_hp;}
int CombatBehavior::getCurrentHp() {return current_hp;}
int CombatBehavior::getAtkPoint() {return attack_point;}
int CombatBehavior::getDefPoint() {return defense_point;}
int CombatBehavior::getEquipmentAtkPoint() {return equipment_attack;}
int CombatBehavior::getEquipmentDefPoint() {return equipment_defense;}
int CombatBehavior::getEquipmentAgPoint() {return equipment_agility;}
float CombatBehavior::getAtkBoost() {return attack_boost;}
float CombatBehavior::getDefBoost() {return defense_boost;}
int CombatBehavior::getAccuracy() {return accuracy;}
int CombatBehavior::getAgility() {return agility;}
void CombatBehavior::setMaxHp(int input) {max_hp = input;}
void CombatBehavior::setCurrentHp(int input) {current_hp = input;}
void CombatBehavior::setAtkPoint(int input) {attack_point = input;}
void CombatBehavior::setDefPoint(int input) {defense_point = input;}
void CombatBehavior::setEquipmentAtkPoint(int input) {equipment_attack = input;}
void CombatBehavior::setEquipmentDefPoint(int input) {equipment_defense = input;}
void CombatBehavior::setEquipmentAgPoint(int input) {equipment_agility = input;}
void CombatBehavior::setAtkBoost(float input) {attack_boost = input;}
void CombatBehavior::setDefBoost(float input) {defense_boost = input;}
void CombatBehavior::setAccuracy(int input) {accuracy = input;}
void CombatBehavior::setAgility(int input) {agility = input;}

int CombatBehavior::getTotalAtk() {
    return int((getAtkPoint() + getEquipmentAtkPoint()) * getAtkBoost());
}

int CombatBehavior::getTotalDef() {
    return int((getDefPoint() + getEquipmentDefPoint()) * getDefBoost());
}

int CombatBehavior::getTotalAg() {
    return int(getAgility() + getEquipmentAgPoint());
}