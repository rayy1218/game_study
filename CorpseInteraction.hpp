#ifndef CORPSEINTERACTION_HPP
#define CORPSEINTERACTION_HPP

class CorpseHarvest {
private:
    Entity *self;
public:    
    std::vector<Entity*> harvest_item;
    
    CorpseHarvest(Entity *self);
    ~CorpseHarvest();
    
    bool doUse(int index);
    int doRenderContainer();
};

class CorpseLooting {
private:
    Entity *self;
public:
    CorpseLooting(Entity *self);
    
    bool doUse(int index);
    int doRenderContainer();
};

class CorpseBurn {
public:
    bool doUse(Entity *corpse);
};

class CorpseRecall {
private:
    int success_chance;
public:
    CorpseRecall(int success_chance);
    
    bool doUse();
};

class CorpseInteraction {
private:
    Entity *self;
    CorpseHarvest *corpse_harvest;
    CorpseLooting *corpse_looting;
    CorpseBurn *corpse_burn;
    CorpseRecall *corpse_recall;
public:
    
    CorpseInteraction(Entity *self,
                      CorpseHarvest *corpse_harvest,
                      CorpseLooting *corpse_looting,
                      CorpseBurn *corpse_burn,
                      CorpseRecall *corpse_recall);
    ~CorpseInteraction();
    
    void addHasvestableItem(Entity *item, int max_qty, int min_qty);
    void addLootableItem(Entity *item, int max_qty, int min_qty);
    
    bool doRenderCorpseInteraction();
    int getIndexFromInventory();
    void doRenderContainer();
};



#endif /* CORPSEINTERACTION_HPP */

