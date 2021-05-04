#include "main.hpp"

GameManager::GameManager(int width, int height): console_width(width), 
                                                 console_height(height),
                                                 floor_num(1) {

    TCODConsole::initRoot(console_width, console_height, "Game", false);
    TCODConsole::setCustomFont("terminal.png", TCOD_FONT_LAYOUT_ASCII_INROW);
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    global_rng = TCODRandom::getInstance();
    
    doStartup();
    
    gui->addMessage(TCODColor::yellow, "First time playing this game ? [F1] for tutorial");
}

GameManager::~GameManager() {
    delete player;
    delete map;
    delete gui;
    all_character.clearAndDelete();
    all_corpse.clearAndDelete();
    all_item.clearAndDelete();
    all_prop.clearAndDelete();
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
    
    if (status == status::DEFEAT) {
        do {
            TCODSystem::waitForEvent(TCOD_EVENT_ANY, &keyboard, NULL, true);
            
            if (keyboard.vk == TCODK_ESCAPE) {exit(0);}
        }
        while (keyboard.vk != TCODK_SPACE);
        doStartup();
    }
    
    status = status::IDLE;
    
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &keyboard, NULL, false);
    if (keyboard.vk == TCODK_ESCAPE) {
        exit(0);
    }
    
    player->doUpdate();
    
    if (status == status::IDLE) {return;}
    for (Entity *character : all_character) {
        if (character != player) {
            character->doUpdate();   
        }
    }
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
        TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), TCODColor::desaturatedYellow);
    }
    
    for ( Entity *entity : all_character ) {
        if (!map->isInFov(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        if (entity == player) {continue;}
        TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), TCODColor::desaturatedRed);
    }
    
    for ( Entity *entity : all_prop ) {
        if (!map->isExplored(entity->getX(), entity->getY())) {continue;}
        entity->doRender();
        TCODConsole::root->setCharBackground(entity->getX(), entity->getY(), TCODColor::desaturatedBlue);
    }
}

void GameManager::doSpawnPlayer() {
    player = new Entity(0, 0, "player", '@', TCODColor::green);
    player->control = new PlayerControl(player);
    player->inventory = new Container(player, 20);
    player->equipment = new Equipment(player);
    player->move_behavior = new MoveBehavior(player);
    player->combat_behavior = new PlayerCombatBehavior(player, 100, 10, 1);
    
    all_character.push(player);
}

void GameManager::doStartup() {
    if (player) {delete player;}
    if (map) {delete map;}
    if (gui) {delete gui;}
    all_character.clearAndDelete();
    all_corpse.clearAndDelete();
    all_item.clearAndDelete();
    all_prop.clearAndDelete();
    
    doSpawnPlayer();
    map = new Map(100, 50);
    gui = new Gui;
    map->doGenerateMapCA();
    status = status::STARTUP;
}

int GameManager::getConsoleWidth() {return console_width;}
int GameManager::getConsoleHeight() {return console_height;}
void GameManager::setFloorNum(int input) {floor_num = input;}
int GameManager::getFloorNum() {return floor_num;}
void GameManager::setStatus(int input) {status = input;}
int GameManager::getStatus() {return status;}