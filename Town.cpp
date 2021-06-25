#include "main.hpp"

enum menu_dict {
    cave,
    storage,
    general_shop,
    status,
    inventory
};

TownOption::TownOption(std::string name, std::string description): 
                       name(name), description(description) {}

std::string TownOption::getName() {return name;}
std::string TownOption::getDesc() {return description;}

Town::Town() {
    storage_room = new Container(NULL, 32768);
    getTownLocList();
    getTownSelfList();
}

Town::~Town() {
    delete storage_room;
    loc_list.clearAndDelete();
    self_list.clearAndDelete();
}

void Town::doRenderTownConsole() {
    static TCODConsole town_console(100, 50);
    
    int current_pointing = 0;
    
    while (true) {
        town_console.setDefaultBackground(TCODColor::darkGrey);
        town_console.setDefaultForeground(TCODColor::darkestGrey);
        town_console.clear();
        
        town_console.printFrame(0, 0, 100, 50, false, TCOD_BKGND_SET, "town");
        
        doRenderTownLocTab(&town_console, current_pointing);
        doRenderTownSelfTab(&town_console, current_pointing);
        doRenderTownDescTab(&town_console, current_pointing);
        doRenderTownUsageTab(&town_console);
        
        game.gui->doRender();
        TCODConsole::blit(&town_console, 0, 0, 100, 50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, true);
        
        if (game.keyboard.vk == TCODK_NONE || game.keyboard.vk == TCODK_ESCAPE) {
            doCloseWindow();
        }
        
        if (game.keyboard.vk == TCODK_UP) {
            if (current_pointing == 0) {
                current_pointing = loc_list.size() + self_list.size() - 1;
                continue;
            }
            current_pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            if (current_pointing == loc_list.size() + self_list.size() - 1) {
                current_pointing = 0;
                continue;
            }
            current_pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_ENTER) {
            switch (current_pointing) {
                case menu_dict::cave: return;
                
                case menu_dict::storage: doRenderStorage(); break;
                
                case menu_dict::general_shop: {
                    Shop *shop = new Shop;
                    shop->selling_item.push_back(item_dict::food);
                    shop->selling_item.push_back(item_dict::incense);
                    shop->selling_item.push_back(item_dict::molotov);
                    shop->selling_item.push_back(item_dict::potion_healing);
                    
                    shop->buying_item.push_back(item_dict::loot_goblin_braclet);
                    shop->buying_item.push_back(item_dict::loot_goblin_helmet);
                    shop->buying_item.push_back(item_dict::loot_goblin_necklace);
                    shop->buying_item.push_back(item_dict::harvest_meat);
                    ShopInterface *shop_interface = new ShopInterface(shop);
                    
                    shop_interface->doRenderShop();
                    delete shop;
                }
                
                case menu_dict::status: break;
                
                case menu_dict::inventory: {
                    while (game.keyboard.vk != TCODK_ESCAPE) {
                        game.doRender();
                        
                        Entity *to_use = game.gui->getSelectedItem(game.player->inventory);
                        if (to_use != NULL) {
                            if (game.keyboard.vk == TCODK_ENTER || game.keyboard.vk == TCODK_CHAR) {
                                if (!to_use->item_behavior->use(game.player)) {
                                    game.gui->addMessage(TCODColor::white, 
                                                         "Can't use item in town except equiping");
                                }
                                
                            }
                            if (game.keyboard.vk == TCODK_TAB) {
                                game.player->inventory->deleteItem(to_use);
                                game.gui->addMessage(TCODColor::white, "you dropped %s", 
                                                     to_use->getName().c_str());
                            }
                        }
                    }
                    
                    break;
                }
                
                default: break;
            }
        }
    }
}

void Town::doRenderTownLocTab(TCODConsole *town_console, int current_pointing) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(2, 2, 60, 28, false, TCOD_BKGND_SET, "location");
    
    int y = 4;
    town_console->setDefaultBackground(TCODColor::darkerGrey);
    if (current_pointing < loc_list.size()) {
        town_console->rect(4, y + current_pointing, 56, 1, false, TCOD_BKGND_SET);
    }
    
    for (TownOption *loc : loc_list) {
        town_console->printf(4, y, "%s",loc->getName().c_str());
        y++;
    }
}

