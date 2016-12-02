//
//  Game.cpp
//  MyGame
//
//  Created by Vitaliy Rakitin on 30.11.16.
//
//

#include "Game.h"
USING_NS_CC;

/* создаём сцену */
Scene* Game::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = Game::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    return scene;
}

/* инициализация */
bool Game::init(){
    /* 1. super init first */
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /* find real position of the scene */
    Vec2 real_center = Vec2(visibleSize.width/2+origin.x, (visibleSize.height)/2+origin.y);
    
    /* размер клетки */
    cell_size = visibleSize.height/CELL_AMOUNT;
    
    /* позиция клетки (0,0) */
    first_position = Vec2(real_center.x - CELL_AMOUNT/2 * cell_size , real_center.y - CELL_AMOUNT/2 * cell_size);
    
    /* создадим поле */
    desk = Sprite::create(FIELD_FILE_NAME);
    desk->setPosition(real_center);
    desk_size = desk->getContentSize();
    
    /* Подгоним поле под размер экрана */
    desk->setScale(visibleSize.height/desk_size.y);
    
    // add desk as a child to this layer
    this->addChild(desk, 0);
    
    /* в начале ничего не двигаем */
    is_moving = Vec2(TYPE_NONE,TYPE_NONE);
    
    /* добавим игроков */
    players.resize(PLAYERS_AMOUNT);
    players[0].init(PLAYER_1_IMG, Vec2(4,0),this);
    players[1].init(PLAYER_2_IMG, Vec2(4,8),this);
    
    /* добавим стенку слева вне поля */
    real_bricks_amount = 0;
    bricks.resize(BRICKS_AMOUNT);
    bricks[real_bricks_amount].init(BRICK_IMG, Vec2(-2, 4.5), UPRIGHT_BRICK_ORIENTATION, this);
    real_bricks_amount++;
    
    /* для считывания нажатий */
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(Game::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(Game::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(Game::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(Game::onTouchesEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

/* перевод из глобальных координат в координаты поля */
Vec2 Game::to_local_coordinates(cocos2d::Vec2 real_coordinates){
    return Vec2((real_coordinates.x - first_position.x)/cell_size,
                (real_coordinates.y - first_position.y)/cell_size);
}

/* перевод из координат поля в глобальные */
Vec2 Game::to_real_coordinates(cocos2d::Vec2 local_coordinates){
    return Vec2(first_position.x + local_coordinates.x * cell_size,
                first_position.y + local_coordinates.y * cell_size);
}

/* преобразуем touch object в точку в глобальных координатах */
Point Game::touchToPoint(Touch* touch)
{
    return Director::getInstance()->convertToGL(touch->getLocationInView());
}

/* проверка на то, что нажимаем на стенку */
bool Game::isTouchingBrick(Touch* touch, size_t brick_number)
{
    return (bricks[brick_number].checkDistance(touchToPoint(touch)));
}

/* проверка на то, что нажимаем на игрока */
bool Game::isTouchingPlayer(Touch* touch, size_t player_number)
{
    return (players[player_number].getDistance(touchToPoint(touch)) < (cell_size/2));
}

/* находим подходящую позицию на поле для игрока */
void Game::move_to_best_place_player(Touch* touch){
    float x,y;
    Vec2 position = touchToPoint(touch) + touchOffset;
    x = (position.x - first_position.x)/cell_size;
    y = (position.y - first_position.y)/cell_size;
    if ((x > CELL_AMOUNT)||(y>CELL_AMOUNT)||(x<0)||(y<0)){
        players[is_moving.y].move_back();
    }
    else{
        players[is_moving.y].move_and_fix(to_real_coordinates(Vec2(int(x+0.5),int(y+0.5))));
    }
}

/* находим подходящую позицию на поле для стенки */
void Game::move_to_best_place_brick(Touch* touch){
    float x,y;
    Vec2 position = touchToPoint(touch) + touchOffset;
    x = (position.x - first_position.x - cell_size/2)/cell_size;
    y = (position.y - first_position.y - cell_size/2)/cell_size;
    if ((x > CELL_AMOUNT)||(y>CELL_AMOUNT)||(x<0)||(y<0)){
        bricks[is_moving.y].move_back();
    }
    else{
        bricks[is_moving.y].move_and_fix(to_real_coordinates(Vec2(int(x+0.5)+0.5,int(y+0.5)+0.5)));
    }
}



/* Когда дотронулись до экрана */
void Game::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    /* смещение прикосновения */
    touchOffset = Point::ZERO;
    
    for( auto touch : touches )
    {
        /* определим, что же трогаем */
        if (touch){
            for (int i=0;i<real_bricks_amount;i++){
                if(bricks[i].is_touchble && isTouchingBrick(touch,i)){
                    /* двигаем - i-ю стенку, включим её в подвижность */
                    is_moving = Vec2(TYPE_BRICK,i);
                    touchOffset = bricks[i].getPosition()- touchToPoint(touch);
                }
            }
            for (int i=0;i<PLAYERS_AMOUNT;i++){
                if (players[i].is_touchble && isTouchingPlayer(touch,i)){
                    /* двигаем - i-го игрока, включим её в подвижность */
                    is_moving = Vec2(TYPE_PLAYER,i);
                    touchOffset = players[i].getPosition() - touchToPoint(touch);
                }
            }
        }
    }
}

/* тянем объект */
void Game::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    if (is_moving.x != TYPE_NONE){
        for( auto touch : touches )
        {
            if( touch && touchOffset.x && touchOffset.y ){
                if(is_moving.x == TYPE_BRICK){
                    bricks[is_moving.y].move_to(touchToPoint(touch) + touchOffset);
                }
                if(is_moving.x == TYPE_PLAYER){
                    players[is_moving.y].move_to(touchToPoint(touch) + touchOffset);
                }
            }
        }
    }
}

/* отпустили палец от экрана */
void Game::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    for( auto touch : touches )
    {
        if( touch && touchOffset.x && touchOffset.y ){
            if(is_moving.x == TYPE_BRICK){
                /* если мы не перетащили палочку далеко, значит хотели сменить ориентацию */
                if (isTouchingBrick(touch,is_moving.y)){
                    bricks[is_moving.y].change_orientation();
                    bricks[is_moving.y].move_back();
                }
                else{
                    move_to_best_place_brick(touch);
                }
            }
            if(is_moving.x == TYPE_PLAYER){
                move_to_best_place_player(touch);
            }
        }
    }
    /* заончили перетаскивать */
    is_moving = Vec2(TYPE_NONE,TYPE_NONE);
}
