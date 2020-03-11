#pragma once
#ifndef _MOVES_H_
#define _MOVES_H_

#include"define.h"

int StaticMakeMove(MoveType &nowmove, int lastcolor);
int MakeMove(MoveType &nowmove, int lastcolor);
void UnMakeMove(MoveType &nowmove);

#endif