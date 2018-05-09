#ifndef PLAY_GAME_H
#define PLAY_GAME_H

#include "HandAI.h"

void InitDeck();
void InitGame();
void MakeFile();
int PlayGame();
void StartGame();
void SoatCard(int);
int DrawCard();
void Attack();
void GetHumanHand();
void GetRandomAIHand();
int JudgeColor(int);
int CheckResult();
int CheckClear(int);
void CheckGameSet();
void ChangePlayer();
bool JudgeNum(int, int);
int GetCardNum(int);
void ShowBoard();
void WriteBoard();

#endif // !PLAY_GAME_H