void Town::doRenderTownSelfTab(TCODConsole *town_console, int current_pointing) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(2, 31, 60, 18, false, TCOD_BKGND_SET, "self");
    
    int y = 33;
    town_console->setDefaultBackground(TCODColor::darkerGrey);
    if (current_pointing >= loc_list.size()) {
        current_pointing -= loc_list.size();
        town_console->rect(4, y + current_pointing, 56, 1, false, TCOD_BKGND_SET);
    }
    
    for (TownOption *self : self_list) {
        town_console->printf(4, y, "%s", self->getName().c_str());
        y++;
    }
}

void Town::doRenderTownDescTab(TCODConsole *town_console, int current_pointing) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(65, 2, 32, 34, false, TCOD_BKGND_SET, "description");
    
    std::string desc;
    
    if (current_pointing < loc_list.size()) {
        TownOption *loc = loc_list.get(current_pointing);
        desc = loc->getDesc();
    }
    else {
        current_pointing -= loc_list.size();
        TownOption *self = self_list.get(current_pointing);
        desc = self->getDesc();
    }
    
    town_console->printRect(67, 4, 28, 30, "%s", desc.c_str());
}

void Town::doRenderTownUsageTab(TCODConsole *town_console) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(65, 37, 32, 12, false, TCOD_BKGND_SET, "usage");
    
    town_console->printRect(67, 39, 38, 8, 
                            "[UP/DOWN] to select\n"
                            "[ENTER] to go\n"
                            "[ESC] save and exit");
}

