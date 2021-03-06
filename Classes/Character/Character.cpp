#include "Character.h"

#include <iostream>

#include "Match.h"

Character::Character()
{
    initialize();
}

static void animListener(spAnimationState* state, spEventType type, spTrackEntry* entry, spEvent* event)
{
    if (type == SP_ANIMATION_COMPLETE)
    {
        entry->animationStart = -1;
    }
}

void Character::update(float delta)
{
    checkActionQueue();
    state = match(state,
                  [&](Idle& state) -> State
                  {
                      auto animationName = characterSpine->getState()->tracks[0]->animation->name;
                      if (strcmp(animationName, "idle"))
                      {
                          characterSpine->setAnimation(0, "idle", true);
                      }

                      moveSpeed = initMoveSpeed;

                      return State{Idle{}};
                  },

                  [&](Walk& state) -> State
                  {
                      auto animationName = characterSpine->getState()->tracks[0]->animation->name;
                      if (strcmp(animationName, "move"))
                      {
                          characterSpine->setAnimation(0, "move", true);
                      }

                      cocos2d::Vec2 currentPosition = characterSpine->getPosition();
                      float distance = state.destination.distance(currentPosition);

                      if (distance <= 10.0f)
                      {
                          if (moveSpeed >= 10.0f)
                          {
                              moveSpeed /= 2;
                          }
                      }
                      
                      if (distance <= 1.0f)
                      {
                          return State{Idle{}};
                      }

                      cocos2d::Vec2 direction = state.destination - characterSpine->getPosition();
                      direction.normalize();

                      cocos2d::Vec2 orig(1, 0);
                      orig.normalize();
                      float cos = ((direction.x * orig.x) + (direction.y * orig.y)) / (direction.length() * orig.
                          length());

                      if (cos >= 0)
                      {
                          characterSpine->setScaleX(-1);
                      }
                      else
                      {
                          characterSpine->setScaleX(1);
                      }

                      cocos2d::Vec2 newPosition = direction * delta * moveSpeed + currentPosition;

                      characterSpine->setPosition(newPosition);

                      return State{state};
                  },

                  [&](Attack& state) -> State
                  {
                      auto animationName = characterSpine->getState()->tracks[0]->animation->name;
                      if (strcmp(animationName, "attack"))
                      {
                          auto animation = characterSpine->setAnimation(0, "attack", false);
                          animation->listener = &animListener;
                      }
                      else if (strcmp(animationName, "attack") == 0 && characterSpine->getState()->tracks[0]->animationStart == -1)
                      {
                          return State{Idle{}};
                      }
                      
                      return State{Attack{}};
                  }
    );
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

    characterSpine = spine::SkeletonAnimation::createWithJsonFile("hero_2.json", "hero_2.atlas", 2.0f);
    characterSpine->setAnimation(0, "idle", true);
    characterSpine->setPosition(cocos2d::Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    characterSpine->setMix("idle", "move", 0.1);
    characterSpine->setMix("move", "idle", 0.1);
    activeScene->addChild(characterSpine);
}

void Character::checkActionQueue()
{
    CharacterAction* action = nullptr;

    if (!actions.empty())
    {
        action = actions.front();
        actions.pop_front();

        if (action)
        {
            moveSpeed = initMoveSpeed;
            if (action->actionDescription == CharacterActionDescription::Idle)
            {
                state = State{Idle{}};
            }
            if (action->actionDescription == CharacterActionDescription::Walk)
            {
                auto walkAction = static_cast<Walk*>(action);
                state = State{Walk{walkAction->time, walkAction->destination}};
            }
            if (action->actionDescription == CharacterActionDescription::Attack)
            {
                auto attackAction = static_cast<Attack*>(action);
                state = State{Attack{}};
            }
        }
    }
}
