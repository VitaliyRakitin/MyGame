//
//  Game.hpp
//  MyGame
//  Игровая логика
//
//  Created by Vitaliy Rakitin on 30.11.16.
//
//

#ifndef Game_hpp
#define Game_hpp


#include <stdio.h>
#include <vector>
#include <math.h>
#include "cocos2d.h"
#include "Player.hpp"
#include "Brick.hpp"
#include "Field.hpp"


#define FIELD_FILE_NAME "desk.png"
#define PLAYER_1_IMG "player5.png"
#define PLAYER_2_IMG "player6.png"
#define BRICK_IMG "brick.png"
#define CELL_AMOUNT 9
#define BRICKS_AMOUNT 10
#define PLAYERS_AMOUNT 2
#define TYPE_BRICK 1
#define TYPE_PLAYER 2
#define TYPE_NONE 0


class Player;
class Brick;

using namespace cocos2d;

class Game : public cocos2d::Layer
{
private:
    Sprite* desk;
    std::vector<Player> players;
    std::vector<Brick> bricks;
    size_t real_bricks_amount;
    Vec2 is_moving; // 0,0 -- nothing, 1,N --- N brick, 2,K --- K player
    Label* MyLabelPlayer1;
    Label* MyLabelPlayer2;

    std::vector<Label*> PlayersCount;
    
    Point touchOffset;
    
    bool isGoodStepPlayer(int x, int y);
    bool isGoodStepBrick(float x,float y, bool orientation);
public:
    Field field;
    int playerNumber;
    
    Game(){};
    ~Game(){};
    virtual bool init();
    static cocos2d::Scene* createScene();
    
    void move_to_best_place_player(Touch* touch);
    void move_to_best_place_brick(Touch* touch);
    

    /// Returns the Cocos2d position of the touch
    Point touchToPoint(Touch* touch);
    
    /// Returns true if the touch is within the boundary of our sprite
    bool isTouchingBrick(Touch* touch,size_t brick_number);
    bool isTouchingPlayer(Touch* touch,size_t player_number);

    
    // we are multi-touch enabled, so we must use the ccTouches functions
    // vs the ccTouch functions
    void onTouchesBegan(const std::vector<Touch*>& touches, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event* event);
    
    // local functions
    Vec2 to_local_coordinates(cocos2d::Vec2 real_coordinates);
    Vec2 to_real_coordinates(cocos2d::Vec2 real_coordinates);
    
    void add_brick();
    
    
    void changePlayer();
    
    bool DeikstraForPlayerInit(size_t player_number, Vec2 next_pos);
    void DeikstraForPlayerStep(Vec2 next, Vec2 prev, int total_dist);
    
    CREATE_FUNC(Game);

};


#endif /* Game_hpp */
