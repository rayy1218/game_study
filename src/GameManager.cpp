#include "main.hpp"

PlayerStats::PlayerStats(): hunger(nullptr), tension(nullptr) {}

PlayerStats::~PlayerStats() {
    if (hunger) {delete hunger;}
    if (tension) {delete tension;}
    if (magic) {delete magic;}
}

DebugMode::DebugMode(): show_fog(true) {}

GameManager::GameManager(int width, int height): console_width(width), 
                                                 console_height(height),
                                                 floor_num(1),
                                                 turn_used(0),
                                                 debug_mode() {

    TCODConsole::initRoot(console_width, console_height, "the living cave", true);
    doReadConfig();
    TCODConsole::setCustomFont(config.font_file_name.c_str(), TCOD_FONT_LAYOUT_ASCII_INROW);
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    global_rng = TCODRandom::getInstance();
    
    doStartup();
    
    gui->addMessage(TCODColor::yellow, "first time playing this game ? [F1] for tutorial");
}

GameManager::~GameManager() {
    all_character.clearAndDelete();
    all_corpse.clearAndDelete();
    all_item.clearAndDelete();
    all_prop.clearAndDelete();
    delete map;
    delete gui;
    delete player_stats;
}

void GameManager::doUpdate() {
    if (getFloorNum() == 0) {
        town->doRenderTownConsole();
        setFloorNum(1);
        doFloorTravel();
        
        doRender();
        TCODConsole::root->flush();
    }
    
    if (getFloorNum() == 10 && !player->combat_behavior->checkEntityDead()) {
        bool is_enemy_exist = false;
        for (Entity *character : all_character) {
            if (character->control->getStatus() == "enemy") {
                is_enemy_exist = true;
            }
        }
        
        if (!is_enemy_exist) {
            int x = 0, y = 0;
            while (!map->canWalk(x, y)) {
                x = game.global_rng->getInt(0, map->getWidth() - 1);
                y = game.global_rng->getInt(0, map->getHeight() - 1);
            }
        }
    }
    
    if (status == status::STARTUP) {
        for (int i = 0; i < 10; i++) {
            Entity *equip = game.player->equipment->getEquipment(i);
            equip = nullptr;
        }
        game.map->getFov(player->getX(), player->getY());
        doRender();
        TCODConsole::root->flush();
    }
    
    if (status == status::VICTORY) {
        game.gui->addMessage(TCODColor::yellow, "you complete the game in %i turn", turn_used);
        game.gui->addMessage(TCODColor::yellow, "[SPACE] restart [ESC] exit");
        
        doRender();
        TCODConsole::root->flush();
        
        do {
            TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &keyboard, NULL, false);
            
            if (keyboard.vk == TCODK_ESCAPE || keyboard.vk == TCODK_NONE) {doCloseWindow();}
        }
        while (keyboard.vk != TCODK_SPACE);
        
        doStartup();
        doRender();
        TCODConsole::root->flush();
    }
    
    if (status == status::DEFEAT) {
        std::remove("save.txt");
        
        game.gui->addMessage(TCODColor::red, "you are dead");
        game.gui->addMessage(TCODColor::yellow, "[SPACE] restart [ESC] exit");
        
        doRender();
        TCODConsole::root->flush();
        
        do {
            TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &keyboard, NULL, false);
            
            if (keyboard.vk == TCODK_ESCAPE || keyboard.vk == TCODK_NONE) {doCloseWindow();}
        }
        while (keyboard.vk != TCODK_SPACE);
        
        doStartup();
        doRender();
        TCODConsole::root->flush();
    }
    
    status = status::IDLE;

    TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &keyboard, NULL, false);
    
    player_stats->hunger->doUpdateHungerEffect();
    player_stats->tension->doUpdateTensionEffect();
    player->doUpdate();
    
    if (status == status::IDLE) {return;}
    
    if (status == status::NEW_TURN) {
        turn_used += 1;
    }
    
    for (Entity *character : all_character) {
        if (character != player) {
            character->doUpdate();   
        }
    }
    
    for (Entity *prop : all_prop) {
        if (!prop) {break;}
        prop->doUpdate();
    }
    
    player_stats->hunger->doUpdateHunger();
    player_stats->tension->doUpdateTension();
}

void GameManager::doRender() {
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    TCODConsole::root->clear();
    
    map->doRender();
    gui->doRender();

    for (Entity *entity : all_corpse) {
        if (!game.debug_mode.show_fog) {}
        else if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
    }
    
    for ( Entity *entity : all_item ) {
        if (!game.debug_mode.show_fog) {}
        else if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), 
                                             TCODColor::desaturatedYellow);
    }
    
    for ( Entity *entity : all_prop ) {
        if (!game.debug_mode.show_fog) {}
        else if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        if (entity->getAsciiChar() != '.') {
            TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), 
                                                 TCODColor::desaturatedBlue);
        }
    }
    
    for (Entity *entity : all_character) {
        if (!game.debug_mode.show_fog) {}
        else if (!map->isInFov(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        if (entity == player) {continue;}
        TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), 
                                             TCODColor::desaturatedRed);
    }
}

