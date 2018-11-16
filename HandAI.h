#ifndef HAND_AI_H
#define HAND_AI_H

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <direct.h>

//�J�[�h�̍ő喇��
#define DECKCARD 24
#define MAX_CARD 12
//�v���C���[�̎�ނ̒�`
#define HUMAN 1
#define RAND_AI 2
#define BASE_AI 3
#define SIDE_AI 4
#define CENTER_AI 5
#define BRUFF_AI 6
//�v���C���[�����A�Ƃ肠����2�l��
#define PLAYER_NUM 2
//�J�[�h��
#define COVERED 0
#define CLEAR 1
//�J�[�h�̐F�ɂ��Ē�`
#define BLACK 0
#define WHITE 1
//�v���C���[�ԍ��A�\���̂Ƃ̌��ˍ����ł����Ȃ���
#define PLAYER_1 0
#define PLAYER_2 1
//���͂ł��镶����
#define LEN 32
//�Ֆʕ\�����̃��[�h
#define NORMAL_MODE 0
#define SHOW_MODE 1
//attacks�ŗp����
#define ATTACK_NUM 0
#define SUCCESS_NUM 1

//�v���C���[�̃f�[�^���i�[����\����
typedef struct {
	int playerKind;
	int card[MAX_CARD];	//�J�[�h�͑S����24���A�v���C���[�Ԃł̃J�[�h�̈ړ��͂Ȃ�����1�l�ő�12��
	int getCard;		//���̃^�[���Ɉ������J�[�h�A�A�^�b�N���s���Ɍ��J����K�v������
	char clearCard[MAX_CARD];	//�I�[�v���ɂȂ����J�[�h�Ƃ����łȂ��J�[�h�̔���
	int outsideCard[DECKCARD];	//AI�Ŏg�p�A���łɌ��J����Ă���A�����Ă���ȂǂŐ������O�̃J�[�h���܂Ƃ߂�
	int toldCard[MAX_CARD];		//���肪�錾�����J�[�h����荞��
	int cardNum;		//���݂̃J�[�h�̑�����
	int WinNum;			//�A�펞�̏�����
	int attacks[2];		//�A�^�b�N�̊m�F�p�z��A0���A�^�b�N������1��������
}Player;

void GetRandomAIHand();
void GetBaseAIHand();
void GetSideAIHand();
void GetCenterAIHand();
void GetBruffAIHand();
int JudgeColor(int);
bool JudgeNum(int, int);
int GetCardNum(int);
int CheckClear(int);
int GetRandomNum(int, int);
void LnTrim(char *);

#endif // !HAND_AI_H