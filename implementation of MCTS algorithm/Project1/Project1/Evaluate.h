#pragma once
#ifndef _EVALUATE_H_H_
#define _EVALUATE_H_H_

int judge();  //判断是否进入残局阶段
int trap();   //搜索换手Trap-只搜索数目
int trap2();  //搜索换手Trap-搜索数目并记录位置
int Evaluate(int color);

#endif