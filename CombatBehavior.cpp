#include "main.hpp"

CombatBehavior::CombatBehavior(Entity *self, int max_hp, int attack_point, int defense_point):
                               self(self), max_hp(max_hp), current_hp(max_hp), 
                               attack_point(attack_point), defense_point(defense_point), 
                               attack_boost(1), equipment_attack(0), 
                               defense_boost(1), equipment_defense(0) {}

void CombatBehavior::updateEquipmentAttribute() {
    equipment_attack = 0;
    equipment_defense = 0;
    self->equipment->getAllEquipmentAttribute();
}

int CombatBehavior::doEntityAttack() {
    return (attack_point + equipment_attack) * attack_boost;
}

int CombatBehavior::doEntityAttacked( int damage ) {
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
    self->setName(self->getName() + "'s Corpse");
    
    game.all_character.remove(self);
    game.all_corpse.push(self);
    
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
                                           int attack_point, int defense_point): 
                      CombatBehavior(self, max_hp, attack_point, defense_point) {}

void PlayerCombatBehavior::getEntityDead() {
    game.setStatus(status::DEFEAT);
    game.gui->addMessage(TCODColor::red, "You are dead");
    game.gui->addMessage(TCODColor::white, "[SPACE] Restart [ESC] Exit");
    CombatBehavior::getEntityDead();
}

EnemyCombatBehavior::EnemyCombatBehavior(Entity *self, int max_hp, 
                                         int attack_point, int defense_point):
                     CombatBehavior(self, max_hp, attack_point, defense_point) {}

void EnemyCombatBehavior::getEntityDead() {
    game.gui->addMessage(TCODColor::green, "%s is dead", self->getName().c_str());
    CombatBehavior::getEntityDead();
}

int CombatBehavior::getMaxHp() { return max_hp; }
int CombatBehavior::getCurrentHp() { return current_hp; }
int CombatBehavior::getAtkPoint() { return attack_point; }
int CombatBehavior::getDefPoint() { return defense_point; }
int CombatBehavior::getEquipmentAtkPoint() { return equipment_attack; }
int CombatBehavior::getEquipmentDefPoint() { return equipment_defense; }
float CombatBehavior::getAtkBoost() { return attack_boost; }
float CombatBehavior::getDefBoost() { return defense_boost; }
void CombatBehavior::setMaxHp( int input ) { max_hp = input; }
void CombatBehavior::setCurrentHp( int input ) { current_hp = input; }
void CombatBehavior::setAtkPoint( int input ) { attack_point = input; }
void CombatBehavior::setDefPoint( int input ) { defense_point = input; }
void CombatBehavior::setEquipmentAtkPoint( int input ) { equipment_attack = input; }
void CombatBehavior::setEquipmentDefPoint( int input ) { equipment_defense = input; }
void CombatBehavior::setAtkBoost( float input ) { attack_boost = input; }
void CombatBehavior::setDefBoost( float input ) { defense_boost = input; }