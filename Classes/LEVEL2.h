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
#pragma once

#include "cocos2d.h"
__CCTMX_TILE_MAP_H__
__CCTMX_LAYER_H__


class LEVEL2 : public cocos2d::Scene
{

public:
    static cocos2d::Scene* createScene();

    virtual bool init() override;

    void Play(Ref* pSender);

    CREATE_FUNC(LEVEL2);
    void MenuPause(cocos2d::Ref* sender);
    void GoToGameWin(cocos2d::Ref* sender);
    void GoToGameLEVEL1(cocos2d::Ref* sender);
    void GoToGameHome(cocos2d::Ref* sender);
    void NomLEVEL1(float dt);
private:
    cocos2d::PhysicsWorld* sceneWorld;

    void SetPhysicsWorld(cocos2d::PhysicsWorld* world) { sceneWorld = world; };

};

