#include "main.hpp"

TargetPicking::TargetPicking(Entity *self): self(self) {
}

TargetPicking::~TargetPicking() {
}

SelfTarget::SelfTarget(Entity *self): TargetPicking(self) {}

bool SelfTarget::doTargeting() {
    return self->item_behavior->purpose->doUse(game.player);
}

NearestTarget::NearestTarget(Entity *self, int range): TargetPicking(self), 
                                                             range(range) {}

bool NearestTarget::doTargeting() {
    Entity *closest_enemy = nullptr;
    float closest_range = 10000;
    for (Entity *target : game.all_character) {
        if (target == game.player) {continue;}
        if (target->getX() > game.player->getX() + range ||
            target->getX() < game.player->getX() - range ||
            target->getY() > game.player->getY() + range ||
            target->getY() < game.player->getY() - range) {continue;} 

        float range_to_target = game.player->control->getDistanceTo(target->getX(), target->getY());

        if (range_to_target < closest_range) {
            closest_enemy = target;
        }
    }
    if (closest_enemy == nullptr) {
        game.gui->addMessage(TCODColor::white, "No enemy around you");        
        return false;
    } 
    
    return self->item_behavior->purpose->doUse(closest_enemy);
}

SelectAreaAllTarget::SelectAreaAllTarget(Entity *self, int radius, 
                                         bool include_self): TargetPicking(self),
                                         radius(radius), include_self(include_self) {}

bool SelectAreaAllTarget::doTargeting() {
    int x, y, target_x = game.player->getX(), target_y = game.player->getY();
    TCOD_key_t keyboard;
    
    game.doRender();
    TCODConsole::root->flush();
    
    game.gui->addMessage(TCODColor::yellow, "[NUMPAD / Vi key] to move cursor, [NUMPAD5 / ,] to use, [ESC] to cancel");
    while (keyboard.vk != TCODK_ESCAPE) {
        game.doRender();
        
        for (int cx = target_x - radius; cx <= target_x + radius; cx++) {
            for (int cy = target_y - radius; cy <= target_y + radius; cy++) {
                TCODConsole::root->setCharBackground(cx, cy, TCODConsole::root->getCharBackground(cx, cy) + TCODColor::desaturatedGreen);
            }
        }
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &keyboard, NULL, false);
        
        int dx = 0, dy = 0;
        switch (keyboard.vk) {
            case TCODK_KP8: dy--; break;
            case TCODK_KP6: dx++; break;
            case TCODK_KP2: dy++; break;
            case TCODK_KP4: dx--; break;
            case TCODK_KP9: dx++; dy--; break;
            case TCODK_KP3: dx++; dy++; break;
            case TCODK_KP1: dx--; dy++; break;
            case TCODK_KP7: dx--; dy--; break;
        }
        
        switch (keyboard.c) {
            case 'j': dy--; break;
            case 'l': dx++; break;
            case 'k': dy++; break;
            case 'h': dx--; break;
            case 'u': dx++; dy--; break;
            case 'n': dx++; dy++; break;
            case 'b': dx--; dy++; break;
            case 'y': dx--; dy--; break;
        }
        
        if (game.map->isInFov(target_x + dx, target_y + dy)) {
            target_x += dx;
            target_y += dy;
        }
        
        if ((keyboard.vk == TCODK_KP5 || keyboard.c == ',')&& 
            game.map->isInFov(target_x, target_y)) {
            
            x = target_x;
            y = target_y;
            break;
        }
        else if (keyboard.vk == TCODK_ESCAPE) {
            return false;
        }
    }
    
    bool any_target_existed = false;
    for (Entity *target : game.all_character) {
        if (target == game.player && !include_self) {continue;}
        if (target->getX() < x - radius || target->getX() > x + radius) {continue;}
        if (target->getY() < y - radius || target->getY() > y + radius) {continue;}
        
        self->item_behavior->purpose->doUse(target);
        any_target_existed = true;
    }
    
    if (!any_target_existed) {
        game.gui->addMessage(TCODColor::white, "There is no enemy");
    }
    
    return any_target_existed;
}