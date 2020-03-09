#include<stdio.h>
#include<string.h>
#include<time.h>
#include"Moves.h"
#include"Define.h"
#include"Evaluate.h"
#include"UCTSearch.h"

int chainstack[20][3];     //存放链环信息
time_t start, end;
int GlobalAlpha;

//临时map&state初始化
void init_temmap()
{
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			TemMap[i][j] = Map[i][j];
			TemState[i][j] = State[i][j];
		}
	}
}

//估计剩余Step
int GetStep()
{
	init_temmap();
	int count = 0;
	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if (TemMap[i][j] == 0)
			{
				if (TemState[i][j] == LINEONE)
				{
					if (TemState[i + 1][j] < 2 && TemState[i - 1][j] < 2)
					{
						count++;
						TemState[i + 1][j]++;
						TemState[i - 1][j]++;
						TemMap[i][j] = LINEONE;
					}
				}
				else
				{
					if (TemState[i][j + 1] < 2 && TemState[i][j - 1] < 2)
					{
						count++;
						TemState[i][j + 1]++;
						TemState[i][j - 1]++;
						TemMap[i][j] = LINETWO;
					}
				}
			}
		}
	}
	return count;
}

//确定链长并区分链&环
int dpMakeMove(int a, int b)
{
	int count = 0;
	int flag = 0;    //单步吃格为2,说明为环,count加100标记
	if (TemState[a][b] == LINEONE)
	{
		TemMap[a][b] = LINEONE;
		TemState[a + 1][b]++;
		TemState[a - 1][b]++;
		if (TemState[a + 1][b] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a + 1 + dx[k]][b + dy[k]] == 0)
				{
					count += dpMakeMove(a + 1 + dx[k], b + dy[k]);
				}
			}
		}
		else if (TemState[a + 1][b] == 4)
		{
			count++;
			flag++;
		}
		if (TemState[a - 1][b] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a - 1 + dx[k]][b + dy[k]] == 0)
				{
					count += dpMakeMove(a - 1 + dx[k], b + dy[k]);
				}
			}
		}
		else if (TemState[a - 1][b] == 4)
		{
			count++;
			flag++;
		}
	}
	else
	{
		TemMap[a][b] = LINETWO;
		TemState[a][b + 1]++;
		TemState[a][b - 1]++;
		if (TemState[a][b + 1] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a + dx[k]][b + 1 + dy[k]] == 0)
				{
					count += dpMakeMove(a + dx[k], b + 1 + dy[k]);
				}
			}
		}
		else if (TemState[a][b + 1] == 4)
		{
			count++;
			flag++;
		}
		if (TemState[a][b - 1] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a + dx[k]][b - 1 + dy[k]] == 0)
				{
					count += dpMakeMove(a + dx[k], b - 1 + dy[k]);
				}
			}
		}
		else if (TemState[a][b - 1] == 4)
		{
			count++;
			flag++;
		}
	}
	if (flag == 2)
	{
		count += 100;
	}
	return count;
}

//判断送格数目;1格可送,返回0;2格及以上不能,返回1
int dpAlphaBeta()
{
	int flag = 0;    //1格链2格环4格环可送,返回0;2格链及以上6格环及以上不能,返回1
	init_temmap();
	for (int i = 2; i <= 10; i += 2)
	{
		for (int j = 2; j <= 10; j += 2)
		{
			if (TemState[i][j] == 3)
			{
				for (int k = 0; k < 4; k++)
				{
					if (TemMap[i + dx[k]][j + dy[k]] == 0)
					{
						int tem = dpMakeMove(i + dx[k], j + dy[k]);
						if (tem > 104 || (tem > 1 && tem < 100))
						{
							return 1;
						}
					}
				}
			}
		}
	}
	return 0;
}

