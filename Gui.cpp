#include <cstdarg>

#include "main.hpp"

static const int LOG_HEIGHT = 9;
static const int MAX_ITEM_SHOWN = 16;
static int page = 1;
static int backpack_pointing = 0;
static int console_width = TCODConsole::root->getWidth() / 2;

Message::Message(std::string message, TCODColor text_color): message( message ),
                                                             text_color( text_color ) {}

Gui::Gui() {
    log_console = new TCODConsole( 100, 10 );
    infomation_console = new TCODConsole(30 , 60);
}

Gui::~Gui() {
    
}

void Gui::doRender() {
    log_console->setDefaultBackground( TCODColor::black );
    log_console->clear();
    
    doCreateInfomationConsole();
    
    int y = 1;
    for ( Message *message : message_log ) {
        log_console->setDefaultForeground( message->text_color );
        log_console->printf( 0, y, message->message.c_str() );
        y++;
    }
    
    addPlayerNowStandOn(0, 0);
    
    TCODConsole::root->blit(log_console, 0, 0, 100, 10, TCODConsole::root, 0, game.map->getHeight());
    TCODConsole::root->blit(infomation_console, 0, 0, 30, 60, TCODConsole::root, 100, 0);
}

void Gui::addBar( int x, int y, int w, int h, TCODColor empty_color, TCODColor full_color, 
                  float max_value, float current_value, std::string bar_title, TCODColor title_color ) {
    
    infomation_console->setDefaultBackground( empty_color );
    infomation_console->rect( x, y, w, h, true, TCOD_BKGND_SET );
    
    int bar_len = ( current_value / max_value ) * w;
    
    infomation_console->setDefaultBackground( full_color );
    infomation_console->rect( x ,y, bar_len, h, true, TCOD_BKGND_SET );
    
    infomation_console->setDefaultForeground( title_color );
    infomation_console->printf( x, y, "%s: %.0f / %.0f", bar_title.c_str(), current_value, max_value );
}

void Gui::doCreateInfomationConsole() {
    infomation_console->setDefaultBackground(TCODColor::black);
    infomation_console->clear();
    
    infomation_console->setDefaultForeground(TCODColor::white);
    infomation_console->printf(1, 1, "Floor: %i", game.getFloorNum());
    infomation_console->printf(1, 3, "%s ", game.player->getName());
    infomation_console->printf(1, 4, "Attack: %i * %.2f", game.player->combat_behavior->getAtkPoint() +
                                                          game.player->combat_behavior->getEquipmentAtkPoint(), 
                                                          game.player->combat_behavior->getAtkBoost());
    infomation_console->printf(1, 5, "Defense: %i * %.2f", game.player->combat_behavior->getDefPoint() +
                                                           game.player->combat_behavior->getEquipmentDefPoint(), 
                                                           game.player->combat_behavior->getDefBoost());
    
    std::string hand_using = (game.player->equipment->isPrimaryHand())? "Primary Hand" : "Secondary Hand";
    infomation_console->printf(1, 6, "Using: %s", hand_using.c_str());
    
    addBar(1, 8, 28, 1, TCODColor::lighterRed, TCODColor::red, 
           game.player->combat_behavior->getMaxHp(), 
           game.player->combat_behavior->getCurrentHp(), "Health", TCODColor::white);
    addBar(1, 9, 28, 1, TCODColor::lighterBlue, TCODColor::han, 50, 25, "Mana", TCODColor::white);
    addBar(1, 10, 28, 1, TCODColor::lighterViolet, TCODColor::violet, 
           game.player_stats->tension->getMaxTension(), 
           game.player_stats->tension->getCurrentTension(), "Tension", TCODColor::white);
    addBar(1, 11, 28, 1, TCODColor::lighterSepia, TCODColor::sepia, 
           game.player_stats->hunger->getMaxHungerPoint(), 
           game.player_stats->hunger->getCurrentHungerPoint(), "Hunger", TCODColor::white );
}

void Gui::addMessage(TCODColor text_color, const char *fmt, ...) {
    va_list ap;
    char buffer[128];
    va_start(ap, fmt);
    vsprintf(buffer, fmt, ap);
    va_end(ap);
    
    char *line_begin = buffer;
    char *line_end;
    
    do {
        if (message_log.size() == LOG_HEIGHT) {
            Message *to_remove = message_log.get(0);
            message_log.remove( to_remove );
            delete to_remove;
        }

        line_end = strchr(line_begin, '\n');

        if (line_end) {
            *line_end = '\0';
        }

        Message *msg = new Message(line_begin, text_color);
        message_log.push(msg);
        
        line_begin = line_end + 1;
    }
    while (line_end);
}

