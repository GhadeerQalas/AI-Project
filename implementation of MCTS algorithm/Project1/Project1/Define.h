#pragma once
#ifndef _DEFINE_H_
#define _DEFINE_H_

#define BORDER  -99
#define LINEONE 11     //横边
#define LINETWO 22     //竖边
#define	MAPLENGTH 13   //Map的长度


//格子
//        ( 2, 2)         ( 2, 4)         ( 2, 6)         ( 2, 8)         ( 2,10)
//        ( 4, 2)         ( 4, 4)         ( 4, 6)         ( 4, 8)         ( 4,10)
//        ( 6, 2)         ( 6, 4)         ( 6, 6)         ( 6, 8)         ( 6,10)
//        ( 8, 2)         ( 8, 4)         ( 8, 6)         ( 8, 8)         ( 8,10)
//        (10, 2)         (10, 4)         (10, 6)         (10, 8)         (10,10)

//边
//        ( 1, 2)         ( 1, 4)         ( 1, 6)         ( 1, 8)         ( 1,10)
//( 2, 1)         ( 2, 3)         ( 2, 5)         ( 2, 7)         ( 2, 9)         ( 2,11)
//        ( 3, 2)         ( 3, 4)         ( 3, 6)         ( 3, 8)         ( 3,10)
//( 4, 1)         ( 4, 3)         ( 4, 5)         ( 4, 7)         ( 4, 9)         ( 4,11)
//        ( 5, 2)         ( 5, 4)         ( 5, 6)         ( 5, 8)         ( 5,10)
//( 6, 1)         ( 6, 3)         ( 6, 5)         ( 6, 7)         ( 6, 9)         ( 6,11)
//        ( 7, 2)         ( 7, 4)         ( 7, 6)         ( 7, 8)         ( 7,10)
//( 8, 1)         ( 8, 3)         ( 8, 5)         ( 8, 7)         ( 8, 9)         ( 8,11)
//        ( 9, 2)         ( 9, 4)         ( 9, 6)         ( 9, 8)         ( 9,10)
//(10, 1)         (10, 3)         (10, 5)         (10, 7)         (10, 9)         (10,11)
//        (11, 2)         (11, 4)         (11, 6)         (11, 8)         (11,10)

extern int Map[13][13];
extern int State[13][13];
extern int Score[2];     //0为先手方得分，1为后手方得分
extern int ValExch;      //换手数目
extern int LastColor;    //上一步颜色
extern int OurColor;
extern int TrapStack[20][3];
extern int Step;
extern int HopeVal;

extern int TemMap[13][13];
extern int TemState[13][13];
extern int dx[4];
extern int dy[4];

struct MoveType
{
	int a, b;
	int color;
	int exchange;
};

extern MoveType BestMove;

//new add
struct UCTNode {
	int win;
	int visit;
	bool getScore;
	int frontScore;
	int backScore;

	MoveType move;
	UCTNode *son;
	UCTNode *father;
	UCTNode *brother;

	int currentMap[13][13];
};

extern UCTNode mainNode;

#endif