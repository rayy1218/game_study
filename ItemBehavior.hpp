#ifndef ITEM_HPP
#define ITEM_HPP

class ItemPurpose;
class TargetPicking;

class ItemBehavior {
protected:
    Entity *self;
    int qty, max_stack;
    float weight;
public:
    TCODList<ItemPurpose*> all_purpose;
    TargetPicking *targeting;
    ItemBehavior(Entity *self, float weight, int qty, int max_stack = 1);
    virtual ~ItemBehavior();
    
    void setMaxStack( int input );
    void setQty( int input );
    void setWeight( float input );
    int getMaxStack();
    int getQty();
    float getWeight();
    
    bool pick(Entity *pick_by);
    bool use(Entity *use_by);
};

#endif /* ITEM_HPP */