Entity* Gui::getSelectedItem(Container *inventory) {
    doRenderInventory(inventory);
    if (game.keyboard.vk == TCODK_ENTER || game.keyboard.vk == TCODK_TAB) {
        int index = getInventoryIndex(backpack_pointing);
        backpack_pointing = 0;
        if (index == -1) {return nullptr;}
        return inventory->getItem(index);
    }
    if (game.keyboard.vk == TCODK_CHAR) {
        int index = getInventoryIndex();
        if (index == -1) {return nullptr;}
        return inventory->getItem(index);
    }
    return nullptr;
}

void Gui::doRenderInventory(Container *inventory) {
    static const int INVENTORY_WIDTH = 100;
    static const int INVENTORY_HEIGHT = 50;
    
    static TCODConsole inventory_console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
    
    backpack_pointing = 0;
    
    do {
        //Print the inventory
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.clear();
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(0, 0 , INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_SET, "Inventory");
        
        //Equipment tab
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(3, 2, 54, 20, false, TCOD_BKGND_SET, "Equipment");
        
        //Print equiped item
        std::string equipment[10] = {"Headwear", "Bodywear", "Legwear", "Footwear",
                                     "Armwear", "Ring", "Ring", "Accessory",
                                     "Primary hand", "Secondary Hand"};
        
        int y = 4;
        for (int i = 0; i < 10; i++) {
            std::string equipment_name = equipment[i];
            inventory_console.printf(5, y, "%s", equipment_name.c_str());
            inventory_console.setChar(20, y, ':');
            Entity *equiping = game.player->equipment->getEquipment(y - 4);
            if (equiping == nullptr) {
                inventory_console.printf(22, y, "Empty");
            }
            else {
                inventory_console.printf(22, y, "%s", equiping->getName().c_str());
            }
            
            y++;
        }
        
        //Backpack tab
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(3, 23, 54, 26, false, TCOD_BKGND_SET, "Backpack");
        
        //Print item in backpack
        char alphabet = 'a';
        int item_in_page = 0;
        for (int y = 25; y <= MAX_ITEM_SHOWN + 25; y++) {
            Entity *item = inventory->getItem((y - 25) + MAX_ITEM_SHOWN * (page - 1));
            if (item != NULL) { 
                
                std::string equiped_str = (item->item_behavior->getIsEquip())? "[equiped]" : "";
                
                if (!item->item_behavior->isStackable()) {
                    inventory_console.printf(5, y, "%c) %s %s", alphabet,
                                             item->getName().c_str(), equiped_str.c_str());
                    item_in_page++;
                }
                else {
                    inventory_console.printf(5, y, "%c) %s x %i", alphabet, 
                                             item->getName().c_str(),
                                             item->item_behavior->getQty());
                    item_in_page++;
                }
                alphabet++;

            }
        }
        
        //Highlight select item
        inventory_console.setDefaultBackground(TCODColor::darkerGrey);
        if (backpack_pointing < item_in_page) {
            inventory_console.rect(5, 25 + backpack_pointing, 50, 1, false, TCOD_BKGND_SET);
        }
        
        //Print info of backpack tab
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.hline(4, 46, 52);
        int max_page = (int(inventory->getItemNum() / MAX_ITEM_SHOWN) + 1);
        inventory_console.printf(43, 47, "page : %i / %i", page, max_page);
        inventory_console.printf(5, 47, "weight : %.1f / %.1f", 
                                 inventory->getCurrentWeight(), 
                                 inventory->getMaxWeight());
        
        //Description tab
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(61, 2, 36, 30, false, TCOD_BKGND_SET, "Description");
        
        //Print description that fit into description tab size
        Entity* backpack_pointing_item;
        int index = getInventoryIndex(backpack_pointing);
        if (index == -1) {backpack_pointing_item = nullptr;}
        backpack_pointing_item = inventory->getItem(index);
        if (backpack_pointing_item != nullptr) {
            int y = 4;
            std::string word, line, desc;
            desc = backpack_pointing_item->item_behavior->getDesc();
            for (auto iterator = desc.begin(); iterator != desc.end(); iterator++) {
                word += *iterator;
                if (*iterator == ' ' || *iterator == '\n' || (iterator + 1) == desc.end()) {
                    if (line.size() + word.size() >= 32) {
                        inventory_console.printf(63, y, "%s", line.c_str());
                        line.clear();
                        y++;
                    }
                    line += word;
                    word.clear();
                }
                if ((iterator + 1) == desc.end() || *iterator == '\n') {
                    inventory_console.printf(63, y, "%s", line.c_str());
                    line.clear();
                    if (*iterator == '\n') {y++;}
                }
            }
        }
        
        //Usage tab
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(61, 33, 36, 16, false, TCOD_BKGND_SET, "Usage");
        
        //Print usage
        inventory_console.printf(63, 35, "[UP/DOWN] Select item");
        inventory_console.printf(63, 36, "[ENTER] Use / Equip / Unequip");
        inventory_console.printf(63, 37, "[A ~ L] Quick Use");
        inventory_console.printf(63, 38, "[TAB] Drop");
        
        //Blit the inventory tab to root
        TCODConsole::blit(&inventory_console, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, 
                          TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        
        //Wait and respond for change
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_NONE) {
            doCloseWindow();
        }
        
        if (game.keyboard.vk == TCODK_RIGHT && page + 1 <= max_page) {page++;}
        
        if (game.keyboard.vk == TCODK_LEFT && page != 1) {page--;}
        
        if (game.keyboard.vk == TCODK_UP && backpack_pointing != 0) {
            backpack_pointing -= 1;
        }
        
        
        if (game.keyboard.vk == TCODK_DOWN && 
            backpack_pointing < MAX_ITEM_SHOWN && 
            backpack_pointing + 1 < item_in_page) {
            
            backpack_pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_ENTER ||
            game.keyboard.vk == TCODK_TAB ||
            game.keyboard.vk == TCODK_CHAR) {
            
            return;
        }
    }
    while (game.keyboard.vk != TCODK_ESCAPE);
}

