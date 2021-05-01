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
    void doCreateInfomationConsole();
    void addMessage(TCODColor text_color, const char *fmt, ...);
    void addPlayerNowStandOn(int x, int y);
    Entity* getSelectedItem(Container *inventory);
    void doRenderInventory(Container *inventory);
    int getInventoryIndex();
    int getInventoryIndex(int index);
};

#endif /* GUI_HPP */

