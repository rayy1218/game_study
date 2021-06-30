#include "main.hpp"

static int MAX_MONSTER_IN_MAP = 6;
static int MAX_ITEM_IN_MAP = 12;
static int MAX_HOLE_IN_MAP = 4;
static int MAX_TRAP_IN_MAP = 20;

Tile::Tile(): is_explored(false) {} 

Map::Map(int width, int height): width(width), height(height) {
    tiles = new Tile[width * height];
    map = new TCODMap(width, height);
}

Map::~Map() {
    delete tiles;
    delete map;
}

void Map::doGenerateMap(int seed) {
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
        doGenerateMap();
        
    }
    else {
    //If enough open space put player inside
        do {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
        }
        while (isWall(x, y));

        game.player->setX(x);
        game.player->setY(y);
        
        getFov(x, y);
        
        if (game.getFloorNum() == 10) {
            doGenerateBoss();
            return;
        }
        
        Entity *prop; 
        Control *control;
        
        do {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
        }
        while (isWall(x, y) || game.player->control->getDistanceTo(x, y) < 20);
        
        prop = new Entity(x, y, "Down Stair", '>', TCODColor::white);
        control = new DownStairControl(prop);
        prop->control = control;
        game.all_prop.push(prop);
        
        do {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
        }
        while (isWall(x, y) || game.player->control->getDistanceTo(x, y) >= 2 || 
               (game.player->getX() == x && game.player->getY() == y));
        
        prop = new Entity(x, y, "Up Stair", '<', TCODColor::white);
        control = new UpStairControl(prop);
        prop->control = control;
        game.all_prop.push(prop);
        
        for (int i = 1; i <= MAX_HOLE_IN_MAP; i++) {
            do {
                x = game.global_rng->getInt(1, width - 1);
                y = game.global_rng->getInt(1, height - 1);
            }
            while (isWall(x, y) || game.player->control->getDistanceTo(x, y) < 20);
            
            prop = new Entity(x, y, "Shaft", 'O', TCODColor::white);
            control = new HoleControl(prop);
            prop->control = control;
            game.all_prop.push(prop);
        }
        
        int monster_in_map = 0;
        while (monster_in_map < MAX_MONSTER_IN_MAP) {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
            if (!canWalk(x, y) || isInFov(x, y) || 
                game.map->isInFov(x, y)) {continue;}
            
            addMonster(x, y);
            monster_in_map++;
        }

        int item_in_map = 0;
        while (item_in_map < MAX_ITEM_IN_MAP) {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
            if (!canWalk(x, y) || isInFov(x, y) || 
                game.map->isInFov(x, y)) {continue;}
            
            addItem(x, y);
            item_in_map++;
        }
        
        int trap_in_map = 0;
        while (trap_in_map < MAX_TRAP_IN_MAP) {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
            if (!canWalk(x, y) || isInFov(x, y) || 
                game.map->isInFov(x, y)) {continue;}
            
            addTrap(x, y);
            trap_in_map++;
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
                TCODConsole::root->setCharBackground(x, y, isWall(x, y) ? wall : floor);
                TCODConsole::root->setCharBackground(x, y, TCODConsole::root->getCharBackground(x, y) * TCODColor::lighterLime);
                TCODConsole::root->putChar(x, y, (isWall(x, y)) ? '#' : '.');
            }
            else {
                TCODConsole::root->setCharBackground(x, y, TCODConsole::root->getCharBackground(x, y) * TCODColor::darkerGrey);
            }
        }
    }
}

