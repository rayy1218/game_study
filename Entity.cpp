
#include "main.hpp"

Entity::Entity(int x, int y, std::string name, int ascii_char, TCODColor ascii_color)
              : position_x(x), position_y(y), name(name), ascii(ascii_char),
                ascii_color(ascii_color), control(nullptr), 
                move_behavior(nullptr), combat_behavior(nullptr),
                item_behavior(nullptr), inventory(nullptr), equipment(nullptr) {}

Entity::~Entity() {
    if (control != NULL) {delete control;}
    if (move_behavior != NULL) {delete move_behavior;}
    if (inventory != NULL) {delete inventory;}
    if (item_behavior != NULL) {delete item_behavior;}
    if (equipment != NULL) {delete equipment;}
    if (combat_behavior != NULL) {delete combat_behavior;}
}

void Entity::doUpdate() {
    control->doUpdate();
    if (game.getStatus() == status::NEW_TURN && equipment != nullptr) {
        combat_behavior->updateEquipmentAttribute();
    }
    for (Effect *effect : all_effect) {
        if (game.getStatus() != status::NEW_TURN) {break;}
        effect->doUpdate();
    }
}

void Entity::doRender() {
    TCODConsole::root->putChar( position_x, position_y, ascii );
    TCODConsole::root->setCharForeground( position_x, position_y, ascii_color );
}

int Entity::getX() { return position_x; }
int Entity::getY() { return position_y; }
std::string Entity::getName() { return name; }
int Entity::getAsciiChar() { return ascii; }
TCODColor Entity::getAsciiColor() { return ascii_color; }
void Entity::setX(int input) { position_x = input; }
void Entity::setY(int input) { position_y = input; }
void Entity::setName(std::string input) { name = input; }
void Entity::setAsciiChar(int input) { ascii = input; }
void Entity::setAsciiColor(TCODColor input) { ascii_color = input; }
        