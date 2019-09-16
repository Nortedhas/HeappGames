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
#include "ui/CocosGUI.h"
#include <time.h>
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene=Scene::create();
    auto layer=HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init() {

    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


    /////////////////////////////

    //спрайт игрока
    ship = Sprite::create("player.png");
    ship->setScale(5);
    ship->setPosition(30, 100);

    //жизни
    for(int i=0;i<lifes;i++){
        heart=Sprite::create("heart.png");
        heart->setPosition(Vec2(origin.x + 30 + i * 20,visibleSize.height-10));
        heart->setScale(7);
        this->addChild(heart,2);
        hearts.push_back(heart);
    }


    //задний фон
    background=Sprite::create("background.png");
    background->setAnchorPoint(Point(0.5,0.5));
    background->setScale(8);
    background->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2);

    //кнопка вверх
    ui::Button *upButton=ui::Button::create("button_up.png","button_up.png");
    upButton->setPosition(Vec2(30,80));
    upButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEventUp,this));

    //кнопка вниз
    ui::Button *downButton=ui::Button::create("button_down.png","button_down.png");
    downButton->setPosition(Vec2(30,40));
    downButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEventDown,this));

    //кнопка выстрела
    ui::Button *shootButton=ui::Button::create("button_shoot.png","button_shoot.png");
    shootButton->setPosition(Vec2(visibleSize.width-50,50));
    shootButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchEventShoot,this));

    //добавляем элементы на сцену
    this->addChild(upButton,2);
    this->addChild(downButton,2);
    this->addChild(shootButton,2);
    this->addChild(background,0);
    this->addChild(ship,1);

    this->scheduleUpdate();

    this->schedule(schedule_selector(HelloWorld::setGamePlaySpeed),1);

    return true;
}



//метод для создания спрайтов врагов и их движения
void HelloWorld::setGamePlaySpeed(float dt) {

    //каждую секунду, за пределами экрана создаётся два ряда врагов
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 6; j++) {
            enemy = Sprite::create("enemy.png");
            enemy->setPosition(Vec2(
                    Director::getInstance()->getVisibleSize().width + 50 + i * 100,
                    Director::getInstance()->getVisibleSize().height * 0.2  + j * 30));
            enemy->setScale(5);
            this->addChild(enemy, 1);
            enemies.push_back(enemy);
        }
    }

    //движение каждого спрайта из двух разных рядом по кривой Безье
    if(enemiesNumber<enemies.size()/2 + 1  && enemiesNumberReverse>enemies.size()/2 - 1){
        Point startPosition;
        startPosition.x = enemies[enemiesNumber]->getPositionX();
        startPosition.y = enemies[enemiesNumber]->getPositionY();

        //кривая Безье для первого ряда
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Point(Director::getInstance()->getVisibleSize().width/5, Director::getInstance()->getVisibleSize().height - 50);
        bezier.controlPoint_2 = Point(Director::getInstance()->getVisibleSize().width/10, Director::getInstance()->getVisibleSize().height);
        bezier.endPosition = startPosition;

        Point startPositionReverse;
        startPositionReverse.x = enemies[enemiesNumberReverse]->getPositionX();
        startPositionReverse.y = enemies[enemiesNumberReverse]->getPositionY();

        //кривая Безье для второго ряда
        ccBezierConfig bezierReverse;
        bezierReverse.controlPoint_1 = Point(Director::getInstance()->getVisibleSize().width/10, Director::getInstance()->getVisibleSize().height / 10);
        bezierReverse.controlPoint_2 = Point(Director::getInstance()->getVisibleSize().width/5, Director::getInstance()->getVisibleSize().height);
        bezierReverse.endPosition = startPositionReverse;

        auto actionReverse = BezierTo::create(4, bezierReverse);

        auto action = BezierTo::create(4, bezier);

        //переменная random для случайного значения Y
        int random=cocos2d::RandomHelper::random_int(20,height);
        //переменная time для случайной длительности action
        int time=cocos2d::RandomHelper::random_int(3,5);

        auto move=MoveTo::create(time,Vec2(Director::getInstance()->getVisibleSize().width/100,random));

        random=cocos2d::RandomHelper::random_int(1,height);
        time=cocos2d::RandomHelper::random_int(3,5);

        auto moveReverse=MoveTo::create(time,Vec2(Director::getInstance()->getVisibleSize().width/100,random));

        auto delay=DelayTime::create(1);

        //послдеовательность action
        auto seq=Sequence::create(move,action,delay,NULL);
        auto seqReverse=Sequence::create(moveReverse,actionReverse,delay,NULL);

        //запускаем последовательности для каждого ряда спрайтов
        enemies[enemiesNumber]->runAction(seq);
        enemies[enemiesNumberReverse]->runAction(seqReverse);

    }else{
        //заново инициализируем переменные для бесконечного повтора
        enemiesNumber=0;
        enemiesNumberReverse=enemies.size();
    }
    //инкремент и дикремент
    enemiesNumber++;
    enemiesNumberReverse--;

}

