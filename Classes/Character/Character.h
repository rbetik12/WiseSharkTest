#pragma once
#include "cocos2d.h"
#include <variant>

#include "spine/SkeletonAnimation.h"

enum class CharacterActionDescription
{
    Idle,
    Walk,
    Attack
};

struct CharacterAction
{
    CharacterAction(CharacterActionDescription actionDescription): actionDescription(actionDescription)
    {
    }

    CharacterActionDescription actionDescription;
};

struct Idle : public CharacterAction
{
    explicit Idle(): CharacterAction(CharacterActionDescription::Idle) {}
};

struct Walk : public CharacterAction
{
    explicit Walk(double time, cocos2d::Vec2 destination): destination(destination), time(time),
                                                            moveToAction(nullptr), CharacterAction(CharacterActionDescription::Walk)
    {
    }

    cocos2d::Vec2 destination;
    double time;
    cocos2d::Action* moveToAction;
};

struct Attack : public CharacterAction
{
    explicit Attack(): CharacterAction(CharacterActionDescription::Attack) {}
};

using State = std::variant<Idle, Walk, Attack>;

class Character
{
public:
    Character();
    void update(float delta);
    void addAction(CharacterAction* action);
private:
    void initialize();
    void checkActionQueue();
    
    cocos2d::Action* spriteMoveToAction = nullptr;
    State state;
    std::deque<CharacterAction*> actions;
    spine::SkeletonAnimation *characterSpine = nullptr;
    
    const float initMoveSpeed = 500.0f;
    float moveSpeed = initMoveSpeed;
};
