#include "main.hpp"

static const int CONSOLE_WIDTH = 130, CONSOLE_HEIGHT = 60;

GameManager game(CONSOLE_WIDTH, CONSOLE_HEIGHT);

int main(void) {
    if (checkFileExist("save.txt")) {game.doLoad();}
    
    while (!TCODConsole::isWindowClosed()) {
        game.doUpdate();
        game.doRender();
        TCODConsole::root->flush();
    }
    
    doCloseWindow();
}

void doCloseWindow() {
    game.doSave();
    exit(0);
}

void checkGlobalKey() {
    switch (game.keyboard.vk) {
        case TCODK_NONE : doCloseWindow(); break;
        case TCODK_F11: TCODConsole::root->setFullscreen(!TCODConsole::root->isFullscreen()); break;
    }
}

bool checkFileExist(std::string file_name) {
    std::ifstream file;
    file.open(file_name.c_str());
    bool exist = file.good();
    file.close();
    
    return exist;
}