#include<stdio.h>
#include<string.h>
#include"Define.h"
#include"Moves.h"

//判断是否进入残局阶段
int judge()
{
	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if (Map[i][j] == 0 && State[i][j] > 10)
			{
				if (State[i][j] == LINEONE)
				{
					if (State[i + 1][j] + 1 < 3 && State[i - 1][j] + 1 < 3)
					{
						return 0;
					}
				}
				else
				{
					if (State[i][j + 1] + 1 < 3 && State[i][j - 1] + 1 < 3)
					{
						return 0;
					}
				}
			}
		}
	}
	return 1;     //进入残局阶段
}

void UnTemMakeMove(int a, int b)
{
	TemMap[a][b] = 0;
	if (TemState[a][b] == LINEONE)
	{
		TemState[a + 1][b]--;
		TemState[a - 1][b]--;
	}
	else
	{
		TemState[a][b + 1]--;
		TemState[a][b - 1]--;
	}
}

//trap检测第二步
int TemMakeMove2(int a, int b)
{
	if (TemState[a][b] == LINEONE)
	{
		TemMap[a][b] = LINEONE;
		TemState[a + 1][b]++;
		TemState[a - 1][b]++;
		if (TemState[a + 1][b] + TemState[a - 1][b] >= 7)
		{
			return 0;
		}
	}
	else
	{
		TemMap[a][b] = LINETWO;
		TemState[a][b + 1]++;
		TemState[a][b - 1]++;
		if (TemState[a][b + 1] + TemState[a][b - 1] >= 7)
		{
			return 0;
		}
	}
	return 1;
}

//trap检测第一步
int TemMakeMove(int a, int b)
{
	int count = 0;
	if (TemState[a][b] == LINEONE)
	{
		int x = -1, y = -1;
		TemMap[a][b] = LINEONE;
		TemState[a + 1][b]++;
		TemState[a - 1][b]++;
		if (TemState[a + 1][b] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a + 1 + dx[k]][b + dy[k]] == 0)
				{
					x = a + 1 + dx[k];
					y = b + dy[k];
					if (TemMakeMove2(x, y) == 0)
					{
						UnTemMakeMove(x, y);
						return 0;
					}
				}
			}
			count++;
		}
		else if (TemState[a + 1][b] == 4)
		{
			return 0;
		}
		if (TemState[a - 1][b] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a - 1 + dx[k]][b + dy[k]] == 0)
				{
					if (TemMakeMove2(a - 1 + dx[k], b + dy[k]) == 0)
					{
						UnTemMakeMove(a - 1 + dx[k], b + dy[k]);
						if (x > 0)
						{
							UnTemMakeMove(x, y);
						}
						return 0;
					}
				}
			}
			count++;
		}
		else if (TemState[a - 1][b] == 4)
		{
			if (x > 0)
			{
				UnTemMakeMove(x, y);
			}
			return 0;
		}
	}
	else
	{
		int x = -1, y = -1;
		TemMap[a][b] = LINETWO;
		TemState[a][b + 1]++;
		TemState[a][b - 1]++;
		if (TemState[a][b + 1] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a + dx[k]][b + 1 + dy[k]] == 0)
				{
					x = a + dx[k];
					y = b + 1 + dy[k];
					if (TemMakeMove2(x, y) == 0)
					{
						UnTemMakeMove(x, y);
						return 0;
					}
				}
			}
			count++;
		}
		else if (TemState[a][b + 1] == 4)
		{
			return 0;
		}
		if (TemState[a][b - 1] == 3)
		{
			for (int k = 0; k < 4; k++)
			{
				if (TemMap[a + dx[k]][b - 1 + dy[k]] == 0)
				{
					if (TemMakeMove2(a + dx[k], b - 1 + dy[k]) == 0)
					{
						UnTemMakeMove(a + dx[k], b - 1 + dy[k]);
						if (x > 0)
						{
							UnTemMakeMove(x, y);
						}
						return 0;
					}
				}
			}
			count++;
		}
		else if (TemState[a][b - 1] == 4)
		{
			if (x > 0)
			{
				UnTemMakeMove(x, y);
			}
			return 0;
		}
	}
	return count;
}

//搜索换手Trap-只搜索数目
int trap()
{
	int count = 0;
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			TemMap[i][j] = Map[i][j];
			TemState[i][j] = State[i][j];
		}
	}
	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if (TemMap[i][j] == 0 && (TemState[i][j] == 11 || TemState[i][j] == 22))
			{
				if (TemMakeMove(i, j) > 0)
				{
					count++;
				}
				else
				{
					UnTemMakeMove(i, j);
				}
			}
		}
	}
	return count;
}

//搜索换手Trap-搜索数目并记录位置
int trap2()
{
	int count = 0;
	memset(TrapStack, 0, sizeof(TrapStack));
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 13; j++)
		{
			TemMap[i][j] = Map[i][j];
			TemState[i][j] = State[i][j];
		}
	}
	for (int i = 1; i <= 11; i++)
	{
		for (int j = 1; j <= 11; j++)
		{
			if (TemMap[i][j] == 0 && (TemState[i][j] == 11 || TemState[i][j] == 22))
			{
				int k = TemMakeMove(i, j);
				if (k > 0)
				{
					TrapStack[0][0]++;
					TrapStack[TrapStack[0][0]][0] = k;
					TrapStack[TrapStack[0][0]][1] = i;
					TrapStack[TrapStack[0][0]][2] = j;
					count++;
				}
				else
				{
					UnTemMakeMove(i, j);
				}
			}
		}
	}
	return count;
}

int Evaluate(int color)
{
	int Val;
	Val = trap();
	if (Val % 2 == 1)
	{
		return  105;
	}
	else
	{
		return -105;
	}
}