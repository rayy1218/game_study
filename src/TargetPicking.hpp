#ifndef TARGETPICKING_HPP
#define TARGETPICKING_HPP

class Purpose;

class TargetPicking {
public:
    std::vector<Entity*> all_target;
    
    virtual bool doTargeting() = 0;
};

class SelfTarget : public TargetPicking {
public:
    bool doTargeting();
};

class NearestTarget : public TargetPicking {
private:
    int range;
public:
    NearestTarget(int upper_bound_range);
    bool doTargeting();
};

class SelfAreaRandomTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelfAreaRandomTarget(int radius, bool include_self);
    bool doTargeting();
};

class SelfAreaAllTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelfAreaAllTarget(int radius, bool include_self);
    bool doTargeting();
};

class SelectAreaRandomTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelectAreaRandomTarget(int radius, bool include_self);
    bool doTargeting();
};

class SelectAreaAllTarget : public TargetPicking {
private:
    int radius;
    bool include_self;
public:
    SelectAreaAllTarget(int radius, bool include_self);
    bool doTargeting();
};

class SelectLineAllTarget : public TargetPicking {
private:
    int range;
public:
    SelectLineAllTarget(int range);
    bool doTargeting();
};

#endif /* TARGETPICKING_HPP */

