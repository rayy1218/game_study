#include <cstdarg>

#include "main.hpp"

static const int LOG_HEIGHT = 9;
static const int MAX_ITEM_SHOWN = 16;
static int page = 1;
static int backpack_pointing = 0;

Message::Message(std::string message, TCODColor text_color): message( message ),
                                                             text_color( text_color ) {}

Gui::Gui() {
    log_console = new TCODConsole( 100, 10 );
    infomation_console = new TCODConsole(30 , 60);
    focused_enemy = nullptr;
}

Gui::~Gui() {
    message_log.clearAndDelete();
    delete log_console;
    delete infomation_console;
}

void Gui::doRender() {
    log_console->setDefaultBackground( TCODColor::black );
    log_console->clear();
    
    infomation_console->setDefaultBackground(TCODColor::black);
    infomation_console->setDefaultForeground(TCODColor::white);
    infomation_console->clear();
    infomation_console->printf(1, 1, "floor: %i", game.getFloorNum());
    doRenderPlayerInfo();
    doRenderEnemyInfo();
    
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

void Gui::doRenderPlayerInfo() {
    const int START_Y = 3;
    
    infomation_console->setDefaultBackground(TCODColor::black);
    infomation_console->setDefaultForeground(TCODColor::white);

    infomation_console->printFrame(1, START_Y, 28, 10, false, TCOD_BKGND_SET, 
                                   "player's status");
    infomation_console->printf(2, START_Y + 1, "attack: %i * %.2f", game.player->combat_behavior->getAtkPoint() +
                                                          game.player->combat_behavior->getEquipmentAtkPoint(), 
                                                          game.player->combat_behavior->getAtkBoost());
    infomation_console->printf(2, START_Y + 2, "defense: %i * %.2f", game.player->combat_behavior->getDefPoint() +
                                                           game.player->combat_behavior->getEquipmentDefPoint(), 
                                                           game.player->combat_behavior->getDefBoost());
    
    std::string hand_using = (game.player->equipment->isPrimaryHand())? "primary Hand" : "secondary Hand";
    infomation_console->printf(2, START_Y + 3, "using: %s", hand_using.c_str());
    
    addBar(2, START_Y + 5, 26, 1, TCODColor::lighterRed, TCODColor::red, 
           game.player->combat_behavior->getMaxHp(), 
           game.player->combat_behavior->getCurrentHp(), "health", TCODColor::white);
    addBar(2, START_Y + 6, 26, 1, TCODColor::lighterBlue, TCODColor::han,
           game.player_stats->magic->getMaxMp(),
           game.player_stats->magic->getMp(), "Mana", TCODColor::white);
    addBar(2, START_Y + 7, 26, 1, TCODColor::lighterViolet, TCODColor::violet, 
           game.player_stats->tension->getMaxTension(), 
           game.player_stats->tension->getCurrentTension(), "tension", TCODColor::white);
    addBar(2, START_Y + 8, 26, 1, TCODColor::lighterSepia, TCODColor::sepia, 
           game.player_stats->hunger->getMaxHungerPoint(), 
           game.player_stats->hunger->getCurrentHungerPoint(), "hunger", TCODColor::white);
}

void Gui::doRenderEnemyInfo() {
    const int START_Y = 14;
    
    bool previous_focused_exist = false;
    
    infomation_console->setDefaultBackground(TCODColor::black);
    infomation_console->setDefaultForeground(TCODColor::white);
    
    infomation_console->printFrame(1, START_Y, 28, 10, false, TCOD_BKGND_SET, "focusing enemy");

    character_in_fov.erase(character_in_fov.begin(), character_in_fov.end());
    for (Entity *character : game.all_character) {
        if (character == game.player) {continue;}
        if (!game.map->isInFov(character->getX(), character->getY())) {continue;}
        character_in_fov.push_back(character);
        if (focused_enemy == character) {
            previous_focused_exist = true;
        }
    }
    
    if (!previous_focused_exist) {
        if (character_in_fov.empty()) {
            focused_enemy = nullptr;
        }
        else {
            focused_enemy = character_in_fov[0];
        }
    }
    
    if (focused_enemy) {
        infomation_console->printf(2, START_Y + 2, "name: %s", focused_enemy->getName().c_str());
        
        std::string health_str;
        float health_percentage = (float)focused_enemy->combat_behavior->getCurrentHp() / 
                                         focused_enemy->combat_behavior->getMaxHp();
        if (health_percentage <= 0.2) {
            health_str = "dying";
        }
        else if (health_percentage <= 0.5) {
            health_str = "critical";
        }
        else if (health_percentage <= 0.8) {
            health_str = "moderate damaged";
        }
        else if (health_percentage < 1) {
            health_str = "minor damaged";
        }
        else if (health_percentage == 1) {
            health_str = "whole";
        }
        infomation_console->printf(2, START_Y + 3, "health: %s", health_str.c_str());
        
        float dfs_adv = (float)game.player->combat_behavior->getTotalDef() /
                               focused_enemy->combat_behavior->getTotalDef();
        std::string dfs_str;
        if (dfs_adv >= 0.8 && dfs_adv < 1.2) {
            dfs_str = "none";
        }
        else if (dfs_adv >= 1.2 && dfs_adv < 1.5) {
            dfs_str = "adv";
        }
        else if (dfs_adv >= 1.5 && dfs_adv < 2) {
            dfs_str = "high adv";
        }
        else if (dfs_adv > 2) {
            dfs_str = "extreme adv";
        }
        if (dfs_adv >= 0.5 && dfs_adv < 0.8) {
            dfs_str = "disadv";
        }
        else if (dfs_adv >= 0.2 && dfs_adv < 0.5) {
            dfs_str = "high disadv";
        }
        else if (dfs_adv < 0.2) {
            dfs_str = "extreme disadv";
        }
        infomation_console->printf(2, START_Y + 4, "dfs: %s", dfs_str.c_str());
        
        float atk_adv = (float)game.player->combat_behavior->getTotalAtk() /
                               focused_enemy->combat_behavior->getTotalAtk();
        std::string atk_str;
        if (atk_adv >= 0.8 && atk_adv < 1.2) {
            atk_str = "none";
        }
        else if (atk_adv >= 1.2 && atk_adv < 1.5) {
            atk_str = "adv";
        }
        else if (atk_adv >= 1.5 && atk_adv < 2) {
            atk_str = "high adv";
        }
        else if (atk_adv > 2) {
            atk_str = "extreme adv";
        }
        if (atk_adv >= 0.5 && atk_adv < 0.8) {
            atk_str = "disadv";
        }
        else if (atk_adv >= 0.2 && atk_adv < 0.5) {
            atk_str = "high disadv";
        }
        else if (atk_adv < 0.2) {
            atk_str = "extreme disadv";
        }
        infomation_console->printf(2, START_Y + 5, "atk: %s", atk_str.c_str());
        infomation_console->printf(2, START_Y + 6, "status: %s", 
                                   focused_enemy->control->getStatus().c_str());
        
        return;
    }
    
    infomation_console->printf(2, START_Y + 2, "name: none");
}

void Gui::doResetFocusedEnemy() {
    focused_enemy = nullptr;
}

void Gui::doRenderFocusedEnemy() {
    int enemy_num_in_fov = character_in_fov.size();
    int index = 0;
    
    if (enemy_num_in_fov == 0) {
        game.gui->addMessage(TCODColor::yellow, "no enemy around");
        return;
    }
    
    game.gui->addMessage(TCODColor::yellow, "[F] change all_target [SPACE] confirm all_target");
    
    while (true) {
        focused_enemy = character_in_fov.at(index);
        
        game.doRender();
        TCODConsole::root->setCharBackground(focused_enemy->getX(), focused_enemy->getY(), 
                                             TCODColor::lightLime, TCOD_BKGND_SET);
        
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);

        if (game.keyboard.vk == TCODK_SPACE) {
            break;
        }

        if (game.keyboard.vk == TCODK_CHAR && game.keyboard.c == 'f') {
            index++;
            if (index == enemy_num_in_fov) {
                index = 0;
            }
            continue;
        }
    }
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
    doRenderPlayerInventory(inventory);
    if (game.keyboard.vk == TCODK_ENTER || game.keyboard.vk == TCODK_TAB) {
        int index = getInventoryIndex(backpack_pointing);
        backpack_pointing = 0;
        if (index == -1) {return nullptr;}
        return inventory->getIndexItem(index);
    }
    if (game.keyboard.vk == TCODK_CHAR) {
        int index = getInventoryIndex();
        if (index == -1) {return nullptr;}
        return inventory->getIndexItem(index);
    }
    return nullptr;
}

