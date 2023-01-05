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

#include "PauseGame.h"
#include "LEVEL1.h"
#include "MainMenuScene.h"
#include "Difinitions.h"

USING_NS_CC;

Scene* PauseGame::createScene()
{
    auto scene = Scene::create();
    auto layer = PauseGame::create();
    scene->addChild(layer);
    return scene;
}

bool PauseGame::init()
{
    if (!Scene::init())
    {
        return false;
    }

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto over = Sprite::create("Bgover.png");
    over->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    addChild(over);

    auto restart = MenuItemImage::create("menu/restart.png", "menu/restartSelected.png", CC_CALLBACK_1(PauseGame::GoToGameLEVEL1, this));
    auto home = MenuItemImage::create("menu/home.png", "menu/homeSelected.png", CC_CALLBACK_1(PauseGame::GoToGameHome, this));

    //auto complite = MenuItemImage::create("menu/complite.png", "menu/compliteSelected.png", CC_CALLBACK_0(GameOverScene::Complite, this));

    auto menuPause = Menu::create(home, restart, NULL);
    this->addChild(menuPause);

    menuPause->alignItemsHorizontally();
    menuPause->alignItemsHorizontallyWithPadding(80);
    return true;
}


void PauseGame::GoToGameLEVEL1(Ref* sender)
{
    auto scene = LEVEL1::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}


void PauseGame::GoToGameHome(Ref* sender)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}
/*

void PauseGame::Complite()
{
    Director::getInstance()->popScene();
}
*/