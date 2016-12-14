//
//  Player.hpp
//  MyGame
//  Описание игрока
//
//  Created by Vitaliy Rakitin on 30.11.16.
//
//

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"

class Game;

using namespace cocos2d;


class Player: public cocos2d:: Sprite{
private:
    Sprite* player;
   // Vec2 coordinates;
    Vec2 real_coordinates;
    
public:
    int wallPlaced;
    bool is_touchble;
    
    Player(){};
    Player(const Player&){};
    ~Player(){
        CC_SAFE_RELEASE_NULL(player);};
    void init(std:: string file_img_name, Vec2 coord,bool touchble,Game* game);

    float getDistance(Point touch);
    Point getPosition();
    
    void move_to(Point position);
    void move_and_fix(Point position);
    void move_back();
};




#endif /* Player_hpp */
