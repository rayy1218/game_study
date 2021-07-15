#ifndef CASTING_HPP
#define CASTING_HPP

class Casting {
public:
    int mana_cost;
    std::string name;
    TargetPicking *target_picking;
    std::vector<Purpose*> purpose_list;

    bool doCast();
};

#endif //CASTING_HPP
