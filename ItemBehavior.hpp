#ifndef ITEM_HPP
#define ITEM_HPP

class ItemPurpose;
class TargetPicking;
class ItemEquipment;

class ItemBehavior {
protected:
    Entity *self;
    int qty;
    bool stackable;
    float weight;
    int equipment_index;
    bool is_equiped;
    std::string description;
public:
    TCODList<ItemPurpose*> all_purpose;
    TargetPicking *targeting;
    ItemBehavior(Entity *self, float weight, int qty, bool stackable, int equipment_index = 0);
    virtual ~ItemBehavior();
    
    void setQty( int input );
    void setWeight( float input );
    bool isStackable();
    int getQty();
    float getWeight();
    void setIsEquip(bool input);
    bool getIsEquip();
    int getEquipmentIndex();
    std::string getDesc();
    void setDesc(std::string desc);
    
    bool pick(Entity *pick_by);
    virtual bool use(Entity *use_by);
};

class ItemEquipmentBehavior : public ItemBehavior{
public:
    ItemEquipmentBehavior(Entity *self, float weight, int qty, bool stackable, int equipment_index = 0);
    
    bool use(Entity *use_by);
};

#endif /* ITEM_HPP */