//update-функция
void HelloWorld::update(float dt) {

    //разрешение коллизий между выстрелом и противником
    for(int i=0;i<lasers.size();i++){
        for(int j=0;j<enemies.size();j++){
            if (enemies[j]->getBoundingBox().containsPoint(lasers[i]->getPosition())) {

                enemies[j]->setVisible(false);
                enemies.erase(enemies.begin()+j);

                lasers[i]->setVisible(false);
                lasers.erase(lasers.begin()+i);
                kills++;

                //если лазер не попал по врагу
            }else if(lasers[i]->getPositionX()==Director::getInstance()->getVisibleSize().width){

                lasers[i]->setVisible(false);
                // lasers.erase(lasers.begin()+i);
                //Функции this->removeChild(lasers[i]) , lasers[i]->removeFromParent() , this->removeByTag() выкидывают из игры.
                //Конечно, происходит утечка памяти и со временем, перебор большого вектора лазеров каждый фрейм, становится всё более трудоёмкой задачей и снижает FPS.
            }
        }
    }

    //разрешение коллизий между врагом и игроком
    for(int i=0;i<enemies.size();i++){
        if(enemies[i]->getBoundingBox().containsPoint(ship->getPosition())){

            lifes--;
            hearts[lifes]->setVisible(false);
            enemies[i]->setVisible(false);
            enemies.erase(enemies.begin()+i);

            //в случае смерти выводим лейбл и кнопку для рестарта сцены
            if(lifes==0){

                ship->setPosition(0,Director::getInstance()->getVisibleSize().height+400);
                endGameLabel=Label::createWithSystemFont("You died","arial",20);
                endGameLabel->setPosition(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2+40);

                ui::Button *retryButton=ui::Button::create("retry.png","retry.png");
                retryButton->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/3+20));
                retryButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::touchRetry,this));

                this->addChild(endGameLabel,2);
                this->addChild(retryButton,2);

            }
            break;
        }
    }

    //при убийстве 5 врагов создаём power up
    if(kills==5){

        powerup=Sprite::create("power_up.png");
        powerup->setScale(9);

        //переменная для рандомного значения Y
        int random=cocos2d::RandomHelper::random_int(30,height);
        powerup->setPosition(Director::getInstance()->getVisibleSize().width,random);

        auto movePowerUp=MoveTo::create(5,Vec2(0,random));
        this->addChild(powerup,2);

        powerup->runAction(movePowerUp);
        //переменная для проверки power up на нахождение в игре
        isCreatePowerUp=true;

        kills++;
    }

    if(isCreatePowerUp) {
        //разрешение коллизий между power up и игроком
        if (powerup->getBoundingBox().intersectsRect(ship->getBoundingBox())) {
            isPowerUp = true;
            powerup->setVisible(false);
            powerup->setPosition(0,0);

            //если игрок не поймал power up
        } else if (powerup->getPositionX() == 0) {

             powerup->setVisible(false);
             powerup->setPosition(0,0);
        }
    }
}

//listener для кнопки вверх
void HelloWorld::touchEventUp(Ref *sender, ui::Widget::TouchEventType type) {

    if(type==ui::Widget::TouchEventType::BEGAN){
        //проверка на нахождение игрока в пределах экрана
        if(ship->getPositionY()<Director::getInstance()->getVisibleSize().height-20) {

            Point startShipPosition = Point();
            startShipPosition.y = ship->getPositionY();
            ship->setPosition(30, startShipPosition.y + 20);

        }
    }
}

//listener для кнопки вниз
void HelloWorld::touchEventDown(Ref *sender, ui::Widget::TouchEventType type) {

    if(type==ui::Widget::TouchEventType::BEGAN){
        //проверка на нахождение игрока в пределах экрана
        if(ship->getPositionY()>60) {

            Point startShipPosition = Point();
            startShipPosition.y = ship->getPositionY();
            ship->setPosition(30, startShipPosition.y - 20);

        }
    }
}
//listener для кнопки выстрела
void HelloWorld::touchEventShoot(Ref *sender, ui::Widget::TouchEventType type) {

    if(type==ui::Widget::TouchEventType::BEGAN){

        //создаём спрайт лазера
        laser=Sprite::create("laser.png");
        laser->setScale(1.5);

        //позиция игрока по оси Y в момент нажатия
        Point startShipPosition=Point();
        startShipPosition.y=ship->getPositionY();
        laser->setPosition(30, startShipPosition.y);

        this->addChild(laser);
        lasers.push_back(laser);

        //если игрок поймап power up
        if(isPowerUp){

            //создаём кривую Безье для траектории выстрела
            ccBezierConfig bezierConfig;

            if(shoot) {
                bezierConfig.controlPoint_1 = Point(
                        Director::getInstance()->getVisibleSize().width / 2,
                        startShipPosition.y + 150);
                bezierConfig.controlPoint_2 = Point(Director::getInstance()->getVisibleSize().width,
                                                    startShipPosition.y - 150);
                bezierConfig.endPosition = Point(Director::getInstance()->getVisibleSize().width,
                                                 startShipPosition.y - 10 );

                auto bezier = BezierTo::create(3, bezierConfig);

                laser->runAction(bezier);

                shoot=false;
            }else if(!shoot){
                bezierConfig.controlPoint_1 = Point(
                        Director::getInstance()->getVisibleSize().width / 2,
                        startShipPosition.y - 150);

                bezierConfig.controlPoint_2 = Point(
                        Director::getInstance()->getVisibleSize().width,
                        startShipPosition.y + 150);

                bezierConfig.endPosition = Point(Director::getInstance()->getVisibleSize().width,
                                                 startShipPosition.y + 10);

                auto bezier = BezierTo::create(3, bezierConfig);

                laser->runAction(bezier);
                shoot=true;
            }
                                                                                                                                                                                                                                                                                +
        }else {
            auto action = MoveTo::create(2, Vec2(Director::getInstance()->getVisibleSize().width,startShipPosition.y));
            laser->runAction(action);

        }
    }
}


//listener для кнопки рестарта
void HelloWorld::touchRetry(Ref *sender, ui::Widget::TouchEventType type) {

    //заново запускаем сцену с эффектом
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, HelloWorld::create(), Color3B(0,255,255)));

}



