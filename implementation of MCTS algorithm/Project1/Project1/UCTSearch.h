#pragma once
#ifndef _UCTSEARCH_H_H_
#define _UCTSEARCH_H_H_

void UCTSearch();
bool UCTExpand(UCTNode *root);
UCTNode * UCTChooseSimNode(UCTNode *head);
bool PlaySimulation(UCTNode *simulateNode);
void UpdateValue(bool AIwin, UCTNode *p);
bool JudgeGetScore(UCTNode* root, int i, int j, UCTNode* node);
bool suitableStep(UCTNode *rootStrp, int posI, int posJ);
void TempMapSet();
#endif