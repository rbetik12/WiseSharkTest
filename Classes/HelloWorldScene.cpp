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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }


    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    auto stopButton = ui::Button::create(
        "button.png",
        "button_clicked.png");
    // add a "close" icon to exit the progress. it's an autorelease object
    stopButton->setTitleText("Stop");
    stopButton->setTitleFontSize(30);
    if (stopButton == nullptr ||
        stopButton->getContentSize().width <= 0 ||
        stopButton->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - stopButton->getContentSize().width / 2;
        float y = origin.y + stopButton->getContentSize().height / 2;
        // stopButton->setPosition(Vec2(x, y));
    }

    stopButton->addClickEventListener([&](Ref* sender)
    {
        character->addAction(new Idle());
    });

    // create menu, it's an autorelease object
    // auto menu = Menu::create(stopButton, NULL);
    // menu->setPosition(Vec2::ZERO);
    // this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    this->setOnEnterCallback(CC_CALLBACK_0(HelloWorld::onSceneReady, this));
    
    mainUiLayout = cocos2d::ui::Layout::create();
    mainUiLayout->setLayoutType(ui::Layout::Type::HORIZONTAL);
    mainUiLayout->setTouchEnabled(true);
    mainUiLayout->setSwallowTouches(true);
    mainUiLayout->setPosition(Vec2(200, 200));
    mainUiLayout->addChild(stopButton);
    mainUiLayout->setContentSize(stopButton->getContentSize());

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