void Gui::doRenderPlayerInventory(Container *inventory) {
    static const int INVENTORY_WIDTH = 100;
    static const int INVENTORY_HEIGHT = 50;
    
    static TCODConsole inventory_console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
    
    backpack_pointing = 0;
    
    do {
        int max_page, item_in_page;
        
        //Print the inventory
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.clear();
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(0, 0 , INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_SET, "inventory");
        
        doRenderEquipmentTab(&inventory_console);
        
        doRenderBackpackTab(&inventory_console, inventory, &max_page, &item_in_page);
        
        doRenderDescTab(&inventory_console, inventory);
        
        doRenderUsageTab(&inventory_console);
        
        //Blit the inventory tab to root
        TCODConsole::blit(&inventory_console, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, 
                          TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        
        //Wait and respond for change
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
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

void Gui::doRenderEquipmentTab(TCODConsole *inventory_console) {
    inventory_console->setDefaultBackground(TCODColor::darkGrey);
    inventory_console->setDefaultForeground(TCODColor::darkestGrey);
    inventory_console->printFrame(3, 2, 54, 20, false, TCOD_BKGND_SET, "equipment");
        
    //Print equiped item
    inventory_console->printRect(5, 4, 48, 10, "headwear       :\n"
                                               "bodywear       :\n"
                                               "legwear        :\n"
                                               "footwear       :\n"
                                               "armwear        :\n"
                                               "ring           :\n"
                                               "ring           :\n"
                                               "accessory      :\n"
                                               "primary hand   :\n"
                                               "secondary hand :\n");

    int y = 4;
    for (int i = 0; i < 10; i++) {
        Entity *equiping = game.player->equipment->getEquipment(y - 4);
        if (equiping == nullptr) {
            inventory_console->printf(22, y, "empty");
        }
        else {
            inventory_console->printf(22, y, "%s", equiping->getName().c_str());
        }

        y++;
    }
}

void Gui::doRenderBackpackTab(TCODConsole *inventory_console, Container *inventory,
                              int *max_page, int *item_in_page) {
    inventory_console->setDefaultBackground(TCODColor::darkGrey);
    inventory_console->setDefaultForeground(TCODColor::darkestGrey);
    inventory_console->printFrame(3, 23, 54, 26, false, TCOD_BKGND_SET, "backpack");

    //Print item in backpack
    char alphabet = 'a';
    int item_count = 0;
    for (int y = 25; y <= MAX_ITEM_SHOWN + 25; y++) {
        Entity *item = inventory->getIndexItem((y - 25) + MAX_ITEM_SHOWN * (page - 1));
        if (item != NULL) { 

            std::string equiped_str = (item->item_behavior->getIsEquip())? "[equiped]" : "";

            if (!item->item_behavior->isStackable()) {
                inventory_console->printf(5, y, "%c) %s %s", alphabet,
                                         item->getName().c_str(), equiped_str.c_str());
                item_count++;
            }
            else {
                inventory_console->printf(5, y, "%c) %s x %i", alphabet, 
                                         item->getName().c_str(),
                                         item->item_behavior->getQty());
                item_count++;
            }
            alphabet++;

        }
    }

    //Highlight select item
    inventory_console->setDefaultBackground(TCODColor::darkerGrey);
    if (backpack_pointing < item_count) {
        inventory_console->rect(5, 25 + backpack_pointing, 50, 1, false, TCOD_BKGND_SET);
    }

    //Print info of backpack tab
    inventory_console->setDefaultBackground(TCODColor::darkGrey);
    inventory_console->hline(4, 46, 52);
    *max_page = (int(inventory->getItemNum() / MAX_ITEM_SHOWN) + 1);
    inventory_console->printf(43, 47, "page : %i / %i", page, *max_page);
    inventory_console->printf(5, 47, "weight : %.1f / %.1f", 
                             inventory->getCurrentWeight(), 
                             inventory->getMaxWeight());
    
    *item_in_page = item_count;
}

void Gui::doRenderDescTab(TCODConsole *inventory_console, Container *inventory) {
    //Description tab
    inventory_console->setDefaultBackground(TCODColor::darkGrey);
    inventory_console->setDefaultForeground(TCODColor::darkestGrey);
    inventory_console->printFrame(61, 2, 36, 30, false, TCOD_BKGND_SET, "description");

    //Print description that fit into description tab size
    Entity* backpack_pointing_item;
    int index = getInventoryIndex(backpack_pointing);
    if (index == -1) {backpack_pointing_item = nullptr;}
    backpack_pointing_item = inventory->getIndexItem(index);
    if (backpack_pointing_item != nullptr) {
        int y = 4;
        std::string word, line, desc;
        desc = backpack_pointing_item->item_behavior->getDesc();
        for (auto iterator = desc.begin(); iterator != desc.end(); iterator++) {
            word += *iterator;
            if (*iterator == ' ' || *iterator == '\n' || (iterator + 1) == desc.end()) {
                if (line.size() + word.size() >= 32) {
                    inventory_console->printf(63, y, "%s", line.c_str());
                    line.clear();
                    y++;
                }
                line += word;
                word.clear();
            }
            if ((iterator + 1) == desc.end() || *iterator == '\n') {
                inventory_console->printf(63, y, "%s", line.c_str());
                line.clear();
                if (*iterator == '\n') {y++;}
            }
        }
    }
}

void Gui::doRenderUsageTab(TCODConsole *inventory_console) {
    inventory_console->setDefaultBackground(TCODColor::darkGrey);
    inventory_console->setDefaultForeground(TCODColor::darkestGrey);
    inventory_console->printFrame(61, 33, 36, 16, false, TCOD_BKGND_SET, "usage");

    inventory_console->printf(63, 35, "[UP/DOWN] select item");
    inventory_console->printf(63, 36, "[ENTER] use / equip / unequip");
    inventory_console->printf(63, 37, "[A ~ L] quick Use");
    inventory_console->printf(63, 38, "[TAB] drop");
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
    log_console->printf(x, y, "standing on :");
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
    log_console->printf(x, y, "standing on : %s", toppest_entity->getName().c_str());
}

void Gui::doRenderTutorial() {
    static TCODConsole tutorial_console(100, 50);
    tutorial_console.setDefaultBackground(TCODColor::darkGrey);
    tutorial_console.setDefaultForeground(TCODColor::darkestGrey);
    tutorial_console.clear();
    tutorial_console.printFrame(0, 0 , 100, 50, false, TCOD_BKGND_SET, "tutorial");

    std::ifstream tutorial_doc("doc/tutorial.txt");
    std::string tutorial((std::istreambuf_iterator<char>(tutorial_doc)),std::istreambuf_iterator<char>());

    tutorial_console.printRect(2, 2, 96, 56, "%s", tutorial.c_str());
    
    addMessage(TCODColor::yellow, "[ESC] quit tutorial");
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
        select_weapon_slot_console.printFrame(0, 0, 15, 7, false, TCOD_BKGND_SET, "select Weapon Slot [ARROW KEY]");
        select_weapon_slot_console.printf(2, 2, "primary Hand");
        select_weapon_slot_console.printf(2, 3, "secondary Hand");
        
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
    
    addMessage(TCODColor::yellow, "[NUMPAD / VI KEY] move cursor [ESC] stop observing");
    
    while (game.keyboard.vk != TCODK_ESCAPE) {
        log_console->printf(0, 0, "looking at :");
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
            std::string tile_name = game.map->isWall(cx, cy) ? "wall" : "floor";
            log_console->printf(0, 0, "looking at : %s", tile_name.c_str());
        }
        else {
            log_console->printf(0, 0, "looking at : %s", toppest_entity->getName().c_str());
        }
        
        TCODConsole::root->blit(log_console, 0, 0, 100, 10, TCODConsole::root, 0, game.map->getHeight());
        
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
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

void Gui::doRenderMapGenWait() {
    static TCODConsole wait_console(100, 50);
    wait_console.setDefaultBackground(TCODColor::black);
    wait_console.setDefaultForeground(TCODColor::white);
    
    wait_console.printf(50, 20, "Map generating...");
    
    game.gui->doRender();
    TCODConsole::blit(&wait_console, 0, 0, 100, 50, TCODConsole::root, 0, 0);
    TCODConsole::root->flush();
}