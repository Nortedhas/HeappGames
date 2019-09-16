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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Scene
{
public:

    static cocos2d::Scene* createScene();
    //переменные
    int lifes=3;
    int enemiesNumber=0;
    int enemiesNumberReverse=enemies.size();
    int kills=0;
    //целочисленное значени для функции RandomHelper
    int height=Director::getInstance()->getVisibleSize().height;

    bool isPowerUp=false;
    bool isCreatePowerUp=false;
    bool shoot=true;


    //Спрайты
    cocos2d::Sprite * ship;
    cocos2d::Sprite * heart;
    cocos2d::Sprite * laser;
    cocos2d::Sprite * enemy;
    cocos2d::Sprite * powerup;
    cocos2d::Sprite * background;

    //Лейбл
    cocos2d::Label * endGameLabel;

    //Векторы спрайтов
    std::vector<cocos2d::Sprite*> enemies;
    std::vector<cocos2d::Sprite*> lasers;
    std::vector<cocos2d::Sprite*> hearts;

    virtual bool init() override;

    CREATE_FUNC(HelloWorld);


private:


    //listener для кнопок
    void touchEventUp(Ref *sender, ui::Widget::TouchEventType type);
    void touchEventDown(Ref *sender, ui::Widget::TouchEventType type);
    void touchEventShoot(Ref *sender, ui::Widget::TouchEventType type);
    void touchRetry(Ref *sender, ui::Widget::TouchEventType type);

    //update-функции
    void update(float dt) override;
    void setGamePlaySpeed(float dt);
};

#endif // __HELLOWORLD_SCENE_H__
