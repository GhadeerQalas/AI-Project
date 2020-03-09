#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<string.h>
#include"Define.h"
#include"Search.h"
#include"Moves.h"
#include"Evaluate.h"


MoveType BestMove;
int Map[13][13];
int State[13][13];
int TemMap[13][13];
int TemState[13][13];
int dx[4] = { 0,1,-1,0 };
int dy[4] = { 1,0,0,-1 };

int ValExch;
int OurColor;       //记录我方颜色,0先手,1后手
int LastColor;      //上一步颜色
int TrapStack[20][3];
int Score[2];
int Step;
int HopeVal;


//加载残局
//void load_map() {
//	int load[13][13] = {
//		{99,99,99,99,99,99,99,99,99,99,99,99,99},
//		{99, 0,11, 0,11, 0,11, 0, 0, 0,11, 0,99},
//		{99, 0, 0, 0, 0, 0, 0, 0, 0,22, 0,22,99},
//		{99, 0,11, 0,11, 0, 0, 0, 0, 0,11, 0,99},
//		{99,22, 0,22, 0, 0, 0,22, 0, 0, 0,22,99},
//		{99, 0,11, 0,11, 0,11, 0,11, 0, 0, 0,99},
//		{99,22, 0,22, 0,22, 0,22, 0,22, 0,22,99},
//		{99, 0,11, 0,11, 0,11, 0,11, 0, 0, 0,99},
//		{99,22, 0,22, 0,22, 0,22, 0,22, 0,22,99},
//		{99, 0,11, 0,11, 0,11, 0,11, 0,11, 0,99},
//		{99,22, 0,22, 0,22, 0,22, 0,22, 0,22,99},
//		{99, 0,11, 0,11, 0,11, 0,11, 0,11, 0,99},
//		{99,99,99,99,99,99,99,99,99,99,99,99,99},
//
//	};
//	for (int i = 0; i < 13; i++)
//	{
//		for (int j = 0; j < 13; j++)
//		{
//			Map[i][j] = load[i][j];
//		}
//	}
//}

//初始化棋盘
void init_map()
{
	Step = 0;
	ValExch = 0;
	LastColor = 0;
	Score[0] = 0;
	Score[1] = 0;

	memset(Map, 0, sizeof(Map));
	memset(State, 0, sizeof(State));
	for (int i = 0; i < 13; i++)
	{
		Map[0][i] = BORDER;
		Map[12][i] = BORDER;
		Map[i][0] = BORDER;
		Map[i][12] = BORDER;
		State[0][i] = BORDER;
		State[12][i] = BORDER;
		State[i][0] = BORDER;
		State[i][12] = BORDER;
	}
	int A = 1, B = 2;
	for (int i = 0; i < 6; i++)
	{
		B = 2;
		for (int j = 0; j < 5; j++)
		{
			State[A][B] = LINEONE;
			State[B][A] = LINETWO;
			B += 2;
		}
		A += 2;
	}
}

//输出到UI
void output()
{
	char move[] = "move AA\n";	//走法
	move[5] = BestMove.a - 1 + 'A';
	move[6] = BestMove.b - 1 + 'A';
	printf("%s", move);
	fflush(stdout);
}


int main()
{
	init_map();
	char Msg[500] = { 0 };		//保存接收到的消息
	char name[] = "name D-Project1.0\n";	//队伍信息

	while (1)
	{
		//循环接收裁判平台发送的消息
		//发送完需要调用fflush(stdout)清空输出缓冲区,使字符串立刻输出到裁判平台

		memset(Msg, 0, 500);
		scanf("%s", Msg);

		if (strcmp(Msg, "name?") == 0)
		{
			printf("%s", name);
			fflush(stdout);
			continue;
		}
		if (strcmp(Msg, "stop") == 0)
		{
			fflush(stdout);
			return 0;
		}
		if (strcmp(Msg, "new") == 0)
		{
			init_map();
			/*load_map();
			OurColor = 0;*/
			scanf("%s", Msg);
			if (strcmp(Msg, "first") == 0)
			{
				OurColor = 0;

				Search(OurColor);
				/*scanf("%s", Msg);
				BestMove.a = Msg[0] - 'A' + 1;
				BestMove.b = Msg[1] - 'A' + 1;
				BestMove.color = OurColor;*/

				StaticMakeMove(BestMove, LastColor);
				output();

				continue;
			}
			else
			{
				OurColor = 1;
				continue;
			}
		}
		if (strcmp(Msg, "move") == 0)
		{
			scanf("%s", Msg);
			BestMove.a = Msg[0] - 'A' + 1;
			BestMove.b = Msg[1] - 'A' + 1;
			BestMove.color = 1 - OurColor;

			if (StaticMakeMove(BestMove, LastColor) > 0)
			{
				continue;
			}

			Search(OurColor);
			/*scanf("%s", Msg);
			BestMove.a = Msg[0] - 'A' + 1;
			BestMove.b = Msg[1] - 'A' + 1;
			BestMove.color = OurColor;*/

			while (StaticMakeMove(BestMove, LastColor) > 0)
			{
				output();
				Search(OurColor);
				/*scanf("%s", Msg);
				BestMove.a = Msg[0] - 'A' + 1;
				BestMove.b = Msg[1] - 'A' + 1;
				BestMove.color = OurColor;*/
			}
			output();
		}
		if (strcmp(Msg, "exit") == 0)
		{
			return 0;
		}
	}
	return 0;
}