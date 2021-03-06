#ifndef MAIN_HPP
#define MAIN_HPP

//Include Library
#include "libtcod.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

//Include header
#include "GameManager.hpp" //Manager game loop
#include "Map.hpp" //Map cell related
#include "Entity.hpp" //Any things in the game that is not a map cell
#include "MoveBehavior.hpp" //For a entity to be movable
#include "Control.hpp" //Determine what a entity do for every turn
#include "CombatBehavior.hpp" //Everything decide how entity fight
#include "Gui.hpp" //Off screen console
#include "ItemBehavior.hpp" //Item entity
#include "Container.hpp" //Entity to store item
#include "TargetPicking.hpp" //For item to select all_target
#include "Purpose.hpp" //For item to do something to selected all_target
#include "Equipment.hpp"
#include "Hunger.hpp"
#include "Tension.hpp"
#include "Town.hpp"
#include "DebugMode.hpp"

#include "Monster.hpp" //Monster List
#include "Items.hpp" //Item List
#include "Effect.hpp" //Effect List
#include "Trap.hpp" //Trap List
#include "CorpseInteraction.hpp" //Type of interaction with a corpse
#include "Shop.hpp" //Shop Gui and method
#include "Magic.hpp"
#include "Casting.hpp"

void doCloseWindow();
void checkGlobalKey();
bool checkFileExist(std::string file_name);

#endif /* MAIN_HPP */
