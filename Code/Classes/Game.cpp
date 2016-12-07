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
    /* создадим поле */
    field.init(this);
    
    /* в начале ничего не двигаем */
    is_moving = Vec2(TYPE_NONE,TYPE_NONE);
    
    /* добавим игроков */
    players.resize(PLAYERS_AMOUNT);
    players[0].init(PLAYER_1_IMG, Vec2(4,0),this);
    players[1].init(PLAYER_2_IMG, Vec2(4,8),this);
    
    /* добавим стенку слева вне поля */
    real_bricks_amount = 0;
    bricks.resize(BRICKS_AMOUNT);
    add_brick();
    
    /* для считывания нажатий */
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(Game::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(Game::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(Game::onTouchesEnded, this);
    listener->onTouchesCancelled = CC_CALLBACK_2(Game::onTouchesEnded, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

/* добавим стенку слева вне поля */
void Game::add_brick(){
    if (real_bricks_amount< BRICKS_AMOUNT){
        bricks[real_bricks_amount].init(BRICK_IMG, Vec2(-2, 4.5), UPRIGHT_BRICK_ORIENTATION, this);
        real_bricks_amount++;
    }
}

/* перевод из глобальных координат в координаты поля */
Vec2 Game::to_local_coordinates(cocos2d::Vec2 real_coordinates){
    return Vec2((real_coordinates.x - field.first_position.x)/field.cell_size,
                (real_coordinates.y - field.first_position.y)/field.cell_size);
}

/* перевод из координат поля в глобальные */
Vec2 Game::to_real_coordinates(cocos2d::Vec2 local_coordinates){
    return Vec2(field.first_position.x + local_coordinates.x * field.cell_size,
                field.first_position.y + local_coordinates.y * field.cell_size);
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
    return (players[player_number].getDistance(touchToPoint(touch)) < (field.cell_size/2));
}

/* проверяем, что ход игрока возможен */
bool Game::isGoodStepPlayer(int x,int y){
    
    /* проверим, что мы сделали шаг только на 1 клетку вдоль вертикали или горизонтали*/
    Vec2 start_coord = players[is_moving.y].getPosition();
    Vec2 finish_coord = to_real_coordinates(Vec2(x,y));
    Vec2 dist =finish_coord -start_coord;
    
    /* двигаемся вертикально */
    if ((dist.x<1)&&(dist.x>-1)){
        if ((dist.y > 1)&&(dist.y < field.cell_size + 1)){
            /* пришли снизу сделав шаг в 1 клетку */
            //log("down %d, %d",x,y);
            return field.field_matrix[x][y].down;
            
        }
        if ((dist.y < -1)&&(-dist.y < field.cell_size + 1)){
            /* пришли сверху сделав шаг в 1 клетку */
            //log("up %d, %d",x,y);
            return field.field_matrix[x][y].up;
        }
    }
    if ((dist.y<1)&&(dist.y > - 1)){
        if ((dist.x > 1)&&(dist.x < field.cell_size + 1)){
            /* пришли слева сделав шаг в 1 клетку */
            //log("right %d, %d",x,y);
            return field.field_matrix[x][y].left;
        }
        if ((dist.x < -1)&&(-dist.x < field.cell_size + 1)){
            /* пришли справа сделав шаг в 1 клетку */
            //log("left %d, %d",x,y);
            return field.field_matrix[x][y].right;
        }
    }
    return false;
}

/* находим подходящую позицию на поле для игрока */
void Game::move_to_best_place_player(Touch* touch){
    float x,y;

    
    Vec2 position = touchToPoint(touch) + touchOffset;
    x = (position.x - field.first_position.x)/field.cell_size + 0.5;
    y = (position.y - field.first_position.y)/field.cell_size + 0.5;
    
    if ((x < CELL_AMOUNT)&&(y < CELL_AMOUNT)&&(x > 0)&&(y > 0)&&(isGoodStepPlayer(x,y))){
        players[is_moving.y].move_and_fix(to_real_coordinates(Vec2(int(x),int(y))));
    }
    else{
        players[is_moving.y].move_back();
    }
}

/* находим подходящую позицию на поле для стенки */
void Game::move_to_best_place_brick(Touch* touch){
    float x,y;
    Vec2 position = touchToPoint(touch) + touchOffset;
    x = int((position.x - field.first_position.x - field.cell_size/2)/field.cell_size + 0.5) + 0.5;
    y = int((position.y - field.first_position.y - field.cell_size/2)/field.cell_size + 0.5) + 0.5;
    
    //log("x = %lf, y = %lf",x,y);
    if ((x > CELL_AMOUNT-1)||(y>CELL_AMOUNT-1)||(x<0)||(y<0)||!isGoodStepBrick(x,y,bricks[is_moving.y].getOrientation())){
        bricks[is_moving.y].move_back();
    }
    else{
        bricks[is_moving.y].move_and_fix(to_real_coordinates(Vec2(x,y)));
        add_brick();
        field.change_passage_opportunity(x,y,bricks[is_moving.y].getOrientation());
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
    
 /*   log("real_bricks_amount = %zu", real_bricks_amount);
    for (int i=0;i<real_bricks_amount;i++){
        Vec2 coord = to_local_coordinates(bricks[i].getPosition());
        log("%d: x = %lf, y = %lf",i,coord.x, coord.y);
    }
   */ 
    /* заончили перетаскивать */
    is_moving = Vec2(TYPE_NONE,TYPE_NONE);
}

/*
bool Game::isGoodStepBrick(float x,float y){
    Vec2 position = Vec2(x,y);
    log("orientation: %d", bricks[real_bricks_amount].getOrientation());
    float dist;
        for (int i=0; i<real_bricks_amount; i++){
        dist = bricks[i].getDistance(to_real_coordinates(position));
        if (dist < 1 && dist > -1){
            return false;
        }
    }
    return true;
}
*/
bool Game::isGoodStepBrick(float x,float y, bool orientation){
    Vec2 position = Vec2(x,y);
    float dist;
    for (int i=0; i<real_bricks_amount; i++){
        dist = bricks[i].getDistance(to_real_coordinates(position));
        if (dist < 1 && dist > -1){
            return false;
        }
        else{
            if ((orientation == 0) && (bricks[i].getOrientation() == 0) && (std::abs(position.y - to_local_coordinates(bricks[i].getPosition()).y) < 1.5) && (std::abs(position.x - to_local_coordinates(bricks[i].getPosition()).x) < 0.5)){
                log("!!!!!!!!!!!!");
                return false;
            }
            if ((orientation == 1) && (bricks[i].getOrientation() == 1) && (std::abs(position.y - to_local_coordinates(bricks[i].getPosition()).y) < 0.5) && (std::abs(position.x - to_local_coordinates(bricks[i].getPosition()).x) < 1.5)){
                return false;
            }
        }
    }
    return true;
}
