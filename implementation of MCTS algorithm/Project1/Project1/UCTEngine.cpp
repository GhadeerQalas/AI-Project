#include<stdio.h>
#include <iostream>
#include <cstdio>
#include <deque>
#include "Define.h"
#include "UCTEngine.h"

using namespace std;
deque<UCTNode *> nodelist;
deque<UCTNode *> blocklist;

//
UCTNode* newNode()
{
	UCTNode* p;
	if (nodelist.empty())
	{
		p = new UCTNode[1000];
		for (int i = 0; i < 1000; i++)
		{
			nodelist.push_back(p++);
		}
	}
	p = nodelist.front();
	nodelist.pop_front();
	return p;
}

//ɾ���ڵ�
void deleteNode(UCTNode* head1)
{
	UCTNode *p, *q = head1->son;
	while (q != NULL)
	{
		p = q->brother;
		deleteNode(q);
		q = p;
	}
	nodelist.push_back(head1);
}

//���Ƶ�ͼ
void MapCopy(int map[13][13],int copyMap[13][13]) {
	for (int i = 0; i < MAPLENGTH; i++)
	{
		for (int j = 0; j < MAPLENGTH; j++)
		{
			copyMap[i][j] = map[i][j];
		}
	}
}

//������Ϊ(useless)
void MoveCopy(MoveType currentMove) {
	currentMove.a = BestMove.a;
	currentMove.b = BestMove.b;
	currentMove.color = BestMove.color;
	currentMove.exchange = BestMove.exchange;
}

