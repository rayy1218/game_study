#include <cmath>

#include "main.hpp"

class Pathfinding : public ITCODPathCallback {
    public:
        float getWalkCost( int xFrom, int yFrom, int xTo, int yTo, void *userData ) const {
            if ( !game.map->canWalk( xTo, yTo ) ) {return 32767;}
            return sqrtf( powf( xFrom - xTo, 2 ) + powf( yFrom - yTo, 2 ) );
        }
        
};

Control::Control( Entity *self ): self( self ){}

Control::~Control() {
    self = nullptr;
}

void Control::doUpdate() {}

float Control::getDistanceTo(float x, float y) {
    return sqrtf( powf( self->getX() - x, 2 ) + powf( self->getY() - y, 2 ) );
}

PlayerControl::PlayerControl( Entity* self ): Control( self ) {}

void PlayerControl::doUpdate() {
    int dx = 0, dy = 0;
    
    switch (game.keyboard.c) {
        case 'k': dy--; break;
        case 'l': dx++; break;
        case 'j': dy++; break;
        case 'h': dx--; break;
        case 'u': dx++; dy--; break;
        case 'n': dx++; dy++; break;
        case 'b': dx--; dy++; break;
        case 'y': dx--; dy--; break;
            
        case '=': //IDLE
            game.setStatus( status::NEW_TURN );
            break;
    }
    
    switch(game.keyboard.vk) {
        case TCODK_KP8: dy--; break;
        case TCODK_KP6: dx++; break;
        case TCODK_KP2: dy++; break;
        case TCODK_KP4: dx--; break;
        case TCODK_KP9: dx++; dy--; break;
        case TCODK_KP3: dx++; dy++; break;
        case TCODK_KP1: dx--; dy++; break;
        case TCODK_KP7: dx--; dy--; break;
            
        case TCODK_KP5: //IDLE
            game.setStatus( status::NEW_TURN );
            break;
            
        case TCODK_CHAR: {
            int status = (handleCharInput(game.keyboard.c)) ? status::NEW_TURN
                                                            : status::IDLE;
            game.setStatus(status);
            break;
        }
        
        default:
            break;
    }
    if ( dy == 0 && dx == 0 ) {
        return;
    }
    else {
        int status = handleMoveOrAttack( dx, dy ) ? status::NEW_TURN 
                                                  : status::IDLE;
        game.setStatus( status );
        if ( status == status::NEW_TURN)  {
            game.map->getFov( self->getX(), self->getY() );
        }
    }
}

bool PlayerControl::handleMoveOrAttack(int dx, int dy) {
    int to_x = self->getX() + dx;
    int to_y = self->getY() + dy;
    
    if (game.map->down_hole->getX() == to_x && game.map->down_hole->getY() == to_y) {
        game.all_character.remove(game.player);
        game.all_character.clearAndDelete();
        game.all_character.push(game.player);
        game.all_corpse.clearAndDelete();
        game.all_item.clearAndDelete();
        game.all_prop.clearAndDelete();
        game.map->doGenerateMapCA();
        game.setFloorNum(game.getFloorNum() + 1);
        return true;
    }
    
    for ( Entity *character : game.all_character ) {
        if ( character->getX() == to_x && character->getY() == to_y ) {
            int attack_damage = self->combat_behavior->doEntityAttack();
            int damage_taken = character->combat_behavior->doEntityAttacked(attack_damage);
            
            
            game.gui->addMessage( (damage_taken == 0)? TCODColor::red : TCODColor::green,
                                  "You attack %s and dealt %i - %i damage"
                                  , character->getName().c_str(), attack_damage, attack_damage - damage_taken);
            
            if ( character->combat_behavior->checkEntityDead() ) {
                game.gui->addMessage( TCODColor::green, "%s is dead", 
                                      character->getName().c_str() );
                character->combat_behavior->getEntityDead();
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
                return to_use->item_behavior->use(self);
            }
            return false;
            break;
        }
            
        case 'p': {
            bool is_item_here = false;
            for (Entity *item : game.all_item) {
                if (self->getX() == item->getX() && self->getY() == item->getY()) {
                    is_item_here = true;
                    if (item->item_behavior->pick(self)) {
                        game.gui->addMessage(TCODColor::green, "You pick up %s", item->getName().c_str());
                        new_turn = true;
                    }
                    else {
                        game.gui->addMessage(TCODColor::red, "It is too heavy to pick up %s", item->getName().c_str());
                        new_turn = false;
                    }
                    break;
                }
            }
            
            if (!is_item_here) {
                game.gui->addMessage(TCODColor::white, "It is nothing here");
                new_turn = false;
            }
            
            break;
        }
            
        default:
            new_turn = false;
            break;
    }
    
    return new_turn;
}

EnemyControl::EnemyControl( Entity *self ): Control( self ), move_count(0) {
    path_to_player = new TCODPath( game.map->getWidth(), game.map->getHeight(), new Pathfinding, NULL );
}

EnemyControl::~EnemyControl() {
    delete path_to_player;
}

void EnemyControl::doUpdate() {
    path_to_player->compute( self->getX(), self->getY(), 
                            game.player->getX(), game.player->getY() );
    
    if (game.map->isInFov(self->getX(), self->getY())) {move_count = 3;}
    else {move_count--;}
    
    if (move_count > 0) { handleMoveOrAttack(); }
}

void EnemyControl::handleMoveOrAttack() {
    int x, y;
    path_to_player->walk( &x, &y, false );
    
    if ( getDistanceTo( game.player->getX(), game.player->getY() ) < 2 ) {
        int attack_damage = self->combat_behavior->doEntityAttack();
        int damage_taken = game.player->combat_behavior->doEntityAttacked( attack_damage );
        
        game.gui->addMessage( (damage_taken == 0)? TCODColor::green : TCODColor::red,
                              "%s attack you and dealt %i - %i damage"
                              , self->getName().c_str(), attack_damage, attack_damage - damage_taken);
        
        if ( game.player->combat_behavior->checkEntityDead() ) {
            game.gui->addMessage( TCODColor::red, "You are dead");
            game.player->combat_behavior->getEntityDead();
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
        }
    }
    
    self->move_behavior->doMoveEntity(to_x, to_y);
    if (self == game.player) {
            game.setStatus(status::NEW_TURN);
    }
}