/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "ui/UIButton.h"
#include "ui/UILayout.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf(
        "Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

static ui::Button* createButton()
{
    auto button = ui::Button::create(
        "button.png",
        "button_clicked.png");
    button->ignoreContentAdaptWithSize(false);
    button->setContentSize(Size(200, 100));
    button->setTitleFontSize(35);
    if (button == nullptr ||
        button->getContentSize().width <= 0 ||
        button->getContentSize().height <= 0)
    {
        problemLoading("'button.png' and 'button_clicked.png'");
        return nullptr;
    }

    return button;
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    this->setOnEnterCallback(CC_CALLBACK_0(HelloWorld::onSceneReady, this));
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);


    auto stopButton = createButton();
    if (!stopButton)
    {
        return false;
    }

    stopButton->setTitleText("Stop");
    stopButton->addClickEventListener([&](Ref* sender)
        {
            character->addAction(new Idle());
        }
    );

    auto attackButton = createButton();
    if (!attackButton)
    {
        return false;
    }

    attackButton->setTitleText("Attack");
    attackButton->addClickEventListener([&](Ref* sender)
        {
            character->addAction(new Attack());
        }
    );

    mainUiLayout = ui::Layout::create();
    mainUiLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    mainUiLayout->setTouchEnabled(true);
    mainUiLayout->setSwallowTouches(true);
    mainUiLayout->setPosition(Vec2(0, 0));
    mainUiLayout->addChild(stopButton);
    mainUiLayout->addChild(attackButton);
    mainUiLayout->setContentSize(Size(stopButton->getContentSize().width + attackButton->getContentSize().width, stopButton->getContentSize().height));

    this->addChild(mainUiLayout, 100);

    this->scheduleUpdate();
    return true;
}

void HelloWorld::onMouseDown(Event* event)
{
    // to illustrate the event....
    EventMouse* e = (EventMouse*)event;
    Vec2 clickLocation = Vec2(e->getCursorX(), e->getCursorY());
    Rect box = mainUiLayout->getBoundingBox();
    if (box.containsPoint(clickLocation))
    {
        return;
    }
    character->addAction(new Walk(1, cocos2d::Vec2(e->getCursorX(), e->getCursorY())));
}

void HelloWorld::onSceneReady()
{
    character = new Character();
}

void HelloWorld::update(float delta)
{
    Scene::update(delta);
    character->update(delta);
}


void HelloWorld::stopButtonCallback(Ref* pSender)
{
    character->addAction(new Idle());
}
