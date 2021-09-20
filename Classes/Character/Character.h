#pragma once
#include "cocos2d.h"

class Character
{
public:
    enum class CharacterState
    {
        idle,
        walk,
        attack
    };

    Character();

    void onMouseDown(cocos2d::EventMouse* e);
    
    inline cocos2d::Sprite* getSprite() { return sprite; }
private:
    void initialize();
    
    cocos2d::Sprite* sprite = nullptr;
    cocos2d::Action* moveToAction = nullptr;
};
