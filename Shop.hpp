#ifndef SHOP_HPP
#define SHOP_HPP

struct Shop {
    int max_item_count;
    std::vector<int> selling_item, buying_item, all_item;
};

class ShopInterface {
private:
    Shop *shop;
public:
    ShopInterface(Shop *shop);
    
    void doRenderShop();
    void doRefreshShopItem();
    bool doBuy(Entity *to_sell, float multiply_value_by);
    bool doSell(Entity *to_buy, float multiply_value_by);
};

#endif /* SHOP_HPP */

