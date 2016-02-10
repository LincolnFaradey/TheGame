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

    bool operator==(const Spaceship& rhs) const {
        return this->_name.compare(rhs.name()) == 0;
    }

    bool operator<(const Spaceship& rhs) const {
        return this->_name.compare(rhs.name()) < 0;
    }

    std::string name() const {
        return _name;
    }
    void moveTo(Vec2 dest);
};

//struct SpaceshipLessThan : public std::binary_function<Spaceship *, Spaceship *, bool>
//{
//    bool operator()(Spaceship* lhs, Spaceship* rhs) const
//    {
//        cocos2d::log("Comparing... comparator");
//        return *lhs != *rhs;
//    }
//};