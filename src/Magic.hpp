#ifndef MAGIC_HPP
#define MAGIC_HPP

class Casting;

const int CASTING_NUM = 3;

enum casting_dict {
    firebolt,
    lighting_chain,
    windblade,
};

class Magic {
private:
    int max_mana_point, mana_point;
    bool is_casting_unlock[CASTING_NUM];
    std::vector<Casting*> available_casting;
public:
    Magic(int mana_point);
    bool doRenderCastingList();
    bool doCastMagic(int casting_magic);
    void setCastingUnlock(int casting, bool is_unlock);
    int doMpRegen(int amount);

    int getMaxMp();
    int getMp();
    void setMaxMp(int input);
    void setMp(int input);
    int isCastingUnlock(int casting);
};

#endif //MAGIC_HPP
