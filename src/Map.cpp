#include "main.hpp"

const int MAX_MONSTER_IN_MAP = 6;
const int MAX_ITEM_IN_MAP = 12;
const int MAX_HOLE_IN_MAP = 4;
const int MAX_TRAP_IN_MAP = 20;
const int FLOOR_MONSTER_TYPE = 5;

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
        const int FLOOR = game.global_rng->getInt(0, FLOOR_MONSTER_TYPE - 1);
        while (monster_in_map < MAX_MONSTER_IN_MAP) {
            x = game.global_rng->getInt(1, width - 1);
            y = game.global_rng->getInt(1, height - 1);
            if (!canWalk(x, y) || isInFov(x, y) || 
                game.map->isInFov(x, y)) {continue;}
            
            addMonster(x, y, FLOOR);
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
    static TCODColor floor(TCODColor::darkGrey);
    static TCODColor wall(TCODColor::darkestGrey);
    
    TCODConsole::root->setDefaultForeground(TCODColor::white);
    
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (tiles[x + y * width].is_explored || !game.debug_mode.show_fog) {
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

void Map::addMonster(int x, int y, int floor) {
    Entity *monster;

    static int monster_each_floor_type[FLOOR_MONSTER_TYPE][3] = {{monster_dict::slime, monster_dict::slime_acid, monster_dict::slime_gaint},
                                                                      {monster_dict::skeleton, monster_dict::zombie, monster_dict::zombie_high},
                                                                      {monster_dict::demon, monster_dict::imp, monster_dict::tentacle},
                                                                      {monster_dict::griffin, monster_dict::minotaur, monster_dict::medusa},
                                                                      {monster_dict::goblin_armored, monster_dict::goblin_skirmisher, monster_dict::wolf}};

    monster = getMonster(x, y, monster_each_floor_type[floor][game.global_rng->getInt(0, 2)]);
    
    game.all_character.push(monster);
}

void Map::addItem(int x, int y) {
    struct ItemWeight {
        int item_id, weight;
    };

    class ItemChance {
    public:
        std::vector<ItemWeight> item_weight_list;

        int getRandomItemID() {
            int total_weight = 0;

            for (ItemWeight item_weight : item_weight_list) {
                total_weight += item_weight.weight;
            }

            int random_num = game.global_rng->getInt(1, total_weight), cumulative_weight = 0;
            for (ItemWeight item_weight : item_weight_list) {
                cumulative_weight += item_weight.weight;
                if (random_num - cumulative_weight <= 0) {return item_weight.item_id;}
            }
        }
    };

    ItemChance equipment_chance, potion_chance, weapon_chance, utility_chance, tome_chance;

    equipment_chance.item_weight_list = {{item_dict::headwear_heavy_metal, 1},
                                         {item_dict::headwear_light_metal, 6},
                                         {item_dict::headwear_leather, 10},
                                         {item_dict::bodywear_heavy_metal, 1},
                                         {item_dict::bodywear_light_metal, 6},
                                         {item_dict::bodywear_reinforced_leather, 10},
                                         {item_dict::bodywear_cloth, 16},
                                         {item_dict::legging_armored, 1},
                                         {item_dict::legging_reinforced_metal, 6},
                                         {item_dict::legging_reinforced_leather, 10},
                                         {item_dict::footwear_metal, 2},
                                         {item_dict::footwear_leather, 14},
                                         {item_dict::armwear_metal_full, 1},
                                         {item_dict::armwear_reinforced_metal, 6},
                                         {item_dict::armwear_reinforced_leather, 10}};

    potion_chance.item_weight_list = {{item_dict::potion_healing_standard, 1},
                                      {item_dict::potion_healing_flawed, 4},
                                      //{item_dict::potion_mana_standard, 1},
                                      //{item_dict::potion_mana_flawed, 4},
                                      {item_dict::potion_rage_standard, 1},
                                      {item_dict::potion_rage_flawed, 4},
                                      {item_dict::potion_protection_standard, 1},
                                      {item_dict::potion_protection_flawed, 4}};

    utility_chance.item_weight_list = {{item_dict::molotov, 10},
                                       {item_dict::throwing_knife, 10},
                                       {item_dict::incense, 6},
                                       {item_dict::food, 20}};

    weapon_chance.item_weight_list = {{item_dict::weapon_dagger, 20},
                                      {item_dict::weapon_knuckle, 20},
                                      {item_dict::weapon_gladius, 15},
                                      {item_dict::weapon_mace, 15},
                                      {item_dict::weapon_axe, 15},
                                      {item_dict::weapon_battleaxe, 10},
                                      {item_dict::weapon_battlehammer, 10},
                                      {item_dict::weapon_longsword, 10}};

    tome_chance.item_weight_list = {{item_dict::tome_windblade, 1},
                                    {item_dict::tome_firebolt, 1},
                                    {item_dict::tome_lighting_chain, 1}};


    int item_type_list[] = {40, 20, 20, 10, 10};
    int index = getIndexWeightedRandom(item_type_list, 5);

    int item_id;
    switch (index) {
        case 1: {
            item_id = utility_chance.getRandomItemID();
            break;
        }
        
        case 2: {
            item_id = potion_chance.getRandomItemID();
            break;
        }
        
        case 3: {
            item_id = equipment_chance.getRandomItemID();
            break;
        }

        case 4: {
            item_id = weapon_chance.getRandomItemID();
            break;
        }

        case 5: {
            item_id = tome_chance.getRandomItemID();
        }
    }

    Entity *item = getItem(x, y, item_id);

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