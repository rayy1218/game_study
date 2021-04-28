#ifndef MAP_HPP
#define MAP_HPP

#include "main.hpp"

struct Tile{
    bool is_explored;
    
    Tile();
};

class Map {
private:
    int width, height;
    Tile *tiles;
public:
    Entity *down_hole;
    TCODMap *map;
    
    Map(int width, int height);
    ~Map();
    
    bool canWalk(int x, int y);
    bool isExplored(int x, int y);
    bool isInFov(int x, int y);
    bool isWall(int x, int y);
    
    int getWidth();
    int getHeight();
    
    void getFov(int x, int y);
    void doRender();
    
    void addMonster(int x, int y);
    void addItem(int x, int y);
    void doSaveMap();
    void doLoadMap(); 
    
    void doGenerateMapCA();
    void doCellularAutomata(int process_count, int high_bound, int low_bound);
    bool checkNeighborWall(int x, int y, int high_bound, int low_bound);
    void doFloodFill(int x, int y);
};

#endif /* MAP_HPP */

