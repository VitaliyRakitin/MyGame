//
//  Field.hpp
//  MyGame
//
//  Created by Vitaliy Rakitin on 02.12.16.
//
//

#ifndef Field_hpp
#define Field_hpp

#include <stdio.h>
#include <string>
#include "cocos2d.h"

using namespace cocos2d;

class Cell{
private:
public:
    Vec2 coordinates;
    bool left;
    bool right;
    bool up;
    bool down;
    int dist;//расстояние от текущего положения игрока, для алгоритма Дейкстры
    Vec2 prev;
    Vec2 next;
    Cell();
};

class Field{
    private:
        Sprite *desk;
public:
        Vec2 desk_size;
        float cell_size;
        Vec2 first_position;
        // столбцы -- y, строки -- x
        std::vector<std::vector<Cell>> field_matrix;
        Field();
        void init(Game *game);
        void change_passage_opportunity(float x, float y, bool orientation);// true -- horiz
};


#endif /* Field_hpp */
