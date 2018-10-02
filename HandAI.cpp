#include "HandAI.h"

Player player[PLAYER_NUM];

int turnPlayer;
int AgainstPlayer;

/*�����_��AI�Ɏ����͂�����֐�
  �w��J�[�h���������_���������Ă鐔�l�̓J�[�h�̑O��̒l���Q�Ƃ��Ă������痐��
  ����������J�Ȃ�Q�Ƃł��Ȃ�
*/
void GetRandomAIHand() {
	int getNum = -1;
	int ansNum = -1;
	int minNum = 24;	//�ő�l�̔�11�̓����l��23�Ȃ̂œ����l��24�ȏ�̃J�[�h�͑��݂��Ȃ�
	int maxNum = -1;	//�ŏ��l�̍�0�̓����l��0�Ȃ̂�(ry
	int i;
	bool legalSelect = false;
	bool Hitting = false;
	bool attackContinue;
	do {
		player[turnPlayer].attacks[0]++;
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
			player[turnPlayer].attacks[1]++;
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
	const int cardNum = player[AgainstPlayer].cardNum;
	int getNum = -1;
	int ansNum = -1;
	int *minNum = new int[cardNum];
	int *maxNum = new int[cardNum];
	int *rangeNum = new int[cardNum];
	int i, j;
	bool legalSelect = false;
	bool hitting = false;
	bool attackContinue;
	do {
		attackContinue = false;			//�A���A�^�b�N����̃��Z�b�g
		player[turnPlayer].attacks[0]++;
		for (i = 0; i < cardNum; i++) {	//�͈͔���̃��Z�b�g�A�A���A�^�b�N���ɏ㉺���̔���ɉe�����y�ԉ\�������邽�߂����ɔz�u
			minNum[i] = 24;
			maxNum[i] = -1;
			rangeNum[i] = 0;
		}
		for (i = 0; i < player[turnPlayer].cardNum; i++)			//���O�J�[�h�̐ݒ�A�܂��͎莝���J�[�h�����O
			player[turnPlayer].outsideCard[i] = player[turnPlayer].card[i];
		for (i = 0; i < cardNum; i++) {								//�����đ���J�[�h�̏��O�A����͌��J����Ă���J�[�h�݂̂����O����
			if (player[AgainstPlayer].clearCard[i] == CLEAR) {
				for (j = 0; j < DECKCARD; j++) {
					if ((player[turnPlayer].outsideCard[j] == -1) || (player[turnPlayer].outsideCard[j] == player[AgainstPlayer].card[i])) {
						player[turnPlayer].outsideCard[j] = player[AgainstPlayer].card[i];
						break;
					}
				}
			}
		}
		i = 0;
		//while (true) {
		//	if ((player[turnPlayer].outsideCard[i] == -1)||(i>DECKCARD))
		//		break;
		//	else
		//		printf("%d:%d\n", i + 1, player[turnPlayer].outsideCard[i]);
		//	i++;
		//}
		for (i = 0; i < cardNum; i++) {								//�J�[�h���Ƃ͈̔͒����A
			minNum[i] = i;
			maxNum[i] = DECKCARD - cardNum + i;
			if (JudgeColor(player[AgainstPlayer].card[0]) == WHITE)minNum[i]++;
			if (JudgeColor(player[AgainstPlayer].card[cardNum - 1]) == BLACK)maxNum[i]--;
			for (j = 0; j < i; j++)
				if (player[AgainstPlayer].clearCard[j] == CLEAR) minNum[i] = player[AgainstPlayer].card[j] - (j - i);
			for (j = cardNum - 1; j > i; j--)
				if (player[AgainstPlayer].clearCard[j] == CLEAR)maxNum[i] = player[AgainstPlayer].card[j] - (j - i);
			printf("%d:%d�`%d\n", i, minNum[i], maxNum[i]);
		}
		do {		//�A�^�b�N�J�[�h���w��A�Ƃ肠���������_���ŏo���Ă���2���J�[�h�����邩���肷��
			getNum = GetRandomNum(0, cardNum - 1);
			if ((player[AgainstPlayer].clearCard[getNum] == COVERED) && (maxNum[getNum] >= minNum[getNum]))
				legalSelect = true;
			else
				legalSelect = false;
		} while (!legalSelect);
		for (i = 0; i < cardNum; i++) {
			rangeNum[i] = maxNum[i] - minNum[i];
			j = 0;
			while (player[turnPlayer].outsideCard[j] != -1) {
				if ((player[turnPlayer].outsideCard[j] >= minNum[i]) && (player[turnPlayer].outsideCard[j] <= maxNum[i]))
					rangeNum[i]--;
				j++;
			}
			if ((rangeNum[i] <= 3) && (rangeNum[i] >= 0) && (player[AgainstPlayer].clearCard[i] == COVERED))
				getNum = i;
		}
		printf("AI�v���C���[%d��%d�Ԃ̃J�[�h���w�肵�܂���\n", turnPlayer + 1, getNum);
		legalSelect = false;
		do {
			ansNum = GetRandomNum(minNum[getNum], maxNum[getNum]);
			if ((ansNum >= 0) && (ansNum <= 23)) {
				if (JudgeColor(ansNum) == JudgeColor(player[AgainstPlayer].card[getNum])) {
					for (i = 0; i < DECKCARD; i++) {
						if (ansNum == player[turnPlayer].outsideCard[i]) {
							legalSelect = false;
							break;
						}
						else if (player[turnPlayer].outsideCard[i] == -1) {
							legalSelect = true;
							break;
						}
					}
				}
			}
		} while (!legalSelect);
		ansNum = GetCardNum(ansNum);
		hitting = JudgeNum(ansNum, getNum);
		if (hitting) {
			printf("AI��%d��錾���A�������܂���\n", ansNum);
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			player[turnPlayer].attacks[1]++;
			//if ((CheckClear(AgainstPlayer) >= player[AgainstPlayer].cardNum) && (GetRandomNum(0, 1)))
			//	attackContinue = true;
		}
		else {
			printf("AI��%d��錾���A�s�����ł���\n", ansNum);
			player[turnPlayer].clearCard[player[turnPlayer].getCard] = CLEAR;
		}
	} while (attackContinue);
	delete[] minNum;
	delete[] maxNum;
	delete[] rangeNum;
}
/**/
void GetSideAIHand() {
	const int cardNum = player[AgainstPlayer].cardNum;
	int getNum = -1;
	int ansNum = -1;
	int *minNum = new int[cardNum];
	int *maxNum = new int[cardNum];
	int *rangeNum = new int[cardNum];
	int *selectNum = new int[cardNum];
	int i, j;
	bool legalSelect = false;
	bool hitting = false;
	bool attackContinue;
	do {
		attackContinue = false;			//�A���A�^�b�N����̃��Z�b�g
		player[turnPlayer].attacks[0]++;
		for (i = 0; i < cardNum; i++) {	//�͈͔���̃��Z�b�g�A�A���A�^�b�N���ɏ㉺���̔���ɉe�����y�ԉ\�������邽�߂����ɔz�u
			minNum[i] = 24;
			maxNum[i] = -1;
			rangeNum[i] = 0;
			if (i % 2 == 0) {
				selectNum[i] = i / 2;
			}
			else {
				selectNum[i] = cardNum - (i / 2 + 1);
			}
		}
		for (i = 0; i < player[turnPlayer].cardNum; i++)			//���O�J�[�h�̐ݒ�A�܂��͎莝���J�[�h�����O
			player[turnPlayer].outsideCard[i] = player[turnPlayer].card[i];
		for (i = 0; i < cardNum; i++) {								//�����đ���J�[�h�̏��O�A����͌��J����Ă���J�[�h�݂̂����O����
			if (player[AgainstPlayer].clearCard[i] == CLEAR) {
				for (j = 0; j < DECKCARD; j++) {
					if ((player[turnPlayer].outsideCard[j] == -1) || (player[turnPlayer].outsideCard[j] == player[AgainstPlayer].card[i])) {
						player[turnPlayer].outsideCard[j] = player[AgainstPlayer].card[i];
						break;
					}
				}
			}
		}
		i = 0;
		//while (true) {
		//	if ((player[turnPlayer].outsideCard[i] == -1)||(i>DECKCARD))
		//		break;
		//	else
		//		printf("%d:%d\n", i + 1, player[turnPlayer].outsideCard[i]);
		//	i++;
		//}
		for (i = 0; i < cardNum; i++) {								//�J�[�h���Ƃ͈̔͒����A
			minNum[i] = i;
			maxNum[i] = DECKCARD - cardNum + i;
			if (JudgeColor(player[AgainstPlayer].card[0]) == WHITE)minNum[i]++;
			if (JudgeColor(player[AgainstPlayer].card[cardNum - 1]) == BLACK)maxNum[i]--;
			for (j = 0; j < i; j++)
				if (player[AgainstPlayer].clearCard[j] == CLEAR) minNum[i] = player[AgainstPlayer].card[j] - (j - i);
			for (j = cardNum - 1; j > i; j--)
				if (player[AgainstPlayer].clearCard[j] == CLEAR)maxNum[i] = player[AgainstPlayer].card[j] - (j - i);
			printf("%d:%d�`%d\n", i, minNum[i], maxNum[i]);
		}
		for (i = 0; i < cardNum; i++) {
			if (player[AgainstPlayer].clearCard[selectNum[i]] == COVERED) {
				getNum = selectNum[i];
				break;
			}
		}
		for (i = 0; i < cardNum; i++) {
			rangeNum[i] = maxNum[i] - minNum[i];
			j = 0;
			while (player[turnPlayer].outsideCard[j] != -1) {
				if ((player[turnPlayer].outsideCard[j] >= minNum[i]) && (player[turnPlayer].outsideCard[j] <= maxNum[i]))
					rangeNum[i]--;
				j++;
			}
			if ((rangeNum[i] <= 3) && (rangeNum[i] >= 0) && (player[AgainstPlayer].clearCard[i] == COVERED))
				getNum = i;
		}
		printf("AI�v���C���[%d��%d�Ԃ̃J�[�h���w�肵�܂���\n", turnPlayer + 1, getNum);
		legalSelect = false;
		do {
			ansNum = GetRandomNum(minNum[getNum], maxNum[getNum]);
			if ((ansNum >= 0) && (ansNum <= 23)) {
				if (JudgeColor(ansNum) == JudgeColor(player[AgainstPlayer].card[getNum])) {
					for (i = 0; i < DECKCARD; i++) {
						if (ansNum == player[turnPlayer].outsideCard[i]) {
							legalSelect = false;
							break;
						}
						else if (player[turnPlayer].outsideCard[i] == -1) {
							legalSelect = true;
							break;
						}
					}
				}
			}
		} while (!legalSelect);
		ansNum = GetCardNum(ansNum);
		hitting = JudgeNum(ansNum, getNum);
		if (hitting) {
			printf("AI��%d��錾���A�������܂���\n", ansNum);
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			player[turnPlayer].attacks[1]++;
			//if ((CheckClear(AgainstPlayer) >= player[AgainstPlayer].cardNum) && (GetRandomNum(0, 1)))
			//	attackContinue = true;
		}
		else {
			printf("AI��%d��錾���A�s�����ł���\n", ansNum);
			player[turnPlayer].clearCard[player[turnPlayer].getCard] = CLEAR;
		}
	} while (attackContinue);
	delete[] minNum;
	delete[] maxNum;
	delete[] rangeNum;
	delete[] selectNum;
}
/*�������ĂŐ���s�����𔻒肷��֐�
�@Algo_game.cpp�Ō����Ă�ʂ荕0�F0�A��0�F1...�Ƃ��ď������Ă��邽�ߐ��l�̏C�������ޕK�v�����������߂����Ȃ���
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