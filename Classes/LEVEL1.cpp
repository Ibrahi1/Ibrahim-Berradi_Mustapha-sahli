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

#include "LEVEL1.h"
#include "LEVEL2.h"
#include "LEVEL3.h"
#include "MainMenuScene.h"
#include "Difinitions.h"
#include "PauseGame.h"

USING_NS_CC;


Scene* LEVEL1::createScene()
{

    auto scene = Scene::createWithPhysics();
    // scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    //scene->getPhysicsWorld()->setGravity( Vec2( 0, 0 ) );

    // 'layer' is an autorelease object
    auto layer = LEVEL1::create();
    layer->SetPhysicsWorld(scene->getPhysicsWorld());

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;

}

bool LEVEL1::init()
{
    if (!Scene::init())
    {
        return false;
    }


    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(LEVEL1::NomLEVEL1), DISPLAY_TIME_SPLASH_SCENE);
    
    //Background***************************************

    auto bg = Sprite::create("BgGame.png");
    bg->setPosition(Vec2(visibleSize * 0.5f) + origin);
    bg->setScale(visibleSize.width / bg->getContentSize().width);
    bg->getTexture()->setAliasTexParameters();
    addChild(bg);

    //Physics***************************************
    auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 30);

    auto edgeNode = Node::create();
    edgeNode->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.51 + origin.y));
    edgeNode->setPhysicsBody(edgeBody);

    this->addChild(edgeNode);
    //Map**********************************************

    auto map = TMXTiledMap::create("stage_00.tmx");
    map->setName("MAP");
    map->getLayer("grid")->setVisible(DEBUGMODE);
    addChild(map);

    map->runAction(RepeatForever::create(
        MoveBy::create(0.3f, Vect(-16, 0))
    ));

    //Player********************************************




    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("big1.png", Rect(0, 0, 43.5, 48)), "WITCH0");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("big1.png", Rect(58, 0, 43.5, 48)), "WITCH1");


    auto witch = Sprite::createWithSpriteFrameName("WITCH0");
    witch->getTexture()->setAliasTexParameters();
    witch->setPosition(0,55);
    witch->setAnchorPoint(Vec2(0.6f, 0.3f));
    witch->setScale(0.75f);

    //Physic Player ********************************

    auto spriteBody = PhysicsBody::createBox(witch->getContentSize(), PhysicsMaterial(0, NULL, NULL));
    spriteBody->setDynamic(true);
    spriteBody->setCollisionBitmask(1);
    spriteBody->setContactTestBitmask(true);
    witch->setPhysicsBody(spriteBody);
    this->addChild(witch);



    Vector<SpriteFrame*>witchAnim;
    witchAnim.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("WITCH0"));
    witchAnim.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("WITCH1"));

    witch->runAction(RepeatForever::create(
        Animate::create(Animation::createWithSpriteFrames(witchAnim, 0.25f))
    ));
    witch->runAction(MoveTo::create(2.0f, Vec2(80, 96)));

    //Center****************************************
    auto reddot = Sprite::create("reddot.png");
    reddot->setPosition(witch->getPosition());
    reddot->getTexture()->setAliasTexParameters();
    addChild(reddot, 1);
    reddot->schedule([=](float dt) {
        reddot->setPosition(witch->getPosition());
        }, "REDDOT");
    reddot->setVisible(DEBUGMODE);

    //Physics***************************************

    auto pause = MenuItemImage::create("menu/pause.png", "menu/pause.png", CC_CALLBACK_1(LEVEL1::MenuPause, this));
    pause->setPosition(Point(visibleSize.width * 0.96, visibleSize.height * 0.9));
    auto menupause = Menu::create(pause, NULL);
    menupause->setPosition(Point::ZERO);
    this->addChild(menupause);

    //Movement**************************************
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [](EventKeyboard::KeyCode keyCode, Event* event) {
        auto avatar = static_cast<Sprite*>(event->getCurrentTarget());
        Vec2 loc = event->getCurrentTarget()->getPosition();
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            avatar->getPhysicsBody()->applyImpulse(Vec2(-50, 0));

            break;
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            avatar->getPhysicsBody()->applyImpulse(Vec2(50, 0));

            break;
        case EventKeyboard::KeyCode::KEY_UP_ARROW:
            avatar->getPhysicsBody()->applyImpulse(Vec2(0, 50));

            break;
        case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
            avatar->getPhysicsBody()->applyImpulse(Vec2(0, -50));
            break;
        case EventKeyboard::KeyCode::KEY_V:
            event->getCurrentTarget()->setPosition(0, 0);
            break;
        }};

    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, witch);
    


    //Dynamique*************************************

    witch->schedule([=](float dt) {
        auto obsLayer = map->getLayer("car");
        auto itemlayer = map->getLayer("item");
        auto winlayer = map->getLayer("win");
        auto pos = witch->getPosition();
        auto tileSize = map->getTileSize();



        int tileX = (int)((pos.x - map->getPosition().x) / tileSize.width);
        int tileY = (int)((map->getContentSize().height - pos.y + map->getPosition().y) / tileSize.height);

        if ((0 <= tileX && tileX < map->getMapSize().width) && (0 <= tileY && tileY < map->getMapSize().height))
        {
            auto gid = obsLayer->getTileGIDAt(Vec2(tileX, tileY));
            auto gid2 = itemlayer->getTileGIDAt(Vec2(tileX, tileY));
            auto gid3 = winlayer->getTileGIDAt(Vec2(tileX, tileY));
            if (gid == 7)
            {
                log("Game Over");
                witch->stopAllActions();
                witch->unschedule("WITCH");
                map->unschedule("MAP");
                map->stopAllActions();
                reddot->unschedule("REDDOT");


                auto over = Sprite::create("gameoverlogo.png");
                over->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.7));
                addChild(over);

                auto restart = MenuItemImage::create("menu/restart.png", "menu/restartSelected.png", CC_CALLBACK_1(LEVEL1::GoToGameLEVEL1, this));

                auto home = MenuItemImage::create("menu/home.png", "menu/homeSelected.png", CC_CALLBACK_1(LEVEL1::GoToGameHome, this));

                auto menuover = Menu::create(home, restart, NULL);
                this->addChild(menuover);

                menuover->alignItemsHorizontally();
                menuover->alignItemsHorizontallyWithPadding(60);

            }
            if (gid2 == 3)
            {
                log("get heart");
                itemlayer->setTileGID(1, Vec2(tileX, tileY));
            }
            if (gid3 == 5 || gid3 == 6 || gid3 == 11 || gid3 == 12)
            {
                log("you win");
                witch->stopAllActions();
                witch->unschedule("WITCH");
                map->unschedule("MAP");
                map->stopAllActions();
                reddot->unschedule("REDDOT");

                auto win = Sprite::create("win.png");
                win->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.8));
                addChild(win);

                auto next = MenuItemImage::create("menu/next.png", "menu/nextSelected.png", CC_CALLBACK_1(LEVEL1::GoToGameWin, this));

                auto restart = MenuItemImage::create("menu/restart.png", "menu/restartSelected.png", CC_CALLBACK_1(LEVEL1::GoToGameLEVEL1, this));

                auto home = MenuItemImage::create("menu/home.png", "menu/homeSelected.png", CC_CALLBACK_1(LEVEL1::GoToGameHome, this));

                auto menuWin = Menu::create(home, restart, next, NULL);
                this->addChild(menuWin);

                menuWin->alignItemsHorizontally();
                menuWin->alignItemsHorizontallyWithPadding(60);

            }
            
        }
        }, "WITCH");

    return true;

}


void LEVEL1::GoToGameLEVEL1(Ref* sender)
{
    auto scene = LEVEL1::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}


void LEVEL1::GoToGameHome(Ref* sender)
{
    auto scene = MainMenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}


void LEVEL1::GoToGameWin(Ref* sender)
{
    auto scene = LEVEL2::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME_1, scene));
}

void LEVEL1::MenuPause(Ref* sender)
{
    auto scene = PauseGame::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME_2, scene));
}




void LEVEL1::NomLEVEL1(float dt)
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto Level1Label = Label::createWithTTF("Level 1", "fonts/Marker Felt.ttf", 15);
    Level1Label->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height * 0.9));
    Level1Label->setColor(cocos2d::Color3B(255, 134, 77));
    addChild(Level1Label);
}