int AlphaBeta(int depth, int alpha, int beta, int color, int eat, int last)
{
	if (judge() == 1)
	{
		return Evaluate(color) + eat * 10;
	}
	if (depth == 0)
	{
		return eat * 10;
	}
	int val;
	MoveType nowmove;

	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if ((State[i][j] == 11 || State[i][j] == 22) && Map[i][j] == 0)
			{
				nowmove.a = i;
				nowmove.b = j;
				nowmove.color = color;
				int temeat;
				temeat = MakeMove(nowmove, last);
				if (temeat > 0)
				{
					val = AlphaBeta(depth, alpha, beta, color, eat + temeat, color);
					UnMakeMove(nowmove);
					if (val >= beta)
					{
						return beta;
					}
					if (val > alpha)
					{
						alpha = val;
					}
					return alpha;
				}
				else
				{
					UnMakeMove(nowmove);
				}
			}
		}
	}

	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if ((State[i][j] == 11 || State[i][j] == 22) && Map[i][j] == 0)
			{
				nowmove.a = i;
				nowmove.b = j;
				nowmove.color = color;
				MakeMove(nowmove, last);
				if (dpAlphaBeta() == 1)
				{
					UnMakeMove(nowmove);
					continue;
				}
				val = -AlphaBeta(depth - 1, -beta, -alpha, 1 - color, -eat, color);
				UnMakeMove(nowmove);
				if (val >= beta)
				{
					return beta;
				}
				if (val > alpha)
				{
					alpha = val;
				}
			}
		}
	}
	return alpha;
}

void AlphaBetaFirst(int depth, int alpha, int beta, int color)
{
	int val;
	MoveType nowmove;
	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if ((State[i][j] == 11 || State[i][j] == 22) && Map[i][j] == 0)
			{
				nowmove.a = i;
				nowmove.b = j;
				nowmove.color = color;
				int eat;
				eat = MakeMove(nowmove, 1 - color);
				if (dpAlphaBeta() == 1)      //送格剪枝
				{
					UnMakeMove(nowmove);
					continue;
				}
				if (eat > 0)
				{
					val = AlphaBeta(depth - 1, alpha, beta, color, eat, color);
					UnMakeMove(nowmove);
				}
				else
				{
					val = -AlphaBeta(depth - 1, -beta, -alpha, 1 - color, 0, color);
					UnMakeMove(nowmove);
				}
				if (val > alpha)
				{
					alpha = val;
					GlobalAlpha = val;
					HopeVal = val;
					BestMove = nowmove;
				}
			}
			/*end = time(NULL);
			if (end - start > 4)
			{
			return;
			}*/
		}
	}
	return;
}

