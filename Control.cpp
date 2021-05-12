#include <cmath>

#include "main.hpp"

class Pathfinding : public ITCODPathCallback {
    public:
        float getWalkCost(int xFrom, int yFrom, int xTo, int yTo, void *userData) const {
            if (!game.map->canWalk(xTo, yTo) ) {return 32767;}
            return sqrtf(powf(xFrom - xTo, 2) + powf(yFrom - yTo, 2));
        }
        
};

Control::Control(Entity *self): self(self) {}

Control::~Control() {
    self = nullptr;
}

void Control::doUpdate() {}

float Control::getDistanceTo(float x, float y) {
    return sqrtf(powf(self->getX() - x, 2) + powf(self->getY() - y, 2));
}

PlayerControl::PlayerControl(Entity* self): Control(self) {}

void PlayerControl::doUpdate() {
    int dx = 0, dy = 0;
    
    switch (game.keyboard.c) { //Vi key Movement
        case 'j': dy--; break;
        case 'l': dx++; break;
        case 'k': dy++; break;
        case 'h': dx--; break;
        case 'u': dx++; dy--; break;
        case 'n': dx++; dy++; break;
        case 'b': dx--; dy++; break;
        case 'y': dx--; dy--; break;
            
        case ',': //IDLE
            game.setStatus(status::NEW_TURN);
            break;
    }
    
    switch (game.keyboard.vk) { //Numpad Movement
        case TCODK_KP8: dy--; break;
        case TCODK_KP6: dx++; break;
        case TCODK_KP2: dy++; break;
        case TCODK_KP4: dx--; break;
        case TCODK_KP9: dx++; dy--; break;
        case TCODK_KP3: dx++; dy++; break;
        case TCODK_KP1: dx--; dy++; break;
        case TCODK_KP7: dx--; dy--; break;
            
        case TCODK_KP5: //IDLE
            game.setStatus(status::NEW_TURN);
            break;
    }
    
    switch (game.keyboard.vk) { //Non Movement key
        case TCODK_CHAR: {
            int status = (handleCharInput(game.keyboard.c)) ? status::NEW_TURN
                                                            : status::IDLE;
            game.setStatus(status);
            break;
        }
        
        case TCODK_F11: {
            TCODConsole::root->setFullscreen(!TCODConsole::root->isFullscreen());
            break;
        }
        
        case TCODK_F1: {
            game.gui->doRenderTutorial();
            break;
        }
    }
    
    if ( dy == 0 && dx == 0 ) {return;}
    
    int status = handleMoveOrAttack(dx, dy) ? status::NEW_TURN : status::IDLE;
    game.setStatus(status);
    if (status == status::NEW_TURN)  {
        game.map->getFov(self->getX(), self->getY());
    }
}

bool PlayerControl::handleMoveOrAttack(int dx, int dy) {
    int to_x = self->getX() + dx;
    int to_y = self->getY() + dy;
    
    for (Entity *character : game.all_character) {
        if (character->getX() == to_x && character->getY() == to_y) {
            int hitting_count = (self->combat_behavior->getSpeed() / 
                                 character->combat_behavior->getSpeed());
            if (hitting_count == 0) {hitting_count = 1;}
            
            for (int i = 1; i <= hitting_count; i++) {
                int attack_damage = self->combat_behavior->doEntityAttack();
                int dice = game.global_rng->getInt(1, 10);
                
                if (dice <= 3) {
                    game.gui->addMessage(TCODColor::red, "%s missed its attack",
                                         self->getName().c_str());
                    return true;
                }
                if (dice == 10) {
                    attack_damage *= 1.5;
                }
                
                int damage_taken = character->combat_behavior->doEntityAttacked(attack_damage);
                
                game.gui->addMessage( (damage_taken == 0)? TCODColor::red : TCODColor::green,
                                      "You attack %s and dealt %i - %i damage"
                                      , character->getName().c_str(), attack_damage, attack_damage - damage_taken);

                if (character->combat_behavior->checkEntityDead()) {break;}
            }
            return true;
        }
    }
    
    return self->move_behavior->doMoveEntity( to_x, to_y );
}

