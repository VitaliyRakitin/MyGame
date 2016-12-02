//
//  Brick.cpp
//  MyGame
//
//  Created by Vitaliy Rakitin on 02.12.16.
//
//

#include "Game.h"

/* инициализация */
void Brick::init(std:: string file_img_name, Vec2 coord, bool in_orientation, Game* game){
    is_touchble = true;
    
    brick = Sprite::create(file_img_name);
    size = brick->getContentSize();
    coordinates = coord;
    real_coordinates = game->to_real_coordinates(coordinates);
    brick->setScale(game->cell_size*2/size.x);
    brick->setPosition(real_coordinates);
    
    orientation = in_orientation;
    size = brick->getContentSize();
    min_size = std::min(size.x,size.y);
    
    /* по дефолту ориентация картинки -- горизонтальна */
    if (orientation == UPRIGHT_BRICK_ORIENTATION){
        brick->setRotation(UPRIGHT);
        size.y = size.x;
        size.x = min_size;
    }
    
    game->addChild(brick, 2);
}

/* возвращает глобальные координаты объекта в данный момент */
Point Brick::getPosition(){
    return real_coordinates;
}

/* возвращает расстояние точки фиксации объекта до данной */
float Brick::getDistance(Point touch){
    return real_coordinates.distance(touch);
}

/* Проверяет, что точка лежит рядом с объектом */
bool Brick::checkDistance(Point touch){
    return (std::abs(real_coordinates.x - touch.x) < size.x + min_size*2 &&
            std::abs(real_coordinates.y - touch.y) < size.y + min_size*2);
}

/* сменить ориентацию */
void Brick::change_orientation(){
    float cur = size.x;
    size.x = size.y;
    size.y = cur;
    
    if (orientation == HORIZONTALLY_BRICK_ORIENTATION){
        orientation = UPRIGHT_BRICK_ORIENTATION;
        brick->setRotation(UPRIGHT);
    }
    else{
        orientation = HORIZONTALLY_BRICK_ORIENTATION;
        brick->setRotation(HORIZONTALLY);
    }
}

/* переместить объект в данную точку, но не фискирует */
void Brick::move_to(Point position){
    brick->setPosition(position);
}

/* переместить объект в данную точку и фискирует */
void Brick::move_and_fix(Point position){
    brick->setPosition(position);
    real_coordinates = position;
}

/* переместить объект на первоночальное место */
void Brick::move_back(){
    brick->setPosition(real_coordinates);
}
