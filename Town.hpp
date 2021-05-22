#ifndef TOWN_HPP
#define TOWN_HPP

struct TownOption {
private:
    std::string name, description;
public:
    TownOption(std::string name, std::string description);
    
    std::string getName();
    std::string getDesc();
};

class Town {
private:
    TCODList<TownOption*> loc_list, self_list;
public:
    Container *storage_room;
    
    Town();
    ~Town();
    
    //Town
    void doRenderTownConsole();
    void doRenderTownDescTab(TCODConsole *town_console, int current_pointing);
    void doRenderTownLocTab(TCODConsole *town_console, int current_pointing);
    void doRenderTownSelfTab(TCODConsole *town_console, int current_pointing);
    void doRenderTownUsageTab(TCODConsole *town_console);
    
    void doRenderStorage();
    
    //List
    void getTownLocList();
    void getTownSelfList();
};

#endif /* TOWN_HPP */

