#include "Character.h"

#include "Match.h"

Character::Character()
{
    initialize();
}

void Character::update(float delta)
{
    CharacterAction* action = nullptr;
    if (!actions.empty())
    {
        action = actions.front();
        actions.pop_front();
    }
    state = match(state,
                  [&](Idle& state) -> State
                  {
                      if (action)
                      {
                          if (action->actionDescription == CharacterActionDescription::Walk)
                          {
                              auto walkAction = static_cast<Walk*>(action);
                              return State{Walk{walkAction->time, walkAction->destination}};
                          }
                          if (action->actionDescription == CharacterActionDescription::Attack)
                          {
                              auto attackAction = static_cast<Attack*>(action);
                              return State{Attack{}};
                          }
                      }

                      return State{Idle{}};
                  },

                  [&](Walk& state) -> State
                  {
                      cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(state.time, state.destination);

                      if (spriteMoveToAction)
                      {
                          sprite->stopAction(spriteMoveToAction);
                      }

                      spriteMoveToAction = sprite->runAction(moveTo);
                      return State{Idle{}};
                  },

                  [&](Attack& state) -> State
                  {
                      return State{Idle{}};
                  }
    );

    delete action;
}

void Character::addAction(CharacterAction* action)
{
    actions.push_back(action);
}

// void Character::onMouseDown(cocos2d::EventMouse* mouse)
// {
//     cocos2d::MoveTo* moveTo = cocos2d::MoveTo::create(1, cocos2d::Vec2(mouse->getCursorX(), mouse->getCursorY()));
//
//     if (moveToAction)
//     {
//         sprite->stopAction(moveToAction);
//     }
//
//     moveToAction = sprite->runAction(moveTo);
// }

void Character::initialize()
{
    auto director = cocos2d::Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    cocos2d::Vec2 origin = director->getVisibleOrigin();

    sprite = cocos2d::Sprite::create("HelloWorld.png");
    sprite->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

    auto activeScene = director->getRunningScene();
    activeScene->addChild(sprite);

    state = State{Idle{}};
}