void Town::doRenderStorage() {
    TCODConsole storage_console(100, 50);
    const int PAGE_MAX_ITEM = 26;
    int storage_room_page = 1, inventory_page = 1, current_pointing = 0;
    bool pointing_storage_or_inventory = true;
    Entity *pointing_item = nullptr;
    
    while (true) {
        storage_console.setDefaultBackground(TCODColor::darkGrey);
        storage_console.setDefaultForeground(TCODColor::darkestGrey);
        storage_console.clear();
        
        int storage_page_item_num, inventory_page_item_num,
            max_storage_room_page = storage_room->getItemNum() / PAGE_MAX_ITEM + 1,
            max_inventory_page = game.player->inventory->getItemNum() / PAGE_MAX_ITEM + 1;
        
        storage_page_item_num = storage_room->getItemNum() % PAGE_MAX_ITEM;
        inventory_page_item_num = game.player->inventory->getItemNum() % PAGE_MAX_ITEM;
        
        int current_page_item_num = (pointing_storage_or_inventory) 
                                    ? storage_page_item_num
                                    : inventory_page_item_num;

        storage_console.printFrame(0, 0, 100, 50, false, TCOD_BKGND_SET, "storage room");
        
        if (!pointing_storage_or_inventory) {
            storage_console.setDefaultForeground(TCODColor::darkerGrey);
        }
        storage_console.printFrame(2, 2, 47, 33, false, TCOD_BKGND_SET, "storage inventory");
        {
            int total_item_num = storage_room->getItemNum(), alphabet = 'a'; 
            
            for (int i = 0; i < PAGE_MAX_ITEM; i++) {
                Entity *item = storage_room->getIndexItem(i + PAGE_MAX_ITEM * (storage_room_page - 1));
                if (item == nullptr) {break;}
                int item_qty = item->item_behavior->getQty();
                if (item_qty > 1) {
                    storage_console.printf(4, 4 + i, "%c) %s x %i", alphabet, 
                                           item->getName().c_str(), item_qty);
                }
                else {
                    storage_console.printf(4, 4 + i, "%c) %s", alphabet, 
                                           item->getName().c_str());
                }
                alphabet++;
            }
            
            storage_console.hline(3, 32, 45, TCOD_BKGND_SET);
            storage_console.printf(3, 33, "page : %i / %i", storage_room_page, 
                                   max_storage_room_page);
            
            if (pointing_storage_or_inventory) {
                if (storage_page_item_num != 0) {
                    storage_console.setDefaultBackground(TCODColor::darkerGrey);
                    storage_console.rect(4, 4 + current_pointing, 43, 1, false, TCOD_BKGND_SET);
                    storage_console.setDefaultBackground(TCODColor::darkGrey);
                    pointing_item = storage_room->getIndexItem(current_pointing + PAGE_MAX_ITEM * (storage_room_page - 1));
                } 
            }
        }
        storage_console.setDefaultForeground(TCODColor::darkestGrey);
        
        if (pointing_storage_or_inventory) {
            storage_console.setDefaultForeground(TCODColor::darkerGrey);
        }
        storage_console.printFrame(51, 2, 47, 33, false, TCOD_BKGND_SET, 
                                   "%s's inventory", game.player->getName().c_str());
        {
            int total_item_num = game.player->inventory->getItemNum(), alphabet = 'a';
            
            for (int i = 0; i < PAGE_MAX_ITEM; i++) {
                Entity *item = game.player->inventory->getIndexItem(i + PAGE_MAX_ITEM * (inventory_page - 1));
                if (item == nullptr) {break;}
                int item_qty = item->item_behavior->getQty();
                if (item->item_behavior->isStackable()) {
                    storage_console.printf(53, 4 + i, "%c) %s x %i", alphabet, 
                                           item->getName().c_str(), item_qty);
                }
                else {
                    storage_console.printf(53, 4 + i, "%c) %s", alphabet, 
                                           item->getName().c_str());
                }
                alphabet++;
            }
            
            storage_console.hline(52, 32, 45, TCOD_BKGND_SET);
            storage_console.printf(52, 33, "page : %i / %i", inventory_page, 
                                   max_inventory_page);
            storage_console.printf(74, 33, "weight : %.2f / %.2f",
                                   game.player->inventory->getCurrentWeight(),
                                   game.player->inventory->getMaxWeight());
            
            if (!pointing_storage_or_inventory) {
                if (inventory_page_item_num != 0) {
                    storage_console.setDefaultBackground(TCODColor::darkerGrey);
                    storage_console.rect(53, 4 + current_pointing, 43, 1, false, TCOD_BKGND_SET);
                    storage_console.setDefaultBackground(TCODColor::darkGrey);
                    pointing_item = game.player->inventory->getIndexItem(current_pointing +  PAGE_MAX_ITEM * (inventory_page - 1));
                }
            }
        }
        storage_console.setDefaultForeground(TCODColor::darkestGrey);
        
        storage_console.printFrame(2, 36, 70, 13, false, TCOD_BKGND_SET, "description");
        {
            if (pointing_item != nullptr) {
                storage_console.printRect(4, 38, 66, 9, "%s", 
                                          pointing_item->item_behavior->getDesc().c_str());
            }
        }
        
        storage_console.printFrame(74, 36, 24, 13, false, TCOD_BKGND_SET, "usage");
        storage_console.printRect(76, 38, 20, 9, "[ENTER] store / take\n"
                                                 "[SPACE] toggle storage\n"
                                                 "[UP/DOWN] move selection\n"
                                                 "[LEFT/RIGHT] move page\n"
                                                 "[ESC] return");
        
        TCODConsole::blit(&storage_console, 0, 0, 100, 50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();

        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_ESCAPE) {return;}

        if (game.keyboard.vk == TCODK_SPACE) {
            int opposite_page_item_num;
            opposite_page_item_num = (pointing_storage_or_inventory) 
                                     ? storage_page_item_num : inventory_page_item_num;

            if (current_pointing > opposite_page_item_num) {
                current_pointing = opposite_page_item_num;
            }
            
            pointing_storage_or_inventory = !pointing_storage_or_inventory;
        }

        if (game.keyboard.vk == TCODK_UP) {
            if (current_pointing == 0) {

                current_pointing = current_page_item_num - 1;
                continue;
            }

            current_pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            int current_page_item_num = (pointing_storage_or_inventory) 
                                        ? storage_page_item_num
                                        : inventory_page_item_num;
            
            if (current_pointing == current_page_item_num - 1) {
                current_pointing = 0;
                continue;
            }
            
            current_pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_LEFT) {
            if (pointing_storage_or_inventory) {
                if (storage_room_page == 1) {continue;}
                storage_room_page -= 1;
            }
            else {
                if (inventory_page == 1) {continue;}
                inventory_page -= 1;
            }
        } 
        
        if (game.keyboard.vk == TCODK_RIGHT) {
            if (pointing_storage_or_inventory) {
                if (storage_room_page == max_storage_room_page) {continue;}
                storage_room_page += 1;
            }
            else {
                if (inventory_page == max_inventory_page) {continue;}
                inventory_page += 1;
            }
        }
        
        if (game.keyboard.vk == TCODK_ENTER) {
            if (pointing_storage_or_inventory) {
                Entity *item = storage_room->getIndexItem(current_pointing);
                if (item == nullptr) {continue;}
                if (item->item_behavior->isStackable()) {
                    game.player->inventory->addItem(getItem(0, 0, 
                                                    item->item_behavior->getItemId()));
                }
                else {
                    game.player->inventory->addItem(item);
                }
                storage_room->removeItem(item);
                
            }
            else {
                Entity *item = game.player->inventory->getIndexItem(current_pointing);
                if (item == nullptr) {continue;}
                if (item->item_behavior->isStackable()) {
                    storage_room->addItem(getItem(0, 0, item->item_behavior->getItemId()));
                }
                else {
                    storage_room->addItem(item);
                }
                game.player->inventory->removeItem(item);
                
            }
        }
        
        if (game.keyboard.vk == TCODK_NONE) {doCloseWindow();}
    }
}

