#ifndef HAND_AI_H
#define HAND_AI_H

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//�J�[�h�̍ő喇��
#define DECKCARD 24
#define MAX_CARD 12
//�v���C���[�̎�ނ̒�`
#define HUMAN 1
#define RAND_AI 2
#define BASE_AI 3
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

//�v���C���[�̃f�[�^���i�[����\����
typedef struct {
	int playerKind;
	int card[MAX_CARD];	//�J�[�h�͑S����24���A�v���C���[�Ԃł̃J�[�h�̈ړ��͂Ȃ�����1�l�ő�12��
	int getCard;		//���̃^�[���Ɉ������J�[�h�A�A�^�b�N���s���Ɍ��J����K�v������
	int clearCard[MAX_CARD];	//�I�[�v���ɂȂ����J�[�h�Ƃ����łȂ��J�[�h�̔���
	int outsideCard[DECKCARD];	//AI�Ŏg�p�A���łɌ��J����Ă���A�����Ă���ȂǂŐ������O�̃J�[�h���܂Ƃ߂�
	int cardNum;		//���݂̃J�[�h�̑�����
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