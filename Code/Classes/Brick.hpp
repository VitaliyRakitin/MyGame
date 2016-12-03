//
//  Brick.hpp
//  MyGame
//  Описание стенки
//
//  Created by Vitaliy Rakitin on 02.12.16.
//
//

#ifndef Brick_hpp
#define Brick_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"

#define HORIZONTALLY_BRICK_ORIENTATION true
#define UPRIGHT_BRICK_ORIENTATION false
#define UPRIGHT 90
#define HORIZONTALLY 180

class Game;

using namespace cocos2d;


class Brick: public cocos2d:: Sprite{
private:
    Sprite* brick;
    bool orientation; // true --- horizontally, false --- upright
    Vec2 coordinates;
    Vec2 real_coordinates;
    Vec2 size;
public:
    bool is_touchble;
    float min_size;
    
    Brick(){};
    Brick(const Brick&){};
    ~Brick(){
        CC_SAFE_RELEASE_NULL(brick);};
    void init(std:: string file_img_name, Vec2 coord, bool orientation, Game* game);
    
    float getDistance(Point touch);
    bool checkDistance(Point touch);
    Point getPosition();
    void change_orientation();
    void move_to(Point position);
    void move_and_fix(Point position);
    void move_back();
    bool getOrientation();
};




#endif /* Brick_hpp */
