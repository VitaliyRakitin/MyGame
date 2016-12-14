//
//  Field.cpp
//  MyGame
//
//  Created by Vitaliy Rakitin on 02.12.16.
//
//

#include "Game.h"


Cell::Cell(){
    up = true;
    down = true;
    left = true;
    right = true;
    dist = -1;
    prev = Vec2(-1,-1);
    next = Vec2(-1,-1);
}

/* построим матрицу в конструкторе */
Field::Field(){
    field_matrix.resize(CELL_AMOUNT);
    for (int k=0;k<CELL_AMOUNT;k++){
        field_matrix[k].resize(CELL_AMOUNT);
        for (int i=0;i<CELL_AMOUNT;i++){
            field_matrix[k][i].coordinates = Vec2(k,i);
        }
    }
    
    /* зададим стенку снизу */
    for (int k=0;k<CELL_AMOUNT;k++){
        field_matrix[k][0].down = false;
    }
    /* зададим стенку сверху */
    for (int k=0;k<CELL_AMOUNT;k++){
        field_matrix[k][CELL_AMOUNT-1].up = false;
     //   printf("%d %d %d\n",CELL_AMOUNT-1,k,field_matrix[CELL_AMOUNT-1][k].up);
    }
    /* зададим стенку слева */
    for (int k=0;k<CELL_AMOUNT;k++){
        field_matrix[0][k].left = false;
    }
    /* зададим стенку справа */
    for (int k=0;k<CELL_AMOUNT;k++){
        field_matrix[CELL_AMOUNT-1][k].right = false;
    }
    printf("%d %d %d %d ",field_matrix[8][8].down,field_matrix[8][8].up,field_matrix[8][8].left,field_matrix[8][8].right );
}

/* инициализация */
void Field::init(Game *game){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    /* find real position of the scene */
    Vec2 real_center = Vec2(visibleSize.width/2+origin.x, (visibleSize.height)/2+origin.y);
    
    /* размер клетки */
    cell_size = visibleSize.height/CELL_AMOUNT;
    
    /* позиция клетки (0,0) */
    first_position = Vec2(real_center.x - CELL_AMOUNT/2 * cell_size , real_center.y - CELL_AMOUNT/2 * cell_size);
    
    /* создадим само поле */
    desk = Sprite::create(FIELD_FILE_NAME);
    desk->setPosition(real_center);
    desk_size = desk->getContentSize();
    
    /* Подгоним поле под размер экрана */
    desk->setScale(visibleSize.height/desk_size.y);
    
    // add desk as a child to this layer
    game->addChild(desk, 0);

}

/* меняем возможность прохода при установке стенки */
void Field::change_passage_opportunity(float x, float y, bool orientation){
    if(orientation){
        /* горизонтально */
        field_matrix[int(x-0.5)][int(y-0.5)].up = false;
        field_matrix[int(x+0.5)][int(y-0.5)].up = false;
        field_matrix[int(x-0.5)][int(y+0.5)].down = false;
        field_matrix[int(x+0.5)][int(y+0.5)].down = false;
    }
    else{
        /* вертикально */
        field_matrix[int(x+0.5)][int(y-0.5)].left = false;
        field_matrix[int(x+0.5)][int(y+0.5)].left = false;
        field_matrix[int(x-0.5)][int(y-0.5)].right = false;
        field_matrix[int(x-0.5)][int(y+0.5)].right = false;
        
    }
}
