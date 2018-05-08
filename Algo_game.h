/*�w�b�_�[�t�@�C��
�@����Ă邱�Ƃ͍\���̓��̒�`�ƃv���g�^�C�v�錾
*/

#ifndef ALGO_GAME_H
#define ALGO_GAME_H

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

//�v���C���[�̃f�[�^���i�[����\����
typedef struct {
	int playerKind;
	int card[MAX_CARD];	//�J�[�h�͑S����24���A�v���C���[�Ԃł̃J�[�h�̈ړ��͂Ȃ�����1�l�ő�12��
	int getCard;		//���̃^�[���Ɉ������J�[�h�A�A�^�b�N���s���Ɍ��J����K�v������
	int clearCard[MAX_CARD];	//�I�[�v���ɂȂ����J�[�h�Ƃ����łȂ��J�[�h�̔���
	int outsideCard[DECKCARD];	//AI�Ŏg�p�A���łɌ��J����Ă���A�����Ă���ȂǂŐ������O�̃J�[�h���܂Ƃ߂�
	int cardNum;		//���݂̃J�[�h�̑�����
}Player;

//�v���g�^�C�v�錾�A�Q�[���ʂł̊֐��Q
void Initialize();
void InitDeck();
void InitGame();
void SettingGame();
void ShowMenu();
void CheckNum();
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
void ShowResult(int);
int GetCardNum(int);
//�V�X�e���ʂł̊֐��Q
void ShowBoard();
int GetRandomNum(int, int);
void LnTrim(char *);
//�t�@�C���ʂł̊֐��Q
void MakeFile();
void WriteBoard();
#endif // ALGO_GAME_H
