#include "main.hpp"

CorpseInteraction::CorpseInteraction(Entity *self,
                                     CorpseHarvest *can_harvest,
                                     CorpseLooting *can_looting,
                                     CorpseBurn *can_burn,
                                     CorpseRecall *can_recall):
                                     self(self),
                                     corpse_harvest(can_harvest),
                                     corpse_looting(can_looting), 
                                     corpse_burn(can_burn),
                                     corpse_recall(can_recall) {}

CorpseInteraction::~CorpseInteraction() {
    if (corpse_harvest) {delete corpse_harvest;}
    if (corpse_looting) {delete corpse_looting;}
    if (corpse_burn) {delete corpse_burn;}
    if (corpse_recall) {delete corpse_recall;}
}

bool CorpseInteraction::doRenderCorpseInteraction() {
    TCODConsole corpse_console(50, 16);
    
    int pointing = 0;
    while (true) {
        corpse_console.setDefaultBackground(TCODColor::darkGrey);
        corpse_console.setDefaultForeground(TCODColor::darkestGrey);
        corpse_console.clear();
        corpse_console.printFrame(0, 0, 50 ,15, false, TCOD_BKGND_SET, "corpse's interact");
        
        corpse_console.setDefaultBackground(TCODColor::darkerGrey);
        corpse_console.rect(2, 2 + pointing, 15, 1, false, TCOD_BKGND_SET);
        
        int y = 2, alphabet = 'a';
        if (corpse_looting != nullptr) {
            corpse_console.printf(2, y, "%c) Loot", alphabet);
            y++;
            alphabet++;
        }
        if (corpse_harvest != nullptr) {
            corpse_console.printf(2, y, "%c) Harvest", alphabet);
            y++;
            alphabet++;
        }
        if (corpse_burn != nullptr) {
            corpse_console.printf(2, y, "%c) Burn", alphabet);
            y++;
            alphabet++;
        }
        if (corpse_recall != nullptr) {
            corpse_console.printf(2, y, "%c) Recall", alphabet);
            y++;
            alphabet++;
        }
        
        game.doRender();
        TCODConsole::blit(&corpse_console, 0, 0, 50, 16, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
    
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_UP) {
            if (pointing == 0) {
                continue;
            }
            pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            if (pointing == y - 3) {
                continue;
            }
            pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_ENTER) {
            int i = 0;
            if (corpse_looting) {
                if (i == pointing) {
                    int index = corpse_looting->doRenderContainer();
                    if (index == -1) {return false;}
                    return corpse_looting->doUse(index);
                }
                i++;
            }
            if (corpse_harvest) {
                if (i == pointing) {
                    int index = corpse_harvest->doRenderContainer();
                    if (index == -1) {return false;}
                    return corpse_harvest->doUse(index);
                }
                i++;
            }
            if (corpse_burn) {
                if (i == pointing) {
                    
                    return corpse_burn->doUse(self);
                }
                i++;
            }
            if (corpse_recall) {
                if (i == pointing) {
                    return corpse_recall->doUse();
                }
                i++;
            }
        }
        
        if (game.keyboard.vk == TCODK_ESCAPE) {
            return false;
        }
        
        if (game.keyboard.vk == TCODK_NONE) {
            doCloseWindow();
        }
    }   
}

CorpseHarvest::CorpseHarvest(Entity *self): self(self) {}

CorpseHarvest::~CorpseHarvest() {
<<<<<<< HEAD
    harvest_item.clearAndDelete();
=======
    for (Entity *item : harvest_item) {
        delete item;
    }
>>>>>>> fight_focus
}

void CorpseInteraction::addHasvestableItem(Entity* item, int max_qty, int min_qty) {
    item->item_behavior->setQty(game.global_rng->getInt(min_qty, max_qty));
<<<<<<< HEAD
    corpse_harvest->harvest_item.push(item);
=======
    corpse_harvest->harvest_item.push_back(item);
>>>>>>> fight_focus
}

bool CorpseHarvest::doUse(int index) {
    Entity *item = nullptr;
<<<<<<< HEAD
    item = harvest_item.get(index);
=======
    item = harvest_item.at(index);
>>>>>>> fight_focus
    
    if (!game.player->inventory->addItem(item)) {
        game.gui->addMessage(TCODColor::red, "%s is too heavy to pick up", 
                             item->getName().c_str());
        return false;
    }
    
<<<<<<< HEAD
    harvest_item.remove(item);
=======
    harvest_item.erase(harvest_item.begin() + index);
>>>>>>> fight_focus
    
    game.gui->addMessage(TCODColor::green, "%s harvest %i %s",
                         game.player->getName().c_str(), item->item_behavior->getQty(),
                         item->getName().c_str());
<<<<<<< HEAD
    
    
=======

>>>>>>> fight_focus
    return true;
}

