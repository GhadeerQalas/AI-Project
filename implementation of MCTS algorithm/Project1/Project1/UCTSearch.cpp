#include <iostream>
#include <time.h>
#include<stdlib.h>
#include<stdio.h>
#include <math.h>
#include "Define.h"
#include "UCTEngine.h"
#include "UCTSearch.h"
#define MINSIM 5
#define MAXSIM 10
#define INF 9999
#define UCB_K 1
UCTNode *root;


int UCTTempMap[13][13];
void UCTSearch() {
	//初始化根结点
	root = (UCTNode *)malloc(sizeof(UCTNode));
	root->father = NULL;
	root->son = NULL;
	root->brother = NULL;
	root->visit = 0;
	root->win = 0;
	root->getScore = false;
	root->frontScore = Score[0];
	root->backScore = Score[1];
	root->move.a = BestMove.a;
	root->move.b = BestMove.b;
	root->move.color = 1-BestMove.color;//根节点是对手下的棋
	root->move.exchange = BestMove.exchange;
	MapCopy(Map,root->currentMap);
	
	//控制剪枝的UCTTempMap
	MapCopy(Map, UCTTempMap);
	TempMapSet();
	
	//开始计时
	time_t start, end;
	start = time(NULL);
	end = time(NULL);

	UCTExpand(root);//扩展下一层节点
	int cnt = 0;
	UCTNode *p;
	bool AIwin = false;
	while (end-start<5/*cnt<200000*/)
	{
		p = UCTChooseSimNode(root->son);//找到需要进行模拟的节点
		AIwin = PlaySimulation(p);//进行模拟
		UpdateValue(AIwin,p);//逐层返回更新胜率
		//更新计时器
		end = time(NULL);
		cnt++;
	}
	p = root->son;
	double maxRate = 0,rate=-INF;
	UCTNode *q=p;
	//找到UCB最大的点修改BestMove
	while (p!=NULL)
	{
		rate = (double)(p->win) / (double)(p->visit);
		//printf("move %c%c rate: %3f visit:%d win:%d\n", p->move.a+'A'-1,p->move.b+'A'-1,rate,p->visit,p->win);
		if (rate>maxRate)
		{
			maxRate = rate;
			q = p;
		}
		p = p->brother;
	}
	BestMove.a = q->move.a;
	BestMove.b = q->move.b;
	BestMove.color = q->move.color;

	deleteNode(root);
}