bool PlayerControl::handleCharInput(int ascii) {
    bool new_turn = false;
    
    switch (ascii) {
        case 'i': {
            Entity *to_use = game.gui->getSelectedItem(self->inventory);
            if (to_use != NULL) {
                if (game.keyboard.vk == TCODK_ENTER || game.keyboard.vk == TCODK_CHAR) {
                    return to_use->item_behavior->use(self);
                }
                if (game.keyboard.vk == TCODK_TAB) {
                    self->inventory->dropItem(to_use);
                    game.gui->addMessage(TCODColor::white, "You dropped %s", to_use->getName().c_str());
                    
                    return true;
                }
            }
            return false;
            break;
        }
            
        case 'g': {
            bool is_item_here = false;
            Entity *toppest_item;
            for (Entity *item : game.all_item) {
                if (self->getX() == item->getX() && self->getY() == item->getY()) {
                    is_item_here = true;
                    toppest_item = item;
                }
            }
            
            if (!is_item_here) {
                game.gui->addMessage(TCODColor::white, "It is nothing here");
                new_turn = false;
                break;
            }
            
            if (toppest_item->item_behavior->pick(self)) {
                game.gui->addMessage(TCODColor::green, "You pick up %s", 
                                     toppest_item->getName().c_str());
                new_turn = true;
            }
            else {
                game.gui->addMessage(TCODColor::red, "%s is too heavy to pick", 
                                     toppest_item->getName().c_str());
                new_turn = false;
            }
            break;
        }
        
        case 't': {
            self->equipment->setHandUsing(!(self->equipment->isPrimaryHand()));
            new_turn = true;
            break;
        }
            
        default:
            new_turn = false;
            break;
    }
    
    return new_turn;
}

EnemyControl::EnemyControl(Entity *self): Control(self), move_count(0) {
    path_to_player = new TCODPath(game.map->getWidth(), game.map->getHeight(), 
                                  new Pathfinding, NULL);
}

EnemyControl::~EnemyControl() {
    delete path_to_player;
}

void EnemyControl::doUpdate() {
    path_to_player->compute(self->getX(), self->getY(), 
                            game.player->getX(), game.player->getY());
    
    if (game.map->isInFov(self->getX(), self->getY())) {move_count = 3;}
    else {move_count--;}
    
    if (move_count > 0) {handleMoveOrAttack();}
}

void EnemyControl::handleMoveOrAttack() {
    int x, y;
    path_to_player->walk(&x, &y, false);
    
    if (getDistanceTo(game.player->getX(), game.player->getY()) < 2 ) {
        int hitting_count = (self->combat_behavior->getSpeed() / 
                             game.player->combat_behavior->getSpeed());
        if (hitting_count == 0) {hitting_count = 1;}
        
        for (int i = 1; i <= hitting_count; i++) {
            int attack_damage = self->combat_behavior->doEntityAttack();
            int dice = game.global_rng->getInt(1, 10);
            
            if (dice <= 3) {
                game.gui->addMessage(TCODColor::green, "%s missed its attack",
                                     self->getName().c_str());
                return;
            }
            if (dice == 10) {
                attack_damage *= 1.5;
            }
                
            int damage_taken = game.player->combat_behavior->doEntityAttacked(attack_damage);

            game.gui->addMessage((damage_taken == 0)? TCODColor::green : TCODColor::red,
                                 "%s attack you and dealt %i - %i damage",
                                 self->getName().c_str(), attack_damage, attack_damage - damage_taken);

            if (game.player->combat_behavior->checkEntityDead()) {break;} 
        }
        
        return;
    }
    
    self->move_behavior->doMoveEntity( x, y );
}

CorpseControl::CorpseControl(): Control(nullptr) {}

void CorpseControl::doUpdate() {
    return;
}

ConfusedControl::ConfusedControl(Entity *self): Control(self) {}

void ConfusedControl::doUpdate() {
    int dy = 0, dx = 0;
    
    TCODRandom *rng = TCODRandom::getInstance();
    switch (rng->getInt(1, 8)) {
        case 1: dy--; break;
        case 2: dx++; break;
        case 3: dy++; break;
        case 4: dx--; break;
        case 5: dx++; dy--; break;
        case 6: dx++; dy++; break;
        case 7: dx--; dy++; break;
        case 8: dx--; dy--; break;
    }
    
    handleMoveOrAttack(self->getX() + dx, self->getY() + dy);
}

void ConfusedControl::handleMoveOrAttack(int to_x, int to_y) {
    for (Entity *character : game.all_character) {
        if (character->getX() == to_x && character->getY() == to_y) {
            int damage = self->combat_behavior->doEntityAttack();
            int damage_dealt = character->combat_behavior->doEntityAttacked(damage);
            game.gui->addMessage(TCODColor::purple, "%s attack %s and dealt %i - %i damage", 
                                 self->getName().c_str(), character->getName().c_str(), damage, damage - damage_dealt);
            
            character->combat_behavior->checkEntityDead();
        }
    }
    
    self->move_behavior->doMoveEntity(to_x, to_y);
    if (self == game.player) {
        game.setStatus(status::NEW_TURN);
    }
}

