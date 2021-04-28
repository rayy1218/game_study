#include "main.hpp"

GameManager::GameManager(int width, int height): console_width(width), 
                                                 console_height(height),
                                                 floor_num(1) {

    TCODConsole::initRoot(console_width, console_height, "Game", true);
    TCODConsole::setCustomFont("terminal.png", TCOD_FONT_LAYOUT_ASCII_INROW);
    TCODConsole::root->setDefaultBackground(TCODColor::black);
    doSpawnPlayer();
    map = new Map(100, 50);
    gui = new Gui;
    map->doGenerateMapCA();
    status = status::STARTUP;
}

GameManager::~GameManager() {
    delete player;
    all_character.clearAndDelete();
    all_corpse.clearAndDelete();
    all_item.clearAndDelete();
    all_prop.clearAndDelete();
    delete map;
    delete gui;
}

void GameManager::doUpdate() {
    if (status == status::STARTUP) {
        game.map->getFov(player->getX(), player->getY());
        doRender();
    }
    
    if (status == status::DEFEAT) {
        Sleep(1000);
        exit(0);
    }
    
    status = status::IDLE;
    
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS, &keyboard, NULL);
    
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
    player->move_behavior = new MoveBehavior(player);
    player->combat_behavior = new PlayerCombatBehavior(player, 100, 10, 1);
    player->inventory = new Container(player, 20);
    all_character.push(player);
}

int GameManager::getConsoleWidth() {return console_width;}
int GameManager::getConsoleHeight() {return console_height;}
void GameManager::setFloorNum(int input) {floor_num = input;}
int GameManager::getFloorNum() {return floor_num;}
void GameManager::setStatus(int input) {status = input;}
int GameManager::getStatus() {return status;}