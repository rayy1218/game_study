#ifndef MAP_HPP
#define MAP_HPP

#include "main.hpp"

struct Tile{
    bool is_explored;
    
    Tile();
};

class Map {
private:
    //Attribute
    int width, height;
    Tile *tiles;
public:
    //Attribute
    TCODMap *map;
    
    //Constructor & Destructor
    Map(int width, int height);
    ~Map();

    //Method
    bool canWalk(int x, int y);
    void getFov(int x, int y);
    bool isInFov(int x, int y);
    void doRender();
    void doGenerateBoss();
    void addMonster(int x, int y);
    void addItem(int x, int y);
    void addTrap(int x, int y);
    void doSaveMap();
    void doLoadMap(); 
    void doGenerateMap(int seed = 0);
    void doCellularAutomata(int process_count, int high_bound, int low_bound);
    bool checkNeighborWall(int x, int y, int high_bound, int low_bound);
    void doFloodFill(int x, int y);
    void doResetExplored();
        
    //Accessor
    int getWidth();
    int getHeight();
    bool isExplored(int x, int y);
    bool isWall(int x, int y);
};

#endif /* MAP_HPP */