StunnedControl::StunnedControl(Entity* self): Control(self) {}

void StunnedControl::doUpdate() {
    if (self == game.player) {
        game.setStatus(status::NEW_TURN);
    }
}

StepTriggerControl::StepTriggerControl(Entity *self): Control(self) {}

void StepTriggerControl::doUpdate() {}

Entity* StepTriggerControl::getCharacterStepOn() {
    Entity* step_by = nullptr;
    for (Entity *character : game.all_character) {
        if (character->getX() == self->getX() && character->getY() == self->getY()) {
            step_by = character;
        }
    }
    return step_by;
}

UpStairControl::UpStairControl(Entity *self): StepTriggerControl(self) {}

void UpStairControl::doUpdate() {
    Entity *step_by = getCharacterStepOn();
    if (step_by != game.player || step_by == nullptr) {return;}
    
    if (game.getFloorNum() == 1) {
        game.gui->addMessage(TCODColor::yellow, "The Sun is a deadly laser");
        game.player->combat_behavior->setCurrentHp(0);
        game.player->combat_behavior->getEntityDead();
        return;
    }
    
    game.setFloorNum(game.getFloorNum() - 1);
    game.doFloorTravel();
}

DownStairControl::DownStairControl(Entity *self): StepTriggerControl(self) {}

void DownStairControl::doUpdate() {
    Entity *step_by = getCharacterStepOn();
    if (step_by != game.player || step_by == nullptr) {return;}
    
    if (game.getFloorNum() == 10) {
        game.gui->addMessage(TCODColor::yellow, "The stair collapsed, bury you under rock");
        game.player->combat_behavior->setCurrentHp(0);
        game.player->combat_behavior->getEntityDead();
        return;
    }
    
    game.setFloorNum(game.getFloorNum() + 1);
    game.doFloorTravel();
}

HoleControl::HoleControl(Entity *self): StepTriggerControl(self) {}

void HoleControl::doUpdate() {
    Entity *step_by = getCharacterStepOn();
    if (step_by != game.player || step_by == nullptr) {return;}
    
    int fall_floor_num = game.global_rng->getInt(1, 3);
    
    if (game.getFloorNum() + fall_floor_num > 10) {
        game.gui->addMessage(TCODColor::yellow, "You fall into the hole that you will never find the end");
        game.player->combat_behavior->setCurrentHp(0);
        game.player->combat_behavior->getEntityDead();
        return;
    }
    
    int damage = game.global_rng->getInt(5, 10) * fall_floor_num;
    game.setFloorNum(game.getFloorNum() + fall_floor_num);
    int damage_dealt = game.player->combat_behavior->doEntityAttacked(damage);
    game.gui->addMessage(TCODColor::red, "You fall into hole and take %i - %i damage",
                         damage, damage - damage_dealt);
    
    if (game.player->combat_behavior->checkEntityDead()) {return;}
    
    game.doFloorTravel();
}

TrapControl::TrapControl(Entity *self, Purpose *purpose): StepTriggerControl(self),
                                                          purpose(purpose) {
    
    orig_char = self->getAsciiChar();
    orig_color = self->getAsciiColor();
    
    if (game.global_rng->getInt(1, 10) > 2) {
        self->setAsciiColor(TCODColor::white);
        self->setAsciiChar('.');
    } 
}

void TrapControl::doUpdate() {
    if (game.player->control->getDistanceTo(self->getX(), self->getY()) < 2) {
        if (game.global_rng->getInt(1, 10) == 1 && self->getAsciiChar() == '.') {
            self->setAsciiColor(orig_color);
            self->setAsciiChar(orig_char);
            game.gui->addMessage(TCODColor::white, "%s is appeared", self->getName().c_str());
        }
    }
    
    Entity *step_by = getCharacterStepOn();
    if (step_by == nullptr) {return;}
    
    TCODColor message_color = (step_by == game.player) ? TCODColor::red : TCODColor::green;
    game.gui->addMessage(message_color, "%s step on %s", step_by->getName().c_str(), 
                                                         self->getName().c_str());
    purpose->doUse(step_by);
    
    game.all_prop.remove(self);
    
    delete self;
}