int Gui::getInventoryIndex() {
    if (game.keyboard.vk != TCODK_CHAR || game.keyboard.c - 'a' < 0) {return -1;}
    
    return ((game.keyboard.c - 'a') + ((page - 1) * MAX_ITEM_SHOWN));
}

int Gui::getInventoryIndex(int index) {
    if (index - 0 < 0) {return -1;}
    return ((index - 0) + ((page - 1) * MAX_ITEM_SHOWN));
}

void Gui::addPlayerNowStandOn(int x, int y) {
    log_console->setDefaultForeground(TCODColor::white);
    log_console->printf(x, y, "Standing on :");
    Entity *toppest_entity = nullptr;
    for (Entity *corpse : game.all_corpse) {
        if (corpse->getX() == game.player->getX() && corpse->getY() == game.player->getY()) {
            toppest_entity = corpse;
        }
    }
    for (Entity *item : game.all_item) {
        if (item->getX() == game.player->getX() && item->getY() == game.player->getY()) {
            toppest_entity = item;
        }
    }
    if (toppest_entity == NULL) {return;}
    log_console->printf(x, y, "Standing on : %s", toppest_entity->getName().c_str());
}

void Gui::doRenderTutorial() {
    static TCODConsole tutorial_console(100, 50);
    tutorial_console.setDefaultBackground(TCODColor::darkGrey);
    tutorial_console.setDefaultForeground(TCODColor::darkestGrey);
    tutorial_console.clear();
    tutorial_console.printFrame(0, 0 , 100, 50, false, TCOD_BKGND_SET, "Tutorial");
    
    tutorial_console.printRect(2, 2, 96, 56,
                               "MOVEMENT\n\n"
                               "[NUMPAD8 / J] MOVE OR ATTACK UP\n"
                               "[NUMPAD6 / L] MOVE OR ATTACK RIGHT\n"
                               "[NUMPAD2 / K] MOVE OR ATTACK DOWN\n"
                               "[NUMPAD4 / H] MOVE OR ATTACK LEFT\n"
                               "[NUMPAD9 / U] MOVE OR ATTACK TOP RIGHT\n"
                               "[NUMPAD3 / N] MOVE OR ATTACK BOTTOM RIGHT\n"
                               "[NUMPAD1 / B] MOVE OR ATTACK BOTTOM LEFT\n"
                               "[NUMPAD7 / Y] MOVE OR ATTACK TOP LEFT\n"
                               "[NUMPAD5 / ,] IDLE\n\n"
                               "UTILITY\n\n"
                               "[G] GRAB ITEM\n"
                               "[T] TOGGLE PRIMARY/SECONDARY HAND"
                               "[I] SHOW INVENTORY\n"
                               "[O] OBSERVING MODE\n\n"
                               "GAME\n\n"
                               "[F11] TOGGLE FULL SCREEN\n");
    
    addMessage(TCODColor::yellow, "[ESC] QUIT TUTORIAL");
    game.doRender();
    
    TCODConsole::blit(&tutorial_console, 0, 0, 100, 50, TCODConsole::root, 0, 0);
    TCODConsole::root->flush();
    
    while (game.keyboard.vk != TCODK_ESCAPE) {
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        if (game.keyboard.vk == TCODK_NONE) {
            doCloseWindow();
        }
    }
}

