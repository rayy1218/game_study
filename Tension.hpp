#ifndef TENSION_HPP
#define TENSION_HPP

class Tension {
private:
    int current_tension, max_tension;
public:
    Tension(int max_tension);

    int getCurrentTension();
    int getMaxTension();
    void setCurrentTension(int input);
    void setMaxTension(int input);
    
    int doDropTension(int tension_drop);
    int doRiseTension(int tension_rise);
    void doUpdateTension();
    void doUpdateTensionEffect();
    int getMonsterNumInFov();
};

#endif /* TENSION_HPP */