//type=1:吃了送,搜索可吃链并决定招法;type=2:只搜索可吃链;
int dp(int type)
{
	int flag = 0;    //是否有可吃链（链最短长度可为1）
	memset(chainstack, 0, sizeof(chainstack));

	init_temmap();
	for (int i = 2; i <= 10; i += 2)
	{
		for (int j = 2; j <= 10; j += 2)
		{
			if (TemState[i][j] == 3)
			{
				for (int k = 0; k < 4; k++)
				{
					if (TemMap[i + dx[k]][j + dy[k]] == 0)
					{
						chainstack[0][0]++;    //链数目
						chainstack[chainstack[0][0]][0] = dpMakeMove(i + dx[k], j + dy[k]);     //链长
						if (chainstack[chainstack[0][0]][0] == 2)
						{
							chainstack[0][1]++;      //长度为2的链的数目
						}
						chainstack[chainstack[0][0]][1] = i + dx[k];
						chainstack[chainstack[0][0]][2] = j + dy[k];
						flag = 1;
					}
				}
			}
		}
	}

	init_temmap();
	if (type == 1)
	{
		//链长为1直接吃
		for (int i = 1; i <= chainstack[0][0]; i++)
		{
			if (chainstack[i][0] == 1)
			{
				BestMove.a = chainstack[i][1];
				BestMove.b = chainstack[i][2];
				return flag;
			}
		}
		//链长大于2且不为环吃1格
		for (int i = 1; i <= chainstack[0][0]; i++)
		{
			if (chainstack[i][0] > 2 && chainstack[i][0] < 100)
			{
				BestMove.a = chainstack[i][1];
				BestMove.b = chainstack[i][2];
				return flag;
			}
		}
		//链长为2送2
		for (int i = 1; i <= chainstack[0][0]; i++)
		{
			if (chainstack[i][0] == 2)
			{
				int deadbox = 0;
				for (int i = 2; i <= 10; i += 2)
				{
					for (int j = 2; j <= 10; j += 2)
					{
						if (Map[i][j] == -77)
						{
							deadbox++;
						}
					}
				}
				//终局不送
				if (deadbox + 2 == 25)
				{
					BestMove.a = chainstack[i][1];
					BestMove.b = chainstack[i][2];
					return flag;
				}
				if (chainstack[0][1] > 1)
				{
					BestMove.a = chainstack[i][1];
					BestMove.b = chainstack[i][2];
					return flag;
				}
				int a = chainstack[i][1];
				int b = chainstack[i][2];
				if (TemState[a][b] == LINEONE)
				{
					TemMap[a][b] = LINEONE;
					TemState[a + 1][b]++;
					TemState[a - 1][b]++;
					if (TemState[a + 1][b] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a + 1 + dx[k]][b + dy[k]] == 0)
							{
								BestMove.a = a + 1 + dx[k];
								BestMove.b = b + dy[k];
							}
						}
					}
					if (TemState[a - 1][b] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a - 1 + dx[k]][b + dy[k]] == 0)
							{
								BestMove.a = a - 1 + dx[k];
								BestMove.b = b + dy[k];
							}
						}
					}
				}
				else
				{
					TemMap[a][b] = LINETWO;
					TemState[a][b + 1]++;
					TemState[a][b - 1]++;
					if (TemState[a][b + 1] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a + dx[k]][b + 1 + dy[k]] == 0)
							{
								BestMove.a = a + dx[k];
								BestMove.b = b + 1 + dy[k];
							}
						}
					}
					if (TemState[a][b - 1] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a + dx[k]][b - 1 + dy[k]] == 0)
							{
								BestMove.a = a + dx[k];
								BestMove.b = b - 1 + dy[k];
							}
						}
					}
				}
				return flag;
			}
		}
		//链长大于5的环或为3为2的环吃1格
		for (int i = 1; i <= chainstack[0][0]; i++)
		{
			if (chainstack[i][0] >= 105 || chainstack[i][0] == 103 || chainstack[i][0] == 102)
			{
				BestMove.a = chainstack[i][1];
				BestMove.b = chainstack[i][2];
				return flag;
			}
		}
		//环长为4送4
		for (int i = 1; i <= chainstack[0][0]; i++)
		{
			if (chainstack[i][0] == 104)
			{
				int deadbox = 0;
				for (int i = 2; i <= 10; i += 2)
				{
					for (int j = 2; j <= 10; j += 2)
					{
						if (Map[i][j] == -77)
						{
							deadbox++;
						}
					}
				}
				//终局不送
				if (deadbox + 4 == 25)
				{
					BestMove.a = chainstack[i][1];
					BestMove.b = chainstack[i][2];
					return flag;
				}
				int a = chainstack[i][1];
				int b = chainstack[i][2];
				if (TemState[a][b] == LINEONE)
				{
					TemMap[a][b] = LINEONE;
					TemState[a + 1][b]++;
					TemState[a - 1][b]++;
					if (TemState[a + 1][b] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a + 1 + dx[k]][b + dy[k]] == 0)
							{
								BestMove.a = a + 1 + dx[k];
								BestMove.b = b + dy[k];
							}
						}
					}
					if (TemState[a - 1][b] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a - 1 + dx[k]][b + dy[k]] == 0)
							{
								BestMove.a = a - 1 + dx[k];
								BestMove.b = b + dy[k];
							}
						}
					}
				}
				else
				{
					TemMap[a][b] = LINETWO;
					TemState[a][b + 1]++;
					TemState[a][b - 1]++;
					if (TemState[a][b + 1] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a + dx[k]][b + 1 + dy[k]] == 0)
							{
								BestMove.a = a + dx[k];
								BestMove.b = b + 1 + dy[k];
							}
						}
					}
					if (TemState[a][b - 1] == 3)
					{
						for (int k = 0; k < 4; k++)
						{
							if (TemMap[a + dx[k]][b - 1 + dy[k]] == 0)
							{
								BestMove.a = a + dx[k];
								BestMove.b = b - 1 + dy[k];
							}
						}
					}
				}
				return flag;
			}
		}
	}
	else if (type == 2)
	{
		return flag;
	}
	return flag;
}

