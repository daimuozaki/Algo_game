#ifndef PLAY_GAME_H
#define PLAY_GAME_H

#include "HandAI.h"

void InitDeck();
void InitGame();
void MakeFile();
int PlayGame();
void GameMenu();
void StartGame();
void SoatCard(int);
int DrawCard();
void Attack();
void GetHumanHand();
int CheckResult();
void CheckGameSet();
void ChangePlayer();
void WriteBoard();
void ShowBoard(int);

#endif // !PLAY_GAME_H
