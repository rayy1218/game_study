#ifndef GUI_HPP
#define GUI_HPP

struct Message {
    std::string message;
    TCODColor text_color;
    
    Message(std::string message, TCODColor text_color);
};

class Gui {
private:
    //Attribute
    TCODConsole *log_console, *infomation_console;
    TCODList<Message*> message_log;
    Entity *focused_enemy;
    std::vector<Entity*> character_in_fov;
public:
    //Contrustor & destructor
    Gui();
    virtual ~Gui();
    
    //Method
    void doRender();
    void addBar(int x, int y, int w, int h, 
                TCODColor empty_color, TCODColor full_color,
                float max_value, float current_value, 
                std::string bar_title, TCODColor title_color);
    void doRenderPlayerInfo();
    void doRenderEnemyInfo();
    void doRenderFocusedEnemy();
    void addMessage(TCODColor text_color, const char *fmt, ...);
    void addPlayerNowStandOn(int x, int y);
    void doRenderTutorial();
    void doRenderObserving();
    void doRenderMapGenWait();
    void doResetFocusedEnemy();
    
    //Inventory
    Entity* getSelectedItem(Container *inventory);
    void doRenderPlayerInventory(Container *inventory);\
    void doRenderEquipmentTab(TCODConsole *inventory_console);
    void doRenderBackpackTab(TCODConsole *inventory_console, Container *inventory, 
                             int *max_page, int *item_in_page);
    void doRenderDescTab(TCODConsole *inventory_console, Container *inventory);
    void doRenderUsageTab(TCODConsole *inventory_console);
    int getInventoryIndex();
    int getInventoryIndex(int index);
    int doSelectWeaponSlot();
};

#endif /* GUI_HPP */

