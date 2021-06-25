#ifndef ITEM_HPP
#define ITEM_HPP

class Purpose;
class TargetPicking;

struct EquipmentAttribute {
    int defense, speed;
    
    EquipmentAttribute(int defense, int speed);
};

struct WeaponAttribute {
    int attack, speed;
    
    WeaponAttribute(int attack, int speed);
};

struct Tradable {
    int price;
    
    Tradable(int price);
};

class ItemBehavior {
protected:
    //Attribute
    Entity *self;
    int item_id;
    int qty;
    bool stackable;
    float weight;
    std::string description;
public:
    //Attribute
    Tradable *tradable;
    Purpose *purpose;
    TargetPicking *targeting;
    
    //Constructor & Destructor
    ItemBehavior(Entity *self, float weight, int qty, bool stackable);
    virtual ~ItemBehavior();
    
    //Method
    bool pick(Entity *pick_by);
    virtual bool use(Entity *use_by);
    
    //Accessor
    int getItemId();
    void setItemId(int input);
    void setQty( int input );
    void setWeight( float input );
    bool isStackable();
    int getQty();
    float getWeight();
    std::string getDesc();
    void setDesc(std::string desc);
    virtual void setIsEquip(bool input);
    virtual bool getIsEquip();
    virtual int getEquipmentIndex();
    virtual void doUpdateEquipmentAttribute(Entity *self);
};

class ItemEquipmentBehavior : public ItemBehavior{
private:
    int equipment_index;
    bool is_equiped;
    EquipmentAttribute *equipment_attribute;
    WeaponAttribute *weapon_attribute;
public:
    ItemEquipmentBehavior(Entity *self, float weight, int qty, bool stackable,
                          int equipment_index = 0,
                          EquipmentAttribute *equipment_attribute = nullptr,
                          WeaponAttribute *weapon_attribute = nullptr);
    
    ~ItemEquipmentBehavior();
    
    bool use(Entity *use_by);
    
    void setIsEquip(bool input);
    bool getIsEquip();
    int getEquipmentIndex();
    void doUpdateEquipmentAttribute(Entity *self);
};

#endif /* ITEM_HPP */

