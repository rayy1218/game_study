#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

class Map;
class Entity;
class Gui;

class GameManager {
private:
    int console_width, console_height;
    int status, floor_num;
public:
    TCOD_key_t keyboard;
    Map *map;
    Entity *player;
    Gui *gui;
    TCODList<Entity*> all_character;
    TCODList<Entity*> all_corpse;
    TCODList<Entity*> all_item;
    TCODList<Entity*> all_prop;
    
    GameManager(int width, int height);
    ~GameManager();
    
    int getConsoleWidth();
    int getConsoleHeight();
    void setFloorNum(int input);
    int getFloorNum();
    void setStatus(int input);
    int getStatus();
    
    void doUpdate();
    void doRender();
    void doSpawnPlayer();
};

extern GameManager game;

#endif /* GAMEMANAGER_HPP */
