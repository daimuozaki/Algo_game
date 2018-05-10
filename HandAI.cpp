#include "HandAI.h"

Player player[PLAYER_NUM];

int turnPlayer;
int AgainstPlayer;

/*�����_��AI�Ɏ����͂�����֐�
�@�w��J�[�h���������_���������Ă鐔�l�̓J�[�h�̑O��̒l���Q�Ƃ��Ă������痐��
  ����������J�Ȃ�Q�Ƃł��Ȃ��悤�ɂ������
*/
void GetRandomAIHand() {
	int getNum = -1;
	int ansNum = -1;
	int minNum = 24;	//�ő�l�̔�11�̓����l��23�Ȃ̂œ����l��24�ȏ�̃J�[�h�͑��݂��Ȃ�
	int maxNum = -1;
	int i;
	bool legalSelect = false;
	bool Hitting = false;
	bool attackContinue;
	do {
		attackContinue = false;
		do {
			getNum = GetRandomNum(0, player[AgainstPlayer].cardNum - 1);
			if (player[AgainstPlayer].clearCard[getNum] == COVERED)
				legalSelect = true;
			else
				legalSelect = false;
		} while (!legalSelect);
		printf("AI�v���C���[%d��%d�Ԃ̃J�[�h���w�肵�܂���\n", turnPlayer + 1, getNum);
		legalSelect = false;
		for (i = getNum; i > -1; i--) {
			if ((player[AgainstPlayer].clearCard[i] == CLEAR) && (minNum > player[AgainstPlayer].card[i]))
				minNum = player[AgainstPlayer].card[i];
		}
		if (minNum == 24)
			minNum = getNum;
		printf("%d�`", minNum);
		for (i = getNum; i < player[AgainstPlayer].cardNum; i++) {
			if ((player[AgainstPlayer].clearCard[i] == CLEAR) && (maxNum < player[AgainstPlayer].card[i]))
				maxNum = player[AgainstPlayer].card[i];
		}
		if (maxNum == -1)
			maxNum = 23 - (player[AgainstPlayer].cardNum - getNum);
		printf("%d\n", maxNum);
		do {
			ansNum = GetRandomNum(minNum, maxNum);
			ansNum = GetCardNum(ansNum);
			if ((ansNum >= 0) && (ansNum <= 11)) {
				Hitting = JudgeNum(ansNum, getNum);
				legalSelect = true;
			}
		} while (!legalSelect);
		if (Hitting) {
			printf("AI��%d��錾���A�������܂���\n", ansNum);
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			if ((CheckClear(AgainstPlayer) < player[AgainstPlayer].cardNum) && (GetRandomNum(0, 1)))
				attackContinue = true;
		}
		else {
			printf("AI��%d��錾���A�s�����ł���\n", ansNum);
			player[turnPlayer].clearCard[player[turnPlayer].getCard] = CLEAR;
		}
	} while (attackContinue);
}
/*�����Ɏg��AI�̃x�[�X�ƂȂ�AI�̊֐��B
�@�����_��AI���x�[�X��2���ȉ��ɂȂ����J�[�h�A�����̔��ʋ@�\�Ȃǂ�ǉ��\��
�@���Ƃ�������outsideCard���{�i�I�Ɏg���Ă��������Ȃ�
*/
void GetBaseAIHand() {
	int getNum = -1;
	int ansNum = -1;
	int minNum = 24;
	int maxNum = -1;
	int i;
	bool legalSelect = false;
	bool hitting = false;
	bool attackContinue;
	do {
		attackContinue = false;
		
	} while (attackContinue);
}
/*�������ĂŐ���s�����𔻒肷��֐�
�@��ԏ�Ō����Ă�ʂ荕0�F0�A��0�F1...�Ƃ��ď������Ă��邽�ߐ��l�̏C�������ޕK�v�����������߂����Ȃ���
 �@�����ł͓��͂��ꂽ���l��n�Ƃ��Đ������s�����ƂƂ���
  */
bool JudgeNum(int anum, int gnum) {
	int cnum;
	if (JudgeColor(player[AgainstPlayer].card[gnum]) == BLACK) cnum = anum * 2;	//���̏ꍇ�A�����I�ɂ͕K�������̒l���Ƃ邽��2n�Ƃ��Čv�Z�ł���(0:0,1:2,2:4...11:22)
	else cnum = anum * 2 + 1;										//���l�ɁA���̏ꍇ��2n+1�Ƃ��Čv�Z����(0:1,1:3,2:5...11:23)

	//�v�Z��̒l��p���Đ��l�̈�v�s��v�𔻒肷��
	if (cnum == player[AgainstPlayer].card[gnum]) return true;
	else return false;
}
/*�����l���J�[�h�ԍ��ɕϊ����邾���̊֐�
�@���엝�R�͓�
 */
int GetCardNum(int num) {
	return num / 2;			//�v�����2�Ŋ��邾��(�����_�ȉ��؂�̂�)�ł���
}
/*�F�̔���
�@2�Ŋ������]��ō��������𔻒肵�Ă���
 */
int JudgeColor(int card) {
	if (card % 2 == 0) return BLACK;
	else return WHITE;
}
/*���J���ꂽ�J�[�h�̖����𔻒肷��֐�
�@��������pl�̓v���C���[�ԍ��ŁA���J����Ă���J�[�h������߂�l�Ƃ��ĕԂ�
*/
int CheckClear(int pl) {
	int i;
	int clearNum = 0;
	for (i = 0; i < player[pl].cardNum; i++) {
		if (player[pl].clearCard[i] == CLEAR)
			clearNum++;
	}
	return clearNum;
}
/*����������
�@�����̂��
 */
int GetRandomNum(int min, int max) {
	return rand() % (max - min + 1) + min;
}
/*�X�g���[����Ɏc����Enter�̌�n���p�֐�
�@Enter��������������I�[���������̂ɕύX���邾��
 */
void LnTrim(char *str) {
	int i = 0;
	while (true) {
		if (str[i] == '\n') {
			str[i] = '\0';
			break;
		}
		i++;
	}
}