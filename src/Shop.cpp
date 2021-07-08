#include "main.hpp"

const int SHOP_PAGE_MAX_ITEM = 26;
const float SHOP_BUYING_VALUE_RATE = 0.8, SHOP_SELLING_VALUE_RATE = 1;

ShopInterface::ShopInterface(Shop *shop): shop(shop) {}

void ShopInterface::doRenderShop() {
    const int PAGE_MAX_ITEM = 16;
    TCODConsole shop_console(100, 50);
    bool pointing_shop_or_self = true;
    int current_pointing = 0, shop_current_page = 1, self_current_page = 1;
    Entity *pointing_item = nullptr;
    
    while (true) {
        int shop_max_page = int(shop->selling_item.size() / PAGE_MAX_ITEM) + 1,
            self_max_page = int(game.player->inventory->getItemNum() / PAGE_MAX_ITEM) + 1,
            shop_page_item_num = shop->selling_item.size() - (shop_current_page - 1) * PAGE_MAX_ITEM,
            self_page_item_num = game.player->inventory->getItemNum() - (self_current_page - 1) * PAGE_MAX_ITEM;
        
        if (pointing_shop_or_self) {
            int index = current_pointing + (shop_current_page - 1) * PAGE_MAX_ITEM;
            pointing_item = getItem(0, 0 , shop->selling_item.at(index));
        }
        else {
            int index = current_pointing + (self_current_page - 1) * PAGE_MAX_ITEM;
            pointing_item = game.player->inventory->getIndexItem(index);
        }
        
        shop_console.setDefaultBackground(TCODColor::darkGrey);
        shop_console.setDefaultForeground(TCODColor::darkestGrey);
        shop_console.clear();
        
        shop_console.printFrame(0, 0, 100, 50, false, TCOD_BKGND_SET, "shop");
        
        if (!pointing_shop_or_self) {
        shop_console.setDefaultForeground(TCODColor::darkerGrey);
        } 
        shop_console.printFrame(2, 2, 60, 23, false, TCOD_BKGND_SET, "shop inventory");
        shop_console.printf(4, 23, "page: %i/%i", shop_current_page, shop_max_page);
        {
            int y = 4;
            
            if (pointing_shop_or_self) {
                shop_console.setDefaultBackground(TCODColor::darkerGrey);
                shop_console.rect(4, current_pointing + y, 56, 1, true, TCOD_BKGND_SET);
                shop_console.setDefaultBackground(TCODColor::darkGrey);
            }
            
            for (int i : shop->selling_item) {
                Entity *item = getItem(0, 0, i);
                shop_console.printf(4, y, "%s sell for %i", item->getName().c_str(),
                                    int(item->item_behavior->tradable->price / SHOP_SELLING_VALUE_RATE));
                y++;
                
                delete item;
            }
        }
        
        shop_console.setDefaultBackground(TCODColor::darkGrey);
        shop_console.setDefaultForeground(TCODColor::darkestGrey);
        shop_console.printFrame(65, 2, 32, 3, false, TCOD_BKGND_SET, "coin");
        if (pointing_shop_or_self) {
            shop_console.setDefaultForeground(TCODColor::darkerGrey);
        } 
        shop_console.printFrame(2, 26, 60, 23, false, TCOD_BKGND_SET, "self inventory");
        shop_console.printf(4, 47, "page: %i/%i", self_current_page, self_max_page);
        {
            int y = 28;
            
            if (!pointing_shop_or_self) {
                shop_console.setDefaultBackground(TCODColor::darkerGrey);
                shop_console.rect(4, current_pointing + y, 56, 1, true, TCOD_BKGND_SET);
                shop_console.setDefaultBackground(TCODColor::darkGrey);
            }
            
            for (int i = 0; i < game.player->inventory->getItemNum(); i++) {
                Entity *item = game.player->inventory->getIndexItem(i);
                shop_console.printf(4, y, "%s x %i", item->getName().c_str(), item->item_behavior->getQty());
                for (int buying_item_id : shop->buying_item) {
                    if (item->item_behavior->getItemId() == buying_item_id && item->item_behavior->tradable) {
                        shop_console.printf(4, y, "%s x %i buy for %i", item->getName().c_str(), 
                                            item->item_behavior->getQty(),
                                            int(item->item_behavior->tradable->price / SHOP_BUYING_VALUE_RATE));
                    }
                }
                
                if (item->item_behavior->getItemId() == item_dict::material_copper_chunk) {
                    shop_console.printf(66, 3, "%i", item->item_behavior->getQty());
                }
                
                y++;
            }
        }
        
        shop_console.setDefaultBackground(TCODColor::darkGrey);
        shop_console.setDefaultForeground(TCODColor::darkestGrey);
        shop_console.printFrame(65, 6, 32, 34, false, TCOD_BKGND_SET, "description");
        shop_console.printRect(67, 8, 28, 6, "%s", pointing_item->item_behavior->getDesc().c_str());
        if (pointing_shop_or_self) {
            delete pointing_item;
        }
        
        shop_console.printFrame(65, 41, 32, 8, false, TCOD_BKGND_SET, "usage");
        shop_console.printRect(67, 43, 28, 6,
                               "[TAB] toggle shop/self\n"
                               "[ENTER] buy/sell\n"
                               "[UP/DOWN] select item\n"
                               "[LEFT/RIGHT] switch page\n"
                               "[ESC] close\n");
        
        TCODConsole::blit(&shop_console, 0, 0 ,100 ,50, TCODConsole::root, 0, 0);
        TCODConsole::root->flush();
        
        TCODSystem::waitForEvent(TCOD_EVENT_KEY_RELEASE, &game.keyboard, NULL, false);
        
        if (game.keyboard.vk == TCODK_ESCAPE) {break;}
        
        if (game.keyboard.vk == TCODK_TAB) {
            pointing_shop_or_self = !pointing_shop_or_self;
            current_pointing = 0;
        }
        
        if (game.keyboard.vk == TCODK_UP) {
            if (current_pointing == 0) {continue;}
            current_pointing -= 1;
        }
        
        if (game.keyboard.vk == TCODK_DOWN) {
            int page_item_num = (pointing_shop_or_self)? shop_page_item_num : self_page_item_num;
            if (current_pointing >= page_item_num - 1) {continue;}
            current_pointing += 1;
        }
        
        if (game.keyboard.vk == TCODK_LEFT) {
            if (pointing_shop_or_self) {
                if (shop_current_page == 1) {continue;}
                shop_current_page -= 1;
            }
            else {
                if (self_current_page == 1) {continue;}
                self_current_page -= 1;
            }
        }
        
        if (game.keyboard.vk == TCODK_RIGHT) {
            if (pointing_shop_or_self) {
                if (shop_current_page == shop_max_page) {continue;}
                shop_current_page -= 1;
            }
            else {
                if (self_current_page == self_max_page) {continue;}
                self_current_page -= 1;
            }
        }
        
        if (game.keyboard.vk == TCODK_ENTER) {
            if (pointing_shop_or_self) {
                int index = current_pointing + (shop_current_page - 1) * PAGE_MAX_ITEM;
                Entity *to_sell = getItem(0, 0, shop->selling_item.at(index));
                doSell(to_sell, SHOP_SELLING_VALUE_RATE);
                
            }
            else {
                int index = current_pointing + (self_current_page - 1) * PAGE_MAX_ITEM;
                Entity *to_buy = game.player->inventory->getIndexItem(index);
                for (int shop_buying_id : shop->buying_item) {
                    if (to_buy->item_behavior->getItemId() == shop_buying_id) {
                        doBuy(to_buy, SHOP_BUYING_VALUE_RATE);
                    }
                }
            }
        }
        
        if (game.keyboard.vk == TCODK_NONE) {doCloseWindow();}
    }
    
}