bool UCTExpand(UCTNode *root) {
	UCTNode *tempNode = root;
	UCTNode *node;
	int cnt = 0;
	bool flagHaveDelete = false;
	//搜索可以下的边
	for (int i = 1; i <12 ; i++)
	{
		for (int j = 1; j < 12; j++)
		{
			//是可以下的点
			if (((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) && root->currentMap[i][j] == 0) {
				//是第一次扩展,但不合适则继续找
				if (root->father==NULL&&suitableStep(root,i,j)==false)
				{
					flagHaveDelete = true;
					continue;		
				}
				//创建一个子节点
				node = newNode();
				//tempNode = node;
				node->brother = NULL;
				node->father = root;
				node->son = NULL;
				node->visit = 0;
				node->win = 0;
				node->frontScore = root->frontScore;
				node->backScore = root->backScore;
				MapCopy(root->currentMap, node->currentMap);//复制数组到新的节点中
				node->currentMap[i][j] = 1;
				node->move.a = i;
				node->move.b = j;
				//假如前一步没有得分
				if (!root->getScore)
				{//改变状态
					node->move.color = 1 - root->move.color;
				}
				node->getScore = JudgeGetScore(root, i, j, node);
				if (cnt==0) {
					root->son = node;
					cnt++;
				}
				else {
					tempNode->brother = node;
				}
				tempNode = node;
				UCTNode *p;
				for (int k = 0; k < MINSIM; k++)
				{
					p = node;
					bool win = false;
					win=PlaySimulation(node);
					UpdateValue(win, p);
				}
				
			}
			
		}
	}

	//由于被删导致没有可以下的边，在不删的情况下重新创建
	if (cnt==0&&flagHaveDelete)
	{
		for (int i = 1; i <12; i++)
		{
			for (int j = 1; j < 12; j++)
			{
				//是可以下的点
				if (((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) && root->currentMap[i][j] == 0) {
					
					//创建一个子节点
					node = newNode();
					//tempNode = node;
					node->brother = NULL;
					node->father = root;
					node->son = NULL;
					node->visit = 0;
					node->win = 0;
					node->frontScore = root->frontScore;
					node->backScore = root->backScore;
					MapCopy(root->currentMap, node->currentMap);//复制数组到新的节点中
					node->currentMap[i][j] = 1;
					node->move.a = i;
					node->move.b = j;
					//假如前一步没有得分
					if (!root->getScore)
					{//改变状态
						node->move.color = 1 - root->move.color;
					}
					node->getScore = JudgeGetScore(root, i, j, node);
					if (cnt == 0) {
						root->son = node;
						cnt++;
					}
					else {
						tempNode->brother = node;
					}
					tempNode = node;
					UCTNode *p;
					for (int k = 0; k < MINSIM; k++)
					{
						p = node;
						bool win = false;
						win = PlaySimulation(node);
						UpdateValue(win, p);
					}

				}

			}
		}
	}

	//没有可以下的边Exband失败
	if (cnt == 0) {
		return false;
	}
	return true;
}

UCTNode * UCTChooseSimNode(UCTNode *head) {
	UCTNode *chosenNode;
	double rate, maxUCB, UCBvalue;
	UCTNode *q;
	q = head;
	chosenNode = q;
	maxUCB = 0;
	while (q!=NULL)
	{
		rate = (double)(q->win) / (double)(q->visit);
		UCBvalue = sqrt(2 * UCB_K*log((double)(q->father->visit)) / (double)(q->visit));
		if (rate + UCBvalue > maxUCB)
		{
			maxUCB = rate + UCBvalue;
			chosenNode = q;
		}
		q = q->brother;
	}
	if (chosenNode->visit>MAXSIM)
	{
		if (chosenNode->son==NULL)
		{
			bool canExpand= UCTExpand(chosenNode);
			//可以扩展
			if (canExpand)
			{
				chosenNode = UCTChooseSimNode(chosenNode->son);
			}
		}
		else
		{
			chosenNode = UCTChooseSimNode(chosenNode->son);
		}
	}
	return chosenNode;
}

bool PlaySimulation(UCTNode *simulateNode) {
	UCTNode *tempSim = new UCTNode();
	tempSim->move.a = simulateNode->move.a;
	tempSim->move.b = simulateNode->move.b;
	tempSim->move.color = simulateNode->move.color;
	tempSim->frontScore = simulateNode->frontScore;
	tempSim->backScore = simulateNode->backScore;
	tempSim->getScore = simulateNode->getScore;
	MoveType restStep[60];
	int lengthOfrest = 0;

	bool getScore = false;
	MapCopy(simulateNode->currentMap, tempSim->currentMap);
	//找出可以下的边
	for (int i = 1; i < MAPLENGTH-1; i++)
	{
		for (int j = 1; j < MAPLENGTH-1; j++)
		{
			if (((i % 2 == 0 && j % 2 == 1) || (i % 2 == 1 && j % 2 == 0)) && (tempSim->currentMap[i][j] == 0))
			{
				restStep[lengthOfrest].a = i;
				restStep[lengthOfrest].b = j;
				lengthOfrest++;
			}
		}
	}
	if (!tempSim->getScore)//假如没有得分到对手下
	{
		tempSim->move.color = 1-tempSim->move.color;
	}
	//不停rand()直到下完这盘棋
	while (1)
	{
		lengthOfrest--;
		if (lengthOfrest <= -1)//所有边都走完
		{
			break;
		}
		int side = 0;
		
		side = rand() % (lengthOfrest+1);//当前选择的边

		getScore = JudgeGetScore(tempSim, restStep[side].a, restStep[side].b, tempSim);
		if (!getScore)
		{
			tempSim->move.color = 1 - tempSim->move.color;
		}
		tempSim->currentMap[restStep[side].a][restStep[side].b] = 1;//改变数组中边的状态表示访问过了
		 //去掉访问过的边(当前访问的边与最后一个未访问的边交换)
		restStep[side].a = restStep[lengthOfrest].a;
		restStep[side].b= restStep[lengthOfrest].b;
		//test
		restStep[lengthOfrest].a = -99;
		restStep[lengthOfrest].b = -99;
		
	}
	if (tempSim->backScore>tempSim->frontScore)//后手赢棋
	{
		if (OurColor == 1) {
			delete(tempSim);
			return true;
		}
	}
	else
	{
		if (OurColor==0)
		{
			delete(tempSim);
			return true;
		}
	}
	delete(tempSim);
	return false;
}

void UpdateValue(bool AIwin,UCTNode *p) {
	if (AIwin)
	{
		while (p != NULL)
		{
			p->visit++;
			p->win++;
			p=p->father;
		}
	}
	else
	{
		while (p != NULL)
		{
			p->visit++;
			p=p->father;
		}
	}
}

bool JudgeGetScore(UCTNode* judgeNode, int iTemp, int jTemp, UCTNode* change) {
	int flag = -1;
	//边下在第一行
	if (iTemp == 1)
	{
		if (judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp + 2][jTemp] != 0) {
			change->currentMap[iTemp + 1][jTemp] = judgeNode->move.color;
			flag = change->move.color;
		}
	}
	//边下在最后一行
	else if (iTemp == MAPLENGTH - 2)
	{
		if (judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 2][jTemp] != 0) {
			change->currentMap[iTemp - 1][jTemp] = judgeNode->move.color;
			flag = change->move.color;
		}
	}
	//边下在第一列
	else if (jTemp == 1)
	{
		if (judgeNode->currentMap[iTemp + 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp][jTemp + 2] != 0) {
			change->currentMap[iTemp][jTemp + 1] = judgeNode->move.color;
			flag = change->move.color;
		}
	}
	//边下在最后一列
	else if (jTemp == MAPLENGTH - 2)
	{
		if (judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp][jTemp - 2] != 0) {
			change->currentMap[iTemp][jTemp - 1] = judgeNode->move.color;
			flag = change->move.color;
		}
	}
	//边在第二行或倒数第二行
	else if (iTemp == 2 || iTemp == MAPLENGTH - 3)
	{
		if (judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp][jTemp - 2] != 0) {
			change->currentMap[iTemp][jTemp - 1] = judgeNode->move.color;
			flag = change->move.color;
		}
		else if (judgeNode->currentMap[iTemp + 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp][jTemp + 2] != 0)
		{
			change->currentMap[iTemp][jTemp + 1] = judgeNode->move.color;
			flag = change->move.color;
		}

	}
	//边在第二列或倒数第二列
	else if (jTemp == 2 || jTemp == MAPLENGTH - 3)
	{
		if (judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp + 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp + 2][jTemp] != 0) {
			change->currentMap[iTemp + 1][jTemp] = judgeNode->move.color;
			flag = change->move.color;
		}
		else if (judgeNode->currentMap[iTemp - 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 2][jTemp] != 0)
		{
			change->currentMap[iTemp - 1][jTemp] = judgeNode->move.color;
			flag = change->move.color;
		}
	}
	else
	{
		if (judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp + 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp + 2][jTemp] != 0) {
			change->currentMap[iTemp + 1][jTemp] = judgeNode->move.color;
			flag = change->move.color;
		}
		else if (judgeNode->currentMap[iTemp - 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 2][jTemp] != 0)
		{
			change->currentMap[iTemp - 1][jTemp] = judgeNode->move.color;
			flag = change->move.color;
		}
		else if (judgeNode->currentMap[iTemp + 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp - 1] != 0 && judgeNode->currentMap[iTemp][jTemp - 2] != 0) {
			change->currentMap[iTemp][jTemp - 1] = judgeNode->move.color;
			flag = change->move.color;
		}
		else if (judgeNode->currentMap[iTemp + 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp - 1][jTemp + 1] != 0 && judgeNode->currentMap[iTemp][jTemp + 2] != 0)
		{
			change->currentMap[iTemp][jTemp + 1] = judgeNode->move.color;
			flag = change->move.color;
		}
	}

	//如果是后手得分
	if (flag == 1)
	{
		change->backScore++;
		return true;
	}
	//如果是先手得分
	else if (flag == 0)
	{
		change->frontScore++;
		return true;
	}
	return false;
}


