#pragma once
#ifndef _UCTENGINE_H_H_
#define _UCTENGINE_H_H_

UCTNode* newNode();
void deleteNode(UCTNode* head1);
void MoveCopy(MoveType currentMove);
void MapCopy(int map[13][13],int copyMap[13][13]);
#endif