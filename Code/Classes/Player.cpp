//
//  Player.cpp
//  MyGame
//
//  Created by Vitaliy Rakitin on 30.11.16.
//
//

//#include "Player.hpp"
#include "Game.h"

/* инициализация */
void Player::init(std:: string file_img_name, Vec2 coord, Game* game){
    Vec2 size;
    is_touchble = true;
    player = Sprite::create(file_img_name);
    size = player->getContentSize();
    coordinates = coord;
    player->setScale(game->cell_size/size.y);
    real_coordinates = game->to_real_coordinates(coordinates);
    player->setPosition(real_coordinates);
    game->addChild(player,2);
}

/* возвращает глобальные координаты объекта в данный момент */
Point Player::getPosition(){
    return real_coordinates;
}

/* возвращает расстояние от объекта в данной точки */
float Player::getDistance(Point touch){
    return real_coordinates.distance(touch);
}

/* переместить объект в данную точку, но не фискирует */
void Player::move_to(Point position){
    player->setPosition(position);
}

/* переместить объект в данную точку и фискирует */
void Player::move_and_fix(Point position){
    player->setPosition(position);
    real_coordinates = position;
}

/* переместить объект на первоночальное место */
void Player::move_back(){
    player->setPosition(real_coordinates);
}
