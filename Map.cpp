#include "main.hpp"

static int MIN_ROOM_SIZE = 8;
static int MAX_ROOM_SIZE = 12;
static int MAX_MONSTER_IN_MAP = 6;
static int MAX_ITEM_IN_MAP = 50;
static int generate_status;

Tile::Tile(): is_explored(false) {} 

Map::Map(int width, int height): width(width), height(height) {
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);
}

Map::~Map() {
    delete tiles;
    delete down_hole;
    delete map;
}

void Map::doGenerateMapCA() {
    //PARAMETER
    static const int WALL_CHANCE = 50;
    static const int WALL_NEEDED = 5;
    
    for (int i = 0; i < (width * height); i++) {
        tiles[i].is_explored = false;
    }
    
    //Reset the map and replace wall to air by chance
    map->clear(false, false);
    TCODRandom *rng = TCODRandom::getInstance();
    for (int x = 1; x < width - 1; x++) {
        for (int y = 1; y < height - 1; y++) {
            if (rng->getInt(0, 100) > WALL_CHANCE) {
                map->setProperties(x, y, true, true);
            }
        }
    }
    
    //Place horizontal blank
    for (int cx = width / 8; cx < width - width / 8; cx++) {
        for (int cy = height / 2 - 1; cy < height / 2 + 1; cy++) {
            map->setProperties(cx, cy, true, true);
        }
    }
    
    for (int cx = width / 8; cx < width - width / 8; cx++) {
        for (int cy = height / 4 - 1; cy < height / 4 + 1; cy++) {
            map->setProperties(cx, cy, true, true);
        }
    }
    
    for (int cx = width / 8; cx < width - width / 8; cx++) {
        for (int cy = (height - height / 4) - 1; cy < (height - height / 4) + 1; cy++) {
            map->setProperties(cx, cy, true, true);
        }
    }
    
    //do Cellular Automata iteration
    doCellularAutomata(3, WALL_NEEDED, 0);
    doCellularAutomata(1, WALL_NEEDED, -1);
    
    //Flood fill a random point
    int x, y;
    do {
        x = rng->getInt(1, width - 1);
        y = rng->getInt(1, height - 1);
    }
    while (isWall(x, y));
    doFloodFill(x, y);
    
    //Check percentage of open space
    float wall_count = 0;
    for (int cx = 1; cx < width - 1; cx++) {
        for (int cy = 1; cy < height - 1; cy++) {
            if (isWall(cx, cy)) {wall_count++;}
        }
    }
    
    //Regenerate map if not enough open space
    //If enough open space put player inside
    if (wall_count / (width * height) * 100 >= 55) {
        doGenerateMapCA();
        
    }
    else {
    //If enough open space put player inside
        do {
            x = rng->getInt(1, width - 1);
            y = rng->getInt(1, height - 1);
        }
        while (isWall(x, y));

        game.player->setX(x);
        game.player->setY(y);
        getFov(x, y);
        
        do {
            x = rng->getInt(1, width - 1);
            y = rng->getInt(1, height - 1);
        }
        while (isWall(x, y) || game.player->control->getDistanceTo(x, y) < 20);
        
        down_hole = new Entity(x, y, "Hole", 'O', TCODColor::white);
        game.all_prop.push(down_hole);
        
        int monster_in_map = 0;
        while (monster_in_map < MAX_MONSTER_IN_MAP) {
            x = rng->getInt(1, width - 1);
            y = rng->getInt(1, height - 1);
            if (!canWalk(x, y) || isInFov(x, y)) {continue;}
            
            addMonster(x, y);
            monster_in_map++;
        }
        
        int item_in_map = 0;
        while( item_in_map < MAX_ITEM_IN_MAP) {
            x = rng->getInt( 1, width - 1 );
            y = rng->getInt( 1, height - 1 );
            if ( !canWalk( x, y ) || isInFov( x, y ) ) { continue; }
            
            addItem( x, y );
            item_in_map++;
        }
    }
}

void Map::doCellularAutomata(int process_count, int high_bound, int low_bound) {
    for (int i = 1; i <= process_count; i++) {
        for (int x = 1; x < width - 1; x++) { 
            for (int y = 1; y < height - 1; y++) {
                if (checkNeighborWall(x, y, high_bound, low_bound)) {
                    map->setProperties(x, y, false, false);
                }
                else {
                    map->setProperties(x, y, true, true);
                }
            }   
        }
    }
}

void Map::doFloodFill(int x, int y) {
    for (int cx = x - 1; cx <= x + 1; cx++) {
        for (int cy = y - 1; cy <= y + 1; cy++) {
            if (!isWall(cx, cy)) {
                map->setProperties(cx, cy, false, false);
                doFloodFill(cx, cy);
            }
        }
    }
}

bool Map::checkNeighborWall(int x, int y, int high_bound, int low_bound) {
    int wall_count = 0;
    for (int cx = x - 1; cx <= x + 1; cx++) {
        for (int cy = y - 1; cy <= y + 1; cy++) {
            if (isWall(cx, cy)) {wall_count++;}
        }
    }
    
    return (wall_count >= high_bound || wall_count <= low_bound); 
}

bool Map::canWalk(int x, int y) {
    for (Entity *entity : game.all_character) {
        if (entity->getX() == x && entity->getY() == y) {return false;}
    }
    
    return map->isWalkable(x, y);
}

void Map::getFov(int x, int y) {
    map->computeFov(x, y, 7, FOV_SYMMETRIC_SHADOWCAST);
}

bool Map::isInFov(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return false;
    }
    if (map->isInFov(x, y)) {
        tiles[x + y * width].is_explored = true;
        return true;
    }
    return false;
}

void Map::doRender() {
    int debug = 0;
    static TCODColor floor(TCODColor::darkGrey);
    static TCODColor wall(TCODColor::darkestGrey);
    
    TCODConsole::root->setDefaultForeground(TCODColor::white);
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (tiles[x + y * width].is_explored || debug) {
                TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? wall : floor);
                TCODConsole::root->putChar(x, y, (isWall(x, y)) ? '#' : '.');
            }
            if (isInFov(x, y)) {
                TCODConsole::root->setCharBackground(x, y, TCODConsole::root->getCharBackground(x, y) * TCODColor::lighterLime);
                TCODConsole::root->putChar(x, y, (isWall(x, y)) ? '#' : '.');
            }
            else {
                TCODConsole::root->setCharBackground(x, y, TCODConsole::root->getCharBackground(x, y) * TCODColor::darkerGrey);
            }
        }
    }
}

void Map::addMonster(int x, int y) {
    Entity *monster;
    TCODRandom *rng = TCODRandom::getInstance();
    
    monster = getMonster(x, y, rng->getInt(0, 4));
    
    game.all_character.push(monster);
}

void Map::addItem(int x, int y) {
    Entity *item;
    TCODRandom *rng = TCODRandom::getInstance();
    
    item = getItem(x, y, rng->getInt(0, 4));
    
    game.all_item.push(item);
}

int Map::getWidth() {return width;}
int Map::getHeight() {return height;}
bool Map::isExplored(int x, int y) {return tiles[x + y * width].is_explored;}
bool Map::isWall(int x, int y) {return !map->isWalkable(x, y);}