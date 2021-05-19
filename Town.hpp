#ifndef TOWN_HPP
#define TOWN_HPP

class Town {
public:
    Town();
    ~Town();
    
    //Town
    void doRenderTownConsole();
    void doRenderTownDescTab(TCODConsole *town_console);
    void doRenderTownLocTab(TCODConsole *town_console);
    void doRenderTownSelfTab(TCODConsole *town_console);
    void doRenderTownUsageTab(TCODConsole *town_console);
};

#endif /* TOWN_HPP */

