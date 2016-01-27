//
//  Spaceship.hpp
//  TheGame
//
//  Created by Andrei Nechaev on 1/25/16.
//
//

#pragma once

#include <stdio.h>
#include "cocos2d.h"


using cocos2d::Sprite;
using cocos2d::Director;
using cocos2d::Vec2;

class Spaceship: public Sprite {

private:
    std::string _name;
    Spaceship(const std::string &name);

public:
    virtual ~Spaceship();
    static Spaceship* create(const std::string& name, const std::string& filename);

    int Compare(const Spaceship& sp) const;

    bool operator()(Spaceship *lhs, Spaceship *rhs) const {
        cocos2d::log("Comparing... ()");
        return lhs->_name != rhs->_name;
    }

//    bool operator==(const Spaceship& rhs) const {
//        cocos2d::log("Comparing... ==");
//        return Compare(rhs) == 0;
//    }

    bool operator<(Spaceship& rhs) const {
        cocos2d::log("Comparing... <");
        return Compare(rhs) < 0;
    }


    std::string& getShipName();
    void moveTo(Vec2 dest);
};

struct SpaceshipLessThan : public std::binary_function<Spaceship *, Spaceship *, bool>
{
    bool operator()(Spaceship* lhs, Spaceship* rhs) const
    {
        return lhs->getShipName() != rhs->getShipName();
    }
};

//bool operator==(Spaceship& rhs, Spaceship& lhs) {
//    cocos2d::log("Comparing... ==");
//    return rhs.Compare(lhs) == 0;
//}