int CorpseHarvest::doRenderContainer() {
    if (harvest_item.size() == 0) {
        game.gui->addMessage(TCODColor::white, "%s has nothing to hasvest anymore",
                             self->getName().c_str());
        return -1;
    }
    
    TCODConsole container_console(50, 16);
    
    int pointing = 0;
    while (true) {
        container_console.setDefaultBackground(TCODColor::darkGrey);
        container_console.setDefaultForeground(TCODColor::darkestGrey);
        container_console.clear();
        container_console.printFrame(0, 0, 50, 16, false, TCOD_BKGND_SET, "harvestable");
        
        container_console.setDefaultBackground(TCODColor::darkerGrey);
<<<<<<< HEAD
        container_console.rect(2, 2, 46, 1, false, TCOD_BKGND_SET);
        
        int y, alphabet = 'a';
        for (y = 2; y <= 14; y++) {
            Entity *item = harvest_item.get(y - 2);
            if (item == nullptr) {continue;}
            container_console.printf(2, y, "%c) %s", alphabet, item->getName().c_str());
            y++;
            alphabet++;
=======
        container_console.rect(2, pointing + 2, 46, 1, false, TCOD_BKGND_SET);
        
        int y = 2, alphabet = 'a';
        for (Entity *item : harvest_item) {
            container_console.printf(2, y, "%c) %s", alphabet, item->getName().c_str());
            alphabet++;
            y++;
>>>>>>> fight_focus
        }
        
        game.doRender();
        TCODConsole::blit(&container_console, 0, 0, 50, 16, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_ENTER) {break;}
        
        if (game.keyboard.vk == TCODK_UP) {
            if (pointing == 0) {continue;}
            pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            if (pointing == y - 3) {continue;}
            pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_NONE) {doCloseWindow();}
        
        if (game.keyboard.vk == TCODK_ESCAPE) {
            return -1;
        }
    }
    
    return pointing;
}

CorpseLooting::CorpseLooting(Entity *self): self(self) {}

void CorpseInteraction::addLootableItem(Entity *item, int max_qty, int min_qty) {
    item->item_behavior->setQty(game.global_rng->getInt(min_qty, max_qty));
    self->inventory->addItem(item);
}

bool CorpseLooting::doUse(int index) {
    Entity *item = nullptr;
    item = self->inventory->getIndexItem(index);
    
    if (!game.player->inventory->addItem(item)) {
        game.gui->addMessage(TCODColor::red, "%s is too heavy to pick up", 
                             item->getName().c_str());
        return false;
    }
    
<<<<<<< HEAD
    self->inventory->removeItem(item, true);
=======
    self->inventory->removeItem(item, item->item_behavior->getQty());
>>>>>>> fight_focus
    
    game.gui->addMessage(TCODColor::green, "%s loot %i %s",
                         game.player->getName().c_str(), item->item_behavior->getQty(),
                         item->getName().c_str());
    
}

int CorpseLooting::doRenderContainer() {
    if (self->inventory->getItemNum() == 0) {
        game.gui->addMessage(TCODColor::white, "%s has nothing to loot anymore", 
                             self->getName().c_str());
        return -1;
    }
    
    TCODConsole container_console(50, 16);
    
    int pointing = 0;
    while (true) {
        container_console.setDefaultBackground(TCODColor::darkGrey);
        container_console.setDefaultForeground(TCODColor::darkestGrey);
        container_console.clear();
        container_console.printFrame(0, 0, 50, 16, false, TCOD_BKGND_SET, "harvestable");
        
        container_console.setDefaultBackground(TCODColor::darkerGrey);
<<<<<<< HEAD
        container_console.rect(2, 2, 46, 1, false, TCOD_BKGND_SET);
=======
        container_console.rect(2, pointing + 2, 46, 1, false, TCOD_BKGND_SET);
>>>>>>> fight_focus
        
        int y, alphabet = 'a';
        for (y = 2; y <= 14; y++) {
            Entity *item = self->inventory->getIndexItem(y - 2);
<<<<<<< HEAD
            if (item == nullptr) {continue;}
            container_console.printf(2, y, "%c) %s", alphabet, item->getName().c_str());
            y++;
=======
            if (item == nullptr) {break;}
            container_console.printf(2, y, "%c) %s", alphabet, item->getName().c_str());
>>>>>>> fight_focus
            alphabet++;
        }
        
        game.doRender();
        TCODConsole::blit(&container_console, 0, 0, 50, 16, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_ENTER) {break;}
        
        if (game.keyboard.vk == TCODK_UP) {
            if (pointing == 0) {continue;}
            pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            if (pointing == y - 3) {continue;}
            pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_NONE) {doCloseWindow();}
        
        if (game.keyboard.vk == TCODK_ESCAPE) {
            return -1;
        }
    }
    
    return pointing;
}

bool CorpseBurn::doUse(Entity *corpse) {
    game.gui->addMessage(TCODColor::white, "%s was burned to ashes", corpse->getName().c_str());
    game.all_corpse.remove(corpse);
    delete corpse;
    
    return true;
}

CorpseRecall::CorpseRecall(int success_chance): success_chance(success_chance) {}

bool CorpseRecall::doUse() {
    game.gui->addMessage(TCODColor::white, "recall success");
    //change control to ally control
}