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

#include "MainMenuScene.h"
#include "LEVEL1.h"
#include "LEVEL2.h"
#include "LEVEL3.h"
#include "Difinitions.h"
#include "PauseGame.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenuScene::create();
    scene->addChild(layer);
    return scene;
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in Level1Scene.cpp\n");
}



bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto backgroundSprite = Sprite::create("background/BgMenu1.png");
    backgroundSprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    this->addChild(backgroundSprite);

    auto playItem = MenuItemFont::create("Play", CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));
    playItem->setColor(cocos2d::Color3B(255, 134, 77));
    playItem->setFontName("fonts/Marker Felt.ttf");
    playItem->setFontSize(30);
    playItem->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.6));

    auto menu = Menu::create(playItem, NULL);
    menu->setPosition(Point::ZERO);

    this->addChild(menu);

    
    auto closeItem = MenuItemFont::create("Exit", CC_CALLBACK_1(MainMenuScene::menuCloseCallback, this));
    closeItem->setColor(cocos2d::Color3B(255, 134, 77));
    closeItem->setFontName("fonts/Marker Felt.ttf");
    closeItem->setFontSize(30);
    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'Exit' and 'Exit'");
    }
    else
    {
        float x = origin.x + visibleSize.width / 2;
        float y = origin.y + visibleSize.height * 0.4;
        closeItem->setPosition(Vec2(x, y));
    }

    // create menu, it's an autorelease object
    auto menu1 = Menu::create(closeItem, NULL);
    menu1->setPosition(Vec2::ZERO);
    this->addChild(menu1, 1);


    return true;
}


void MainMenuScene::GoToGameScene(Ref* sender)
{
    auto scene = LEVEL1::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MainMenuScene::menuCloseCallback(Ref* pSender)
{
    CCLOG("Exit");
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);
}
