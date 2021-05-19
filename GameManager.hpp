#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

class Map;
class Entity;
class Gui;
class Town;
class Hunger;
class Tension;

enum status{
    STARTUP  = 0, 
    IDLE     = 1, 
    NEW_TURN = 2, 
    DEFEAT   = 3, 
    VICTORY  = 4
};

struct PlayerStats {
    Hunger *hunger;
    Tension *tension;
    
    PlayerStats();
    ~PlayerStats();
};

class GameManager {
private:
    //Attribute
    int console_width, console_height, status, floor_num, turn_used;
public:
    //Attribute
    TCOD_key_t keyboard;
    Map *map;
    
    Entity *player;
    PlayerStats *player_stats;
    
    Gui *gui;
    Town *town;
    TCODRandom *global_rng;
    
    //Attribute - Entity type list
    TCODList<Entity*> all_character;
    TCODList<Entity*> all_corpse;
    TCODList<Entity*> all_item;
    TCODList<Entity*> all_prop;
    
    //Constructor & Destructor
    GameManager(int width, int height);
    ~GameManager();
    
    //Method
    void doUpdate();
    void doRender();
    void doSpawnPlayer();
    void doStartup();
    void doFloorTravel();
    
    void doSave();
    void doLoad();
    
    //Accessor
    int getConsoleWidth();
    int getConsoleHeight();
    void setFloorNum(int input);
    int getFloorNum();
    void setStatus(int input);
    int getStatus();
};

extern GameManager game;

#endif /* GAMEMANAGER_HPP */