void GameManager::doSpawnPlayer() {
    player = new Entity(0, 0, "player", '@', TCODColor::green);
    player->control = new PlayerControl(player);
    player->inventory = new Container(player, 20);
    player->equipment = new Equipment(player);
    player->move_behavior = new MoveBehavior(player);
    player->combat_behavior = new PlayerCombatBehavior(player, 100, 2, 1, 10, 10);
    
    all_character.push(player);

    if (!checkFileExist("save.txt")) {
        Entity *starter_kit;
        starter_kit = getItem(player->getX(), player->getY(), item_dict::weapon_dagger);
        starter_kit->item_behavior->pick(player);
    }

    player_stats = new PlayerStats();
    player_stats->hunger = new Hunger(500);
    player_stats->tension = new Tension(100);
    player_stats->magic = new Magic(50);
}

void GameManager::doStartup() {
    floor_num = 1;
    turn_used = 0;
    
    if (map) {delete map;}
    if (gui) {delete gui;}
    if (player_stats) {delete player_stats;}
    all_character.clearAndDelete();
    all_corpse.clearAndDelete();
    all_item.clearAndDelete();
    all_prop.clearAndDelete();
    
    doSpawnPlayer();
    
    map = new Map(100, 50);
    town = new Town();
    gui = new Gui;
    doFloorTravel();

    status = status::STARTUP;
}

void GameManager::doFloorTravel() {
    gui->doRenderMapGenWait();
    
    gui->doResetFocusedEnemy();
    game.all_character.remove(game.player);
    game.all_character.clearAndDelete();
    game.all_corpse.clearAndDelete();
    game.all_item.clearAndDelete();
    game.all_prop.clearAndDelete();
    game.map->doGenerateMap();
    game.all_character.push(game.player);
    game.gui->addMessage(TCODColor::yellow, "you are now at floor %i", game.getFloorNum());
}

void GameManager::doSave() {
    std::ofstream save_file;
    
    std::remove("save.txt");
    if (game.getStatus() == status::DEFEAT || game.getStatus() == status::VICTORY) {
        return;
    }
    
    if (game.getFloorNum() != 0) {
        //Randomly drop item
    }
    
    save_file.open("save.txt");
    
    save_file << turn_used 
              << ' ' << player->combat_behavior->getCurrentHp() 
              << ' ' << player_stats->hunger->getCurrentHungerPoint() 
              << ' ' << player_stats->tension->getCurrentTension()
              << ' ' << player_stats->magic->getMp();
    
    int item_count = 0;
    std::string inventory_save;
    for (int index = 0; index < player->inventory->getItemNum(); index++) {
        Entity *item = player->inventory->getIndexItem(index);
        for (int i = 1; i <= item->item_behavior->getQty(); i++) {
            item_count += 1;
            inventory_save += ' ';
            inventory_save += std::to_string(item->item_behavior->getItemId());
            inventory_save += ' ';
            inventory_save += std::to_string(item->item_behavior->getIsEquip());
        }
    }
    save_file << ' ' << item_count << inventory_save;

    item_count = 0;
    std::string storage_save;
    for (int index = 0; index < game.town->storage_room->getItemNum(); index++) {
        Entity *item = game.town->storage_room->getIndexItem(index);
        for (int i = 1; i <= item->item_behavior->getQty(); i++) {
            item_count += 1;
            storage_save += ' ';
            storage_save += std::to_string(item->item_behavior->getItemId());
        }
    }
    save_file << ' ' << item_count << storage_save;

    for (int i = 0; i < CASTING_NUM; i++) {
        save_file << ' ' << game.player_stats->magic->isCastingUnlock(i);
    }


    save_file.close();
}

void GameManager::doLoad() {
    std::ifstream save_file;
    save_file.open("save.txt");
    
    int hp, hunger, tension, mp;
    save_file >> turn_used >> hp >> hunger >> tension >> mp;
    
    player->combat_behavior->setCurrentHp(hp);
    player_stats->hunger->setCurrentHungerPoint(hunger);
    player_stats->tension->setCurrentTension(tension);
    player_stats->magic->setMp(mp);
    
    int inventory_item_count;
    save_file >> inventory_item_count;
    for (int i = 1; i <= inventory_item_count; i++) {
        int item_id;
        bool is_equip;
        save_file >> item_id >> is_equip;
        
        Entity *item = getItem(0, 0, item_id);
        player->inventory->addItem(item);
        if (is_equip) {
            player->equipment->doEquip(item, item->item_behavior->getEquipmentIndex());
        }
    }
    
    int storage_item_count;
    save_file >> storage_item_count;
    for (int i = 1; i <= storage_item_count; i++) {
        int item_id;
        save_file >> item_id;
        
        Entity *item = getItem(0, 0, item_id);
        game.town->storage_room->addItem(item);
    }

    for (int i = 0; i < CASTING_NUM; i++) {
        bool casting_unlock;
        save_file >> casting_unlock;
        game.player_stats->magic->setCastingUnlock(i, casting_unlock);
    }


    game.setFloorNum(0);
    game.setStatus(status::IDLE);
    game.gui->addMessage(TCODColor::yellow, "save file loaded");
    
    save_file.close();
}

void GameManager::doReadConfig() {
    std::fstream config_file;
    config_file.open("config.txt");
    
    config_file.ignore(256, ' ');
    config_file >> config.font_file_name;
    config_file.ignore(256, ' ');
    config_file >> config.is_debug_mode;
    
    config_file.close();
}

int GameManager::getConsoleWidth() {return console_width;}
int GameManager::getConsoleHeight() {return console_height;}
void GameManager::setFloorNum(int input) {floor_num = input;}
int GameManager::getFloorNum() {return floor_num;}
void GameManager::setStatus(int input) {status = input;}
int GameManager::getStatus() {return status;}