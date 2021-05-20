#include "main.hpp"

TownOption::TownOption(std::string name, std::string description): 
                       name(name), description(description) {}

std::string TownOption::getName() {return name;}
std::string TownOption::getDesc() {return description;}

Town::Town() {
    getTownLocList();
    getTownSelfList();
}

Town::~Town() {
    loc_list.clearAndDelete();
    self_list.clearAndDelete();
}

void Town::doRenderTownConsole() {
    static TCODConsole town_console(100, 50);
    
    int current_pointing = 0;
    
    while (game.keyboard.vk != TCODK_ESCAPE) {
        town_console.setDefaultBackground(TCODColor::darkGrey);
        town_console.setDefaultForeground(TCODColor::darkestGrey);
        town_console.clear();
        
        town_console.printFrame(0, 0, 100, 50, false, TCOD_BKGND_SET, "town");
        
        doRenderTownLocTab(&town_console, current_pointing);
        doRenderTownSelfTab(&town_console, current_pointing);
        doRenderTownDescTab(&town_console, current_pointing);
        doRenderTownUsageTab(&town_console);
        
        game.gui->doRender();
        TCODConsole::blit(&town_console, 0, 0, 100, 50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, true);
        
        if (game.keyboard.vk == TCODK_NONE) {doCloseWindow();}
        if (game.keyboard.vk == TCODK_ESCAPE) {return;}
        
        if (game.keyboard.vk == TCODK_UP) {
            if (current_pointing == 0) {
                current_pointing = loc_list.size() + self_list.size() - 1;
                continue;
            }
            current_pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            if (current_pointing == loc_list.size() + self_list.size() - 1) {
                current_pointing = 0;
                continue;
            }
            current_pointing += 1;
        }
    }
}

void Town::doRenderTownLocTab(TCODConsole *town_console, int current_pointing) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(2, 2, 60, 28, false, TCOD_BKGND_SET, "location");
    
    int y = 4, alphabet = 'a';
    town_console->setDefaultBackground(TCODColor::darkerGrey);
    if (current_pointing < loc_list.size()) {
        town_console->rect(4, y + current_pointing, 56, 1, false, TCOD_BKGND_SET);
    }
    
    for (TownOption *loc : loc_list) {
        town_console->printf(4, y, "%c) %s", alphabet, loc->getName().c_str());
        y++;
    }
}

void Town::doRenderTownSelfTab(TCODConsole *town_console, int current_pointing) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(2, 31, 60, 18, false, TCOD_BKGND_SET, "self");
    
    int y = 33, alphabet = 'a';
    town_console->setDefaultBackground(TCODColor::darkerGrey);
    if (current_pointing >= loc_list.size()) {
        current_pointing -= loc_list.size();
        town_console->rect(4, y + current_pointing, 56, 1, false, TCOD_BKGND_SET);
    }
    
    for (TownOption *self : self_list) {
        town_console->printf(4, y, "%c) %s", alphabet, self->getName().c_str());
        y++;
    }
}

void Town::doRenderTownDescTab(TCODConsole *town_console, int current_pointing) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(65, 2, 32, 34, false, TCOD_BKGND_SET, "description");
    
    std::string desc;
    
    if (current_pointing < loc_list.size()) {
        TownOption *loc = loc_list.get(current_pointing);
        desc = loc->getDesc();
    }
    else {
        current_pointing -= loc_list.size();
        TownOption *self = self_list.get(current_pointing);
        desc = self->getDesc();
    }
    
    town_console->printRect(67, 4, 28, 30, "%s", desc.c_str());
}

void Town::doRenderTownUsageTab(TCODConsole *town_console) {
    town_console->setDefaultBackground(TCODColor::darkGrey);
    town_console->setDefaultForeground(TCODColor::darkestGrey);
    town_console->printFrame(65, 37, 32, 12, false, TCOD_BKGND_SET, "usage");
    
    town_console->printRect(67, 39, 38, 8, 
                            "[UP/DOWN] to select\n"
                            "[ENTER] to go");
}

void Town::getTownLocList() {
    loc_list.push(new TownOption("home - storage room",
                                 "where you store you treasury"));
    
    loc_list.push(new TownOption("home - workshop",
                                 "where you craft item for exploration"));
    
    loc_list.push(new TownOption("market - general shop",
                                 "exploration supplies like food, lantern oil "
                                 "sold here"));
    
    loc_list.push(new TownOption("market - armorsmith shop",
                                 "armor, cloth and shield sold here"));
    
    loc_list.push(new TownOption("market - weaponsmith shop",
                                 "melee and missle weapon sold here"));
    
    loc_list.push(new TownOption("market - enchanted accessory shop",
                                 "accessory enchanted by priest or witch sold here"));
    
    loc_list.push(new TownOption("market - potion alchemist",
                                 "flawed and low quality potion, alchemy material"
                                 "and empty bottle sold here"));
    
    loc_list.push(new TownOption("market - witchcraft", 
                                 "scroll, jar of sprite orb and ritual medium "
                                 "sold here"));
    
    loc_list.push(new TownOption("guild - aventurer marketplace",
                                 "all the loot from cave creature can only be "
                                 "sell here for guild to collect \"cave entering fee\""));
    
    loc_list.push(new TownOption("guild - mission board",
                                 "favourite place of bounty hunter, quest usually"
                                 "consist of suppllies request, great cave creature "
                                 "hunting and rare cave creature loot request"));
    
    loc_list.push(new TownOption("guild - chamber of master",
                                 "voluntary assembly for master class aventurer "
                                 "to provide skill lecture for guild member"));
}

void Town::getTownSelfList() {
    self_list.push(new TownOption("status", "check current status"));
    self_list.push(new TownOption("inventory", "check inventory"));
    self_list.push(new TownOption("journal", "check journal for last aventure"));
    self_list.push(new TownOption("skill", "check level for each skill you have"));
}