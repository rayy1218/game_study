/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Hunger.hpp
 * Author: HP
 *
 * Created on May 13, 2021, 1:49 PM
 */

#ifndef HUNGER_HPP
#define HUNGER_HPP

class Hunger {
private:
    int current_hunger_point, max_hunger_point;
public:
    Hunger(int max_hunger_point);
    
    int getCurrentHungerPoint();
    int getMaxHungerPoint();
    void setCurrentHungerPoint(int input);
    void setMaxHungerPoint(int input);
    
    int doEat(int eat_hunger_point);
    void doHungerDrop(int drop_hunger_point);
    void doUpdateHungerEffect();
};

#endif /* HUNGER_HPP */