//当前状态下在ij位置是否合适
//下在这条边是否会引起周边格子的不自由度变为1
bool suitableStep(UCTNode *rootStrp, int posI, int posJ) {
	bool flagOfSuit = true;
	//第一行
	if (posI==1)
	{
		if (UCTTempMap[posI+1][posJ]+1==3)
		{
			flagOfSuit = false;
		}
	}
	else if (posI==11)//最后一行
	{
		if (UCTTempMap[posI-1][posJ]+1==3)
		{
			flagOfSuit = false;
		}
	}
	else if(posJ==1)//第一列
	{
		if (UCTTempMap[posI][posJ+1]+1==3)
		{
			flagOfSuit = false;
		}
	}
	else if (posJ==11)//最后一列
	{
		if (UCTTempMap[posI][posJ - 1] +1== 3)
		{
			flagOfSuit = false;
		}
	}
	else if(posI%2==0)//中间的竖边
	{
		if (UCTTempMap[posI][posJ+1] +1== 3||UCTTempMap[posI][posJ-1]+1==3)
		{
			flagOfSuit = false;
		}
	}
	else//中间的横边
	{
		if (UCTTempMap[posI +1][posJ]+1 == 3 || UCTTempMap[posI -1][posJ] +1== 3)
		{
			flagOfSuit = false;
		}
	}
	return flagOfSuit;
}
void TempMapSet() {
	for (int i = 1; i < 12; i++)
	{
		for (int j = 1; j < 12; j++)
		{
			if (i%2==0&&j%2==0)//是格子
			{
				UCTTempMap[i][j] = 0;
				if (UCTTempMap[i-1][j]!=0)
				{
					UCTTempMap[i][j]++;
				}
				if (UCTTempMap[i + 1][j] != 0)
				{
					UCTTempMap[i][j]++;
				}
				if (UCTTempMap[i ][j-1] != 0)
				{
					UCTTempMap[i][j]++;
				}
				if (UCTTempMap[i][j + 1] != 0)
				{
					UCTTempMap[i][j]++;
				}
			}
		}
	}
	
}