void Search(int color)
{
	GlobalAlpha = -999;
	BestMove.color = color;
	UCTSearch();
	//if (judge() == 0)
	//{
	//	if (dp(2) == 1)
	//	{
	//		//链长为1直接吃
	//		for (int i = 1; i <= chainstack[0][0]; i++)
	//		{
	//			if (chainstack[i][0] == 1)
	//			{
	//				BestMove.a = chainstack[i][1];
	//				BestMove.b = chainstack[i][2];
	//				return;
	//			}
	//		}
	//		//链长大于2且不为环吃1格
	//		for (int i = 1; i <= chainstack[0][0]; i++)
	//		{
	//			if (chainstack[i][0] > 2 && chainstack[i][0] < 100)
	//			{
	//				BestMove.a = chainstack[i][1];
	//				BestMove.b = chainstack[i][2];
	//				return;
	//			}
	//		}
	//		//链长为2的链数量大于1吃一个链
	//		for (int i = 1; i <= chainstack[0][0]; i++)
	//		{
	//			if (chainstack[i][0] == 2 && chainstack[0][1] > 1)
	//			{
	//				BestMove.a = chainstack[i][1];
	//				BestMove.b = chainstack[i][2];
	//				return;
	//			}
	//		}
	//		//链长大于5的环或为3为2的环吃1格
	//		for (int i = 1; i <= chainstack[0][0]; i++)
	//		{
	//			if (chainstack[i][0] >= 105 || chainstack[i][0] == 103 || chainstack[i][0] == 102)
	//			{
	//				BestMove.a = chainstack[i][1];
	//				BestMove.b = chainstack[i][2];
	//				return;
	//			}
	//		}

	//	}

	//	int depth = 6;
	//	Step = GetStep();
	//	/*if (Step < 120)
	//	{
	//	start = time(NULL);
	//	end = time(NULL);
	//	while (end - start < 10)
	//	{
	//	AlphaBetaFirst(depth++, GlobalAlpha, 999, color);
	//	end = time(NULL);
	//	}

	//	}*/
	//	if (Step < 10)
	//	{
	//		AlphaBetaFirst(12, GlobalAlpha, 999, color);
	//	}
	//	else
	//	{
	//		AlphaBetaFirst(6, GlobalAlpha, 999, color);
	//	}
	//	/*else
	//	{
	//	UCTSearch();
	//	}*/

	//	return;
	//}
	//else
	//{
	//	init_temmap();

	//	if (dp(1) == 1)
	//	{
	//		return;
	//	}
	//	if (trap2() > 0)
	//	{
	//		for (int i = 1; i <= TrapStack[0][0]; i++)
	//		{
	//			if (TrapStack[i][0] == 1)
	//			{
	//				BestMove.a = TrapStack[i][1];
	//				BestMove.b = TrapStack[i][2];
	//				TrapStack[i][0] = 0;
	//				return;
	//			}
	//		}
	//		for (int i = 1; i <= TrapStack[0][0]; i++)
	//		{
	//			if (TrapStack[i][0] == 2)
	//			{
	//				BestMove.a = TrapStack[i][1];
	//				BestMove.b = TrapStack[i][2];
	//				TrapStack[i][0] = 0;
	//				return;
	//			}
	//		}
	//	}
	//}
}