void ShopInterface::doRefreshShopItem() {
    shop->selling_item.clear();
    for (int i = 1; i <= shop->max_item_count; i++) {
        shop->selling_item.push_back(game.global_rng->getInt(0, shop->all_item.size() - 1));
    }
}

bool ShopInterface::doBuy(Entity *to_buy, float multiply_value_by)  {
    if (!to_buy->item_behavior->tradable) {return false;}
    
    Entity *gold = getItem(0, 0, item_dict::material_copper_chunk);
    gold->item_behavior->setQty(int(to_buy->item_behavior->tradable->price * multiply_value_by));
    game.player->inventory->addItem(gold);
    
    game.player->inventory->deleteItem(to_buy);
    
    return true;
}

bool ShopInterface::doSell(Entity *to_sell, float multiply_value_by) {
    if (!to_sell->item_behavior->tradable) {return false;}
    
    int price = to_sell->item_behavior->tradable->price * multiply_value_by;
    for (int i = 0; i < game.player->inventory->getItemNum(); i++) {
        Entity *item = game.player->inventory->getIndexItem(i);
        if (item->item_behavior->getItemId() != item_dict::material_copper_chunk) {
            continue;
        }
        if (item->item_behavior->getQty() < price) {
            delete to_sell;
            return false;
        }
        game.player->inventory->deleteItem(item, price);
        game.player->inventory->addItem(to_sell);
        return true;
    }
}
