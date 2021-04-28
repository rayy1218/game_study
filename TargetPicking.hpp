#ifndef TARGETPICKING_HPP
#define TARGETPICKING_HPP

class ItemPurpose;

class TargetPicking {
protected:
    Entity *self;
public:

    TargetPicking(Entity *self);
    virtual ~TargetPicking();
    
    virtual bool doTargeting() = 0;
};

class SelfTarget : public TargetPicking {
public:
    SelfTarget(Entity *self);
    bool doTargeting();
};

class NearestTarget : public TargetPicking {
private:
    int range;
public:
    NearestTarget(Entity *self, int upper_bound_range);
    bool doTargeting();
};

class SelfAreaRandomTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelfAreaRandomTarget(Entity *self, int radius, bool include_self);
    bool doTargeting();
};

class SelfAreaAllTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelfAreaAllTarget(Entity *self, int radius, bool include_self);
    bool doTargeting();
};

class SelectAreaRandomTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelectAreaRandomTarget(Entity *self, int radius, bool include_self);
    bool doTargeting();
};

class SelectAreaAllTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelectAreaAllTarget(Entity *self, int radius, bool include_self);
    bool doTargeting();
};

class SelectLineAllTarget : public TargetPicking {
private:
    int range;
public:
    SelectLineAllTarget(Entity *self, int range);
    bool doTargeting();
};

#endif /* TARGETPICKING_HPP */