int Gui::doSelectWeaponSlot() {
    static TCODConsole select_weapon_slot_console(15, 7);
    
    int option = 0;
    
    while (game.keyboard.vk != TCODK_ESCAPE) {
        select_weapon_slot_console.setDefaultBackground(TCODColor::darkGrey);
        select_weapon_slot_console.setDefaultForeground(TCODColor::darkestGrey);
        select_weapon_slot_console.clear();
        select_weapon_slot_console.printFrame(0, 0, 15, 7, false, TCOD_BKGND_SET, "Select Weapon Slot [ARROW KEY]");
        select_weapon_slot_console.printf(2, 2, "Primary Hand");
        select_weapon_slot_console.printf(2, 3, "Secondary Hand");
        
        select_weapon_slot_console.setDefaultBackground(TCODColor::darkerGrey);
        select_weapon_slot_console.rect(2, 2 + option, 11, 1, false, TCOD_BKGND_SET);
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_UP && option == 1) {option = 0;}
        if (game.keyboard.vk == TCODK_DOWN && option == 0) {option = 1;}
        
        if (game.keyboard.vk == TCODK_ENTER) {return option;}
        
        TCODConsole::blit(&select_weapon_slot_console, 0, 0, 15, 7, TCODConsole::root, 40, 20);
        TCODConsole::root->flush();
    }
        
    return -1;
}

void Gui::doRenderObserving() {
    int cx = game.player->getX(), cy = game.player->getY();
    
    addMessage(TCODColor::yellow, "[NUMPAD / VI KEY] move cursor [ESC] Stop observing");
    
    while (game.keyboard.vk != TCODK_ESCAPE) {
        log_console->printf(0, 0, "Looking at :");
        game.doRender();
        
        TCODConsole::root->setDefaultForeground(TCODColor::white);
        TCODConsole::root->setChar(cx, cy, 'X');
        
        Entity *toppest_entity = nullptr;
        for (Entity *corpse : game.all_corpse) {
            if (corpse->getX() == cx && corpse->getY() == cy) {
                toppest_entity = corpse;
            }
        }
        
        for (Entity *item : game.all_item) {
            if (item->getX() == cx && item->getY() == cy) {
                toppest_entity = item;
            }
        }
        
        for (Entity *prop : game.all_prop) {
            if (prop->getX() == cx && prop->getY() == cy && prop->getAsciiChar() != '.') {
                toppest_entity = prop;
            }
        }
        
        for (Entity *character : game.all_character) {
            if (character->getX() == cx && character->getY() == cy && game.map->isInFov(cx, cy)) {
                toppest_entity = character;
            }
        }
        
        log_console->setDefaultBackground(TCODColor::black);
        log_console->rect(0, 0, 100, 1, TCOD_BKGND_SET);
        log_console->setDefaultForeground(TCODColor::white);
        if (toppest_entity == NULL) {
            std::string tile_name = game.map->isWall(cx, cy) ? "Wall" : "Floor";
            log_console->printf(0, 0, "Looking at : %s", tile_name.c_str());
        }
        else {
            log_console->printf(0, 0, "Looking at : %s", toppest_entity->getName().c_str());
        }
        
        TCODConsole::root->blit(log_console, 0, 0, 100, 10, TCODConsole::root, 0, game.map->getHeight());
        
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_ESCAPE) {break;}
        
        int dx = 0, dy = 0;
        switch (game.keyboard.c) {
            case 'j': dy--; break;
            case 'l': dx++; break;
            case 'k': dy++; break;
            case 'h': dx--; break;
            case 'u': dx++; dy--; break;
            case 'n': dx++; dy++; break;
            case 'b': dx--; dy++; break;
            case 'y': dx--; dy--; break;
        }
    
        switch (game.keyboard.vk) {
            case TCODK_KP8: dy--; break;
            case TCODK_KP6: dx++; break;
            case TCODK_KP2: dy++; break;
            case TCODK_KP4: dx--; break;
            case TCODK_KP9: dx++; dy--; break;
            case TCODK_KP3: dx++; dy++; break;
            case TCODK_KP1: dx--; dy++; break;
            case TCODK_KP7: dx--; dy--; break;
            case TCODK_NONE: doCloseWindow(); break;
        }
        
        if (game.map->isExplored(cx + dx, cy + dy)) {
            cx += dx;
            cy += dy;
        }
    }
}