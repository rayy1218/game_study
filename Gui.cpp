#include <cstdarg>

#include "main.hpp"

static const int LOG_HEIGHT = 9;
static const int MAX_ITEM_SHOWN = 15;
static int page = 1;
static int console_width = TCODConsole::root->getWidth() / 2;

Message::Message(std::string message, TCODColor text_color): message( message ),
                                                             text_color( text_color ) {}

Gui::Gui() {
    log_console = new TCODConsole( 100, 10 );
    infomation_console = new TCODConsole(30 , 60);
}

Gui::~Gui() {
    
}

void Gui::doRender() {
    log_console->setDefaultBackground( TCODColor::black );
    log_console->clear();
    
    
    
    int y = 1;
    for ( Message *message : message_log ) {
        log_console->setDefaultForeground( message->text_color );
        log_console->printf( 0, y, message->message.c_str() );
        y++;
    }
    
    addPlayerNowStandOn(0, 0);
    
    infomation_console->setDefaultBackground(TCODColor::black);
    infomation_console->clear();
    
    infomation_console->setDefaultForeground(TCODColor::white);
    infomation_console->printf(1, 1, "Floor: %i", game.getFloorNum());
    infomation_console->printf(1, 3, "%s ", game.player->getName());
    infomation_console->printf(1, 4, "Attack: %i * %.2f", game.player->combat_behavior->getAtkPoint(), 
                                                          game.player->combat_behavior->getAtkBoost());
    infomation_console->printf(1, 5, "Defense: %i * %.2f", game.player->combat_behavior->getDefPoint(), 
                                                           game.player->combat_behavior->getDefBoost());
    
    addBar( 1, 7, 28, 1, TCODColor::lighterRed, TCODColor::red, 
           game.player->combat_behavior->getMaxHp(), 
           game.player->combat_behavior->getCurrentHp(), "Health", TCODColor::white );
    addBar( 1, 8, 28, 1, TCODColor::lighterBlue, TCODColor::han, 50, 25, "Mana", TCODColor::white );
    addBar( 1, 9, 28, 1, TCODColor::lighterViolet, TCODColor::violet, 50, 25, "Tension", TCODColor::white );
    addBar( 1, 10, 28, 1, TCODColor::lighterSepia, TCODColor::sepia, 50, 25, "Hunger", TCODColor::white );
    
    TCODConsole::root->blit(log_console, 0, 0, 100, 10, TCODConsole::root, 0, game.map->getHeight());
    TCODConsole::root->blit(infomation_console, 0, 0, 30, 60, TCODConsole::root, 100, 0);
}

void Gui::addBar( int x, int y, int w, int h, TCODColor empty_color, TCODColor full_color, 
                  float max_value, float current_value, std::string bar_title, TCODColor title_color ) {
    
    infomation_console->setDefaultBackground( empty_color );
    infomation_console->rect( x, y, w, h, true, TCOD_BKGND_SET );
    
    int bar_len = ( current_value / max_value ) * w;
    
    infomation_console->setDefaultBackground( full_color );
    infomation_console->rect( x ,y, bar_len, h, true, TCOD_BKGND_SET );
    
    infomation_console->setDefaultForeground( title_color );
    infomation_console->printf( x, y, "%s: %.0f / %.0f", bar_title.c_str(), current_value, max_value );
}

void Gui::addMessage(TCODColor text_color, const char *fmt, ...) {
    va_list ap;
    char buffer[128];
    va_start( ap, fmt );
    vsprintf( buffer, fmt, ap );
    va_end( ap );
    
    char *line_begin = buffer;
    char *line_end;
    
    do {
        if ( message_log.size() == LOG_HEIGHT ) {
            Message *to_remove = message_log.get(0);
            message_log.remove( to_remove );
            delete to_remove;
        }

        line_end = strchr( line_begin, '\n' );

        if ( line_end ) {
            *line_end = '\0';
        }

        Message *msg = new Message( line_begin, text_color );
        message_log.push( msg );
        
        line_begin = line_end + 1;
    }
    while ( line_end );
}

Entity* Gui::getSelectedItem(Container *inventory) {
    doRenderInventory(inventory);
    if (game.keyboard.vk == TCODK_CHAR) {
        int index = getInventoryIndex();
        if (index == -1) {return nullptr;}
        return inventory->getItem(index);
    }   
}

void Gui::doRenderInventory(Container *inventory) {
    static const int INVENTORY_WIDTH = 50;
    static const int INVENTORY_HEIGHT = 20;
    
    static TCODConsole inventory_console(INVENTORY_WIDTH, INVENTORY_HEIGHT);
    
    do {
        inventory_console.setDefaultBackground(TCODColor::darkGrey);
        inventory_console.clear();
        inventory_console.setDefaultForeground(TCODColor::darkestGrey);
        inventory_console.printFrame(0, 0 , INVENTORY_WIDTH, INVENTORY_HEIGHT, true, TCOD_BKGND_SET, "Inventory");
        inventory_console.setDefaultForeground(TCODColor::black);
        
        char alphabet = 'a';
        for (int y = 1; y <= MAX_ITEM_SHOWN; y++) {
            Entity *item = inventory->getItem((y - 1) + MAX_ITEM_SHOWN * (page - 1));
            if (item != NULL) {
                inventory_console.printf(1, y, "%c) %s", alphabet, item->getName().c_str());
                alphabet++;
            }
        }
        
        inventory_console.printf(1, 17, "page : %i / %i", page, (int(inventory->getItemNum() / MAX_ITEM_SHOWN) + 1));
        inventory_console.printf(1, 18, "weight : %.1f / %.1f", inventory->getCurrentWeight(), inventory->getMaxWeight());
        TCODConsole::blit(&inventory_console, 0, 0, INVENTORY_WIDTH, INVENTORY_HEIGHT, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_RIGHT && page + 1 <= (int(inventory->getItemNum() / MAX_ITEM_SHOWN) + 1)) {page++;}
        if (game.keyboard.vk == TCODK_LEFT && page != 1) {page--;}
        
        if (game.keyboard.vk == TCODK_CHAR) {return;}
    }
    while (game.keyboard.vk != TCODK_ESCAPE);
}

int Gui::getInventoryIndex() {
    if (game.keyboard.vk != TCODK_CHAR || game.keyboard.c - 'a' < 0) {return -1;}
    
    return ((game.keyboard.c - 'a') + ((page - 1) * MAX_ITEM_SHOWN));
}

void Gui::addPlayerNowStandOn(int x, int y) {
    log_console->setDefaultForeground(TCODColor::white);
    log_console->printf(x, y, "Standing on :");
    Entity *toppest_entity = nullptr;
    for (Entity *corpse : game.all_corpse) {
        if (corpse->getX() == game.player->getX() && corpse->getY() == game.player->getY()) {
            toppest_entity = corpse;
        }
    }
    for (Entity *item : game.all_item) {
        if (item->getX() == game.player->getX() && item->getY() == game.player->getY()) {
            toppest_entity = item;
        }
    }
    if (toppest_entity == NULL) {return;}
    log_console->printf(x, y, "Standing on : %s", toppest_entity->getName().c_str());
}