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
                      auto animationName = skeletonNode->getState()->tracks[0]->animation->name;
                      if (strcmp(animationName, "idle"))
                      {
                          skeletonNode->setAnimation(0, "idle", true);
                      }
                      if (action)
                      {
                          if (action->actionDescription == CharacterActionDescription::Idle)
                          {
                              skeletonNode->stopAllActions();
                              return State{Idle{}};
                          }
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
                          skeletonNode->stopAction(spriteMoveToAction);
                      }

                      spriteMoveToAction = skeletonNode->runAction(moveTo);
                      // skeletonNode->setAnimation(2, "walk", true);
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

void Character::initialize()
{
    auto director = cocos2d::Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    cocos2d::Vec2 origin = director->getVisibleOrigin();

    auto activeScene = director->getRunningScene();

    state = State{Idle{}};

    skeletonNode = spine::SkeletonAnimation::createWithJsonFile("hero_2.json", "hero_2.atlas", 2.0f);
    skeletonNode->setAnimation(0, "idle", true);
    skeletonNode->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    activeScene->addChild(skeletonNode);
}
