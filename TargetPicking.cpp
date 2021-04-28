#include "main.hpp"

TargetPicking::TargetPicking(Entity *self): self(self) {
}

TargetPicking::~TargetPicking() {
}

SelfTarget::SelfTarget(Entity *self): TargetPicking(self) {}

bool SelfTarget::doTargeting() {
    bool used = false;
    for (ItemPurpose *purpose : self->item_behavior->all_purpose) {
        used = purpose->doUse(game.player);
    }
    return used;
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
    
    bool used = false;
    for (ItemPurpose *purpose : self->item_behavior->all_purpose) {
        used = purpose->doUse(closest_enemy);
    }
    return used;
}

SelectAreaAllTarget::SelectAreaAllTarget(Entity *self, int radius, 
                                         bool include_self): TargetPicking(self),
                                         radius(radius), include_self(include_self) {}

bool SelectAreaAllTarget::doTargeting() {
    TCOD_mouse_t mouse;
    int x, y;
    
    game.gui->addMessage(TCODColor::yellow, "[LMC] to use [RMC] to cancel");
    while (game.keyboard.vk != TCODK_ESCAPE) {
        game.doRender();
        
        TCODSystem::checkForEvent(TCOD_EVENT_ANY, &game.keyboard, &mouse);
        
        if (!game.map->isInFov(mouse.cx, mouse.cy)) {continue;}
        
        for (int cx = mouse.cx - radius; cx <= mouse.cx + radius; cx++) {
            for (int cy = mouse.cy - radius; cy <= mouse.cy + radius; cy++) {
                if (!game.map->isInFov(mouse.cx, mouse.cy)) {}
                TCODConsole::root->setCharBackground(cx, cy, TCODConsole::root->getCharBackground(cx, cy) + TCODColor::desaturatedGreen);
            }
        }
        
        TCODConsole::root->flush();
        
        if (mouse.lbutton_pressed && game.map->isInFov(mouse.cx, mouse.cy)) {
            x = mouse.cx;
            y = mouse.cy;
            break;
        }
        else if (mouse.rbutton_pressed) {
            return false;
        }
    }
    
    bool any_target_existed = false;
    for (Entity *target : game.all_character) {
        if (target == game.player && !include_self) {continue;}
        if (target->getX() < x - radius || target->getX() > x + radius) {continue;}
        if (target->getY() < y - radius || target->getY() > y + radius) {continue;}
        
        for (ItemPurpose *purpose : self->item_behavior->all_purpose) {
            purpose->doUse(target);
        }
        any_target_existed = true;
    }
    
    if (!any_target_existed) {
        game.gui->addMessage(TCODColor::white, "There is no enemy");
    }
    
    return any_target_existed;
}