void Map::doGenerateBoss() {
    switch(game.global_rng->getInt(1, 3)) {
        case 1: {
            for (int monster_num = 0; monster_num < 3; monster_num++) {
                int x = 0, y = 0;
                while (!canWalk(x, y)) {
                    x = game.global_rng->getInt(0, width - 1);
                    y = game.global_rng->getInt(0, height - 1);
                }
                Entity* monster = getMonster(x, y, monster_dict::dragon_child);
                game.all_character.push(monster);
            }
            
            for (int monster_num = 0; monster_num < 2; monster_num++) {
                int x = 0, y = 0;
                while (!canWalk(x, y)) {
                    x = game.global_rng->getInt(0, width - 1);
                    y = game.global_rng->getInt(0, height - 1);
                }
                Entity* monster = getMonster(x, y, monster_dict::dragon_adult);
                game.all_character.push(monster);
            }
            
            int x = 0, y = 0;
            while (!canWalk(x, y)) {
                x = game.global_rng->getInt(0, width - 1);
                y = game.global_rng->getInt(0, height - 1);
            }
            Entity* monster = getMonster(x, y, monster_dict::dragon_boss);
            game.all_character.push(monster);
            
            break;
        }
        
        case 2: {
            int x = 0, y = 0;
            while (!canWalk(x, y)) {
                x = game.global_rng->getInt(0, width - 1);
                y = game.global_rng->getInt(0, height - 1);
            }
            Entity* monster = getMonster(x, y, monster_dict::tyrant_boss);
            game.all_character.push(monster);
            break;
        }
        
        case 3: {
            for (int monster_num = 0; monster_num < MAX_MONSTER_IN_MAP; monster_num++) {
                int x = 0, y = 0;
                while (!canWalk(x, y)) {
                    x = game.global_rng->getInt(0, width - 1);
                    y = game.global_rng->getInt(0, height - 1);
                }
                Entity* monster = getMonster(x, y, monster_dict::one_eyed_infantry + game.global_rng->getInt(0, 1));
                game.all_character.push(monster);
            }
            
            int x = 0, y = 0;
            while (!canWalk(x, y)) {
                x = game.global_rng->getInt(0, width - 1);
                y = game.global_rng->getInt(0, height - 1);
            }
            Entity* monster = getMonster(x, y, monster_dict::dragon_boss);
            game.all_character.push(monster);
            break;
        }
    }
    
    int x = 0, y = 0;
    while (!canWalk(x, y)) {
        x = game.global_rng->getInt(game.player->getX() - 1, game.player->getX() + 1);
        y = game.global_rng->getInt(game.player->getY() - 1, game.player->getY() + 1);
    }
        
    Entity *prop = new Entity(x, y, "Up Stair", '<', TCODColor::white);
    Control *control = new UpStairControl(prop);
    prop->control = control;
    game.all_prop.push(prop);
}

void Map::addMonster(int x, int y) {
    Entity *monster;
    
    monster = getMonster(x, y, game.global_rng->getInt(1, MONSTER_TYPE));
    
    game.all_character.push(monster);
}

void Map::addItem(int x, int y) {
    Entity *item;
    
    static int item_type_weight_list[] = {60,  //Item 
                                          20,   //Equipment
                                          20}; //Weapon
    
    static int equipment_weight_list[] = {1,  //headwear_heavy_metal
                                          6,  //headwear_light_metal
                                          10,  //headwear_leather
                                          1,  //bodywear_heavy_metal
                                          6,  //bodywear_light_metal
                                          10,  //bodywear_reinforced_leather
                                          16,  //bodywear_cloth
                                          1,  //legging_armored
                                          6,  //legging_reinforced_metal
                                          10,  //legging_reinforced_leather
                                          2,  //footwear_metal
                                          14,  //footwear_leather
                                          1,  //armwear_metal_fuul
                                          6,  //armwear_reinforced_metal
                                          10}; //armwear_reinforced_leather
                                          
    
    static int item_weight_list[] = {10, //molotov
                                     12, //throwing_Axe
                                     8,  //incense
                                     18, //potion_healing
                                     20};  
                                        
    static int weapon_weight_list[] = {1,
                                       1,
                                       1,
                                       1,
                                       1,
                                       1,
                                       1,
                                       1};
    
    int index = getIndexWeightedRandom(item_type_weight_list, 3);

    switch (index) {
        case 1: {
            int index = getIndexWeightedRandom(item_weight_list, 5);
            item = getItem(x, y, index);
            break;
        }
        
        case 2: {
            int index = getIndexWeightedRandom(equipment_weight_list, 15);
            item = getItem(x, y, index + 5);
            break;
        }
        
        case 3: {
            int index = getIndexWeightedRandom(weapon_weight_list, 8);
            item = getItem(x, y, index + 26);
            break;
        }
    }
    
    game.all_item.push(item);
}

void Map::addTrap(int x, int y) {
    Entity *trap;
    
    trap = getTrap(x, y, game.global_rng->getInt(1, 5));
    
    game.all_prop.push(trap);
}

void Map::doResetExplored() {
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {           
            tiles[x + y * width].is_explored = false;
        }
    }
}

int Map::getWidth() {return width;}
int Map::getHeight() {return height;}
bool Map::isExplored(int x, int y) {return tiles[x + y * width].is_explored;}
bool Map::isWall(int x, int y) {return !map->isWalkable(x, y);}