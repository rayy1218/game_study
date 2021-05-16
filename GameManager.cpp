#include "main.hpp"

PlayerStats::PlayerStats(): hunger(nullptr), tension(nullptr) {}

PlayerStats::~PlayerStats() {
    if (hunger) {delete hunger;}
    if (tension) {delete tension;}
}

GameManager::GameManager(int width, int height): console_width(width), 
                                                 console_height(height),
                                                 floor_num(1),
                                                 turn_used(0) {

    TCODConsole::initRoot(console_width, console_height, "Game", false);
    TCODConsole::setCustomFont("terminal.png", TCOD_FONT_LAYOUT_ASCII_INROW);
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    global_rng = TCODRandom::getInstance();
    
    doStartup();

    gui->addMessage(TCODColor::yellow, "First time playing this game ? [F1] for tutorial");
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
        game.gui->addMessage(TCODColor::yellow, "You complete the game in %i turn", turn_used);
        game.gui->addMessage(TCODColor::white, "[SPACE] Restart [ESC] Exit");
        
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
        game.gui->addMessage(TCODColor::red, "You are dead");
        game.gui->addMessage(TCODColor::yellow, "[SPACE] Restart [ESC] Exit");
        
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
    
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &keyboard, NULL, false);
    if (keyboard.vk == TCODK_ESCAPE) {
        exit(0);
    }
    
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
    
    for ( Entity *prop : all_prop ) {
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

    for ( Entity *entity : all_corpse ) {
        if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
    }
    
    for ( Entity *entity : all_item ) {
        if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), 
                                             TCODColor::desaturatedYellow);
    }
    
    for ( Entity *entity : all_prop ) {
        if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        if (entity->getAsciiChar() != '.') {
            TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), 
                                                 TCODColor::desaturatedBlue);
        }
    }
    
    for ( Entity *entity : all_character ) {
        if (!map->isInFov(entity->getX(), entity->getY())) {continue;}
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
    player->combat_behavior = new PlayerCombatBehavior(player, 100, 2, 1, 50);
    
    all_character.push(player);
    
    player_stats = new PlayerStats();
    player_stats->hunger = new Hunger(500);
    player_stats->tension = new Tension(100);
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
    gui = new Gui;
    map->doGenerateMapCA();
    
    Entity *starter_kit;
    starter_kit = getItem(player->getX(), player->getY(), item_dict::weapon_dagger);
    all_item.push(starter_kit);
    
    status = status::STARTUP;
}

void GameManager::doFloorTravel() {
    game.all_character.remove(game.player);
    game.all_character.clearAndDelete();
    game.all_corpse.clearAndDelete();
    game.all_item.clearAndDelete();
    game.all_prop.clearAndDelete();
    game.map->doGenerateMapCA();
    game.all_character.push(game.player);
    game.gui->addMessage(TCODColor::yellow, "You are now at floor %i", game.getFloorNum());
}

int GameManager::getConsoleWidth() {return console_width;}
int GameManager::getConsoleHeight() {return console_height;}
void GameManager::setFloorNum(int input) {floor_num = input;}
int GameManager::getFloorNum() {return floor_num;}
void GameManager::setStatus(int input) {status = input;}
int GameManager::getStatus() {return status;}