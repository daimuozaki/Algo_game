#ifndef HAND_AI_H
#define HAND_AI_H

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//カードの最大枚数
#define DECKCARD 24
#define MAX_CARD 12
//プレイヤーの種類の定義
#define HUMAN 1
#define RAND_AI 2
#define BASE_AI 3
//プレイヤー総数、とりあえず2人で
#define PLAYER_NUM 2
//カード状況
#define COVERED 0
#define CLEAR 1
//カードの色について定義
#define BLACK 0
#define WHITE 1
//プレイヤー番号、構造体との兼ね合いでこうなった
#define PLAYER_1 0
#define PLAYER_2 1
//入力できる文字列長
#define LEN 32
//盤面表示時のモード
#define NORMAL_MODE 0
#define SHOW_MODE 1

//プレイヤーのデータを格納する構造体
typedef struct {
	int playerKind;
	int card[MAX_CARD];	//カードは全部で24枚、プレイヤー間でのカードの移動はないため1人最大12枚
	int getCard;		//このターンに引いたカード、アタック失敗時に公開する必要がある
	int clearCard[MAX_CARD];	//オープンになったカードとそうでないカードの判定
	int outsideCard[DECKCARD];	//AIで使用、すでに公開されている、持っているなどで推理候補外のカードをまとめる
	int cardNum;		//現在のカードの総枚数
}Player;

void GetRandomAIHand();
void GetBaseAIHand();
int JudgeColor(int);
bool JudgeNum(int, int);
int GetCardNum(int);
int CheckClear(int);
int GetRandomNum(int, int);
void LnTrim(char *);

#endif // !HAND_AI_H