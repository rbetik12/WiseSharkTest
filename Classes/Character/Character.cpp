#include "Character.h"

Character::Character()
{
    initialize();
}

void Character::onMouseDown(cocos2d::EventMouse* mouse)
{
    cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(1, cocos2d::Vec2(mouse->getCursorX(), mouse->getCursorY()));

    if (moveToAction)
    {
        sprite->stopAction(moveToAction);
    }

    moveToAction = sprite->runAction(moveTo);
}

void Character::initialize()
{
    auto director = cocos2d::Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    cocos2d::Vec2 origin = director->getVisibleOrigin();
    
    sprite = cocos2d::Sprite::create("HelloWorld.png");
    sprite->setPosition(cocos2d::Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    auto activeScene = director->getRunningScene();
    activeScene->addChild(sprite);
}