void Town::getTownLocList() {
    loc_list.push(new TownOption("street - rocky road",
                                 "lead to the way of the Living Cave, place of"
                                 "glory, wealth and death"));
    
    loc_list.push(new TownOption("home - storage room",
                                 "where you store you treasury"));
    
//    loc_list.push(new TownOption("home - workshop",
//                                 "where you craft item for exploration"));
//    
//    loc_list.push(new TownOption("street - merciful house",
//                                 "preist is here to provide quality healing for "
//                                 "aventurer but with price"));
//    
    loc_list.push(new TownOption("market - general shop",
                                 "exploration supplies like food, lantern oil "
                                 "sold here"));
//    
//    loc_list.push(new TownOption("market - armorsmith shop",
//                                 "armor, cloth and shield sold here"));
//    
//    loc_list.push(new TownOption("market - weaponsmith shop",
//                                 "melee and missle weapon sold here"));
//    
//    loc_list.push(new TownOption("market - enchanted accessory shop",
//                                 "accessory enchanted by priest or witch sold here"));
//    
//    loc_list.push(new TownOption("market - potion alchemist",
//                                 "flawed and low quality potion, alchemy material"
//                                 "and empty bottle sold here"));
//    
//    loc_list.push(new TownOption("market - witchcraft", 
//                                 "scroll, jar of sprite orb and ritual medium "
//                                 "sold here"));
//    
//    loc_list.push(new TownOption("guild - aventurer marketplace",
//                                 "all the loot from cave creature can only be "
//                                 "sell here for guild to collect \"cave entering fee\""));
//    
//    loc_list.push(new TownOption("guild - mission board",
//                                 "favourite place of bounty hunter, quest usually"
//                                 "consist of suppllies request, great cave creature "
//                                 "hunting and rare cave creature loot request"));
//    
//    loc_list.push(new TownOption("guild - chamber of master",
//                                 "voluntary assembly for master class aventurer "
//                                 "to provide skill lecture for guild member"));
}

void Town::getTownSelfList() {
    self_list.push(new TownOption("status", "check current status"));
    self_list.push(new TownOption("inventory", "check inventory"));
//    self_list.push(new TownOption("journal", "check journal for last aventure"));
//    self_list.push(new TownOption("skill", "check level for each skill you have"));
}