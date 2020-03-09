#include<stdio.h>
#include"Define.h"

//最终决定招法的移动
int StaticMakeMove(MoveType &nowmove, int lastcolor)
{
	int eat = 0;
	if (State[nowmove.a][nowmove.b] == LINEONE)
	{
		Map[nowmove.a][nowmove.b] = LINEONE;
		State[nowmove.a + 1][nowmove.b]++;
		State[nowmove.a - 1][nowmove.b]++;
		if (State[nowmove.a + 1][nowmove.b] == 4)
		{
			Map[nowmove.a + 1][nowmove.b] = -77;
			eat++;
		}
		if (State[nowmove.a - 1][nowmove.b] == 4)
		{
			Map[nowmove.a - 1][nowmove.b] = -77;
			eat++;
		}
	}
	else
	{
		Map[nowmove.a][nowmove.b] = LINETWO;
		State[nowmove.a][nowmove.b + 1]++;
		State[nowmove.a][nowmove.b - 1]++;
		if (State[nowmove.a][nowmove.b + 1] == 4)
		{
			Map[nowmove.a][nowmove.b + 1] = -77;
			eat++;
		}
		if (State[nowmove.a][nowmove.b - 1] == 4)
		{
			Map[nowmove.a][nowmove.b - 1] = -77;
			eat++;
		}
	}
	if (eat > 0)
	{
		if (lastcolor != nowmove.color)
		{
			ValExch += 2;
		}
		else
		{
			ValExch++;
		}
	}
	Score[nowmove.color] += eat;
	LastColor = nowmove.color;
	return eat;
}

int MakeMove(MoveType &nowmove, int lastcolor)
{
	int eat = 0;
	if (State[nowmove.a][nowmove.b] == LINEONE)
	{
		Map[nowmove.a][nowmove.b] = LINEONE;
		State[nowmove.a + 1][nowmove.b]++;
		State[nowmove.a - 1][nowmove.b]++;
		if (State[nowmove.a + 1][nowmove.b] == 4)
		{
			Map[nowmove.a + 1][nowmove.b] = -77;
			eat++;
		}
		if (State[nowmove.a - 1][nowmove.b] == 4)
		{
			Map[nowmove.a - 1][nowmove.b] = -77;
			eat++;
		}
	}
	else
	{
		Map[nowmove.a][nowmove.b] = LINETWO;
		State[nowmove.a][nowmove.b + 1]++;
		State[nowmove.a][nowmove.b - 1]++;
		if (State[nowmove.a][nowmove.b + 1] == 4)
		{
			Map[nowmove.a][nowmove.b + 1] = -77;
			eat++;
		}
		if (State[nowmove.a][nowmove.b - 1] == 4)
		{
			Map[nowmove.a][nowmove.b - 1] = -77;
			eat++;
		}
	}
	if (eat > 0)
	{
		if (lastcolor != nowmove.color)
		{
			ValExch += 2;
			nowmove.exchange = 2;
		}
		else
		{
			ValExch++;
			nowmove.exchange = 1;
		}
	}
	else
	{
		nowmove.exchange = 0;
	}
	return eat;
}

void UnMakeMove(MoveType &nowmove)
{
	ValExch -= nowmove.exchange;
	Map[nowmove.a][nowmove.b] = 0;
	if (State[nowmove.a][nowmove.b] == LINEONE)
	{
		State[nowmove.a + 1][nowmove.b]--;
		State[nowmove.a - 1][nowmove.b]--;
		if (State[nowmove.a + 1][nowmove.b] == 3)
		{
			Map[nowmove.a + 1][nowmove.b] = 0;
		}
		if (State[nowmove.a - 1][nowmove.b] == 3)
		{
			Map[nowmove.a - 1][nowmove.b] = 0;
		}
	}
	else
	{
		State[nowmove.a][nowmove.b + 1]--;
		State[nowmove.a][nowmove.b - 1]--;
		if (State[nowmove.a][nowmove.b + 1] == 3)
		{
			Map[nowmove.a][nowmove.b + 1] = 0;
		}
		if (State[nowmove.a][nowmove.b - 1] == 3)
		{
			Map[nowmove.a][nowmove.b - 1] = 0;
		}
	}
	return;
}