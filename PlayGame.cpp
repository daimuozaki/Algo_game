#include "PlayGame.h"

extern Player player[PLAYER_NUM];

int deck[DECKCARD];
int deckTop;

int turnNum;

bool gameSet;
FILE *file;

extern int turnPlayer;
extern int AgainstPlayer;

/*�Q�[���̏�����
�@�Đ펞�ɖ���Ăяo���̂ł������ʊ֐��ł����񂶂�˂Ǝv����
*/
void InitGame() {
	int i, j;
	gameSet = false;
	turnNum = 0;
	for (i = 0; i < PLAYER_NUM; i++) {
		player[i].cardNum = 0;
		for (j = 0; j < MAX_CARD; j++) {
			player[i].card[j] = -1;
			player[i].clearCard[j] = COVERED;
			player[i].toldCard[j] = -1;
		}
		for (j = 0; j < DECKCARD; j++) {
			player[i].outsideCard[j] = -1;
		}
	}
	turnPlayer = PLAYER_1;
	AgainstPlayer = PLAYER_2;
	deckTop = 0;
	InitDeck();
	MakeFile();
}
/*�t�@�C���쐬
�@�����ŏ������ł���悤�ɂȂ�܂���
*/
void MakeFile() {
	time_t t = time(NULL);
	char date[64];
	char fileName[100];
	strftime(date, sizeof(date), "%Y%m%d-%H%M%S", localtime(&t));
	sprintf(fileName, "gameLog\\%s.txt", date);
	printf("%s\n", fileName);
	file = fopen(fileName, "a");
	if (file == NULL) {
		printf("�t�@�C���̍쐬�Ɏ��s���܂���\n");
		fopen("gameLog\\test.txt", "a");
	}
}
/*�R�D�̃V���b�t��
�@�_�u��Ȃ��ŗ����𐶐����邱�Ƃŋ^���V���b�t���Ƃ��ė��p
*/
void InitDeck() {
	int i, j;
	int dblNum;
	i = 0;
	while (i < DECKCARD) {
		dblNum = 0;
		deck[i] = GetRandomNum(0, DECKCARD - 1);
		for (j = 0; j < i; j++) {
			if (deck[j] == deck[i]) dblNum++;
		}
		if (dblNum < 1) i++;
	}
}
/*�Q�[�����v���C����
�@��D�̏��������s�����̂��Q�[�����n�߂�A�v�͒��p�_
*/
int PlayGame() {
	int drawCard;
	InitGame();
	StartGame();
	while (!gameSet)
	{
		drawCard = DrawCard();
		SoatCard(drawCard);
		WriteBoard();
		if (player[turnPlayer].playerKind == HUMAN) {
			ShowBoard(NORMAL_MODE);
			GameMenu();
		}
		else {
			Attack();
		}
		CheckGameSet();
		ChangePlayer();
	}
	return CheckResult();
}
/*�J�n���̃J�[�h��4����������
�@i,2����int�^�Ȃ̂Ő؂�̂ĂɂȂ�Adeck[0,1]->player[].card[0]...�ƂȂ�
*/
void StartGame() {
	int i;
	for (i = 0; i < 8; i++) {
		if (i % 2 == 0) {
			player[0].card[i / 2] = deck[deckTop];
			player[0].outsideCard[i / 2] = deck[deckTop];
			player[0].cardNum++;
		}
		else {
			player[1].card[i / 2] = deck[deckTop];
			player[1].outsideCard[i / 2] = deck[deckTop];
			player[1].cardNum++;
		}
		deckTop++;
	}
}
/*�J�[�h��1����������
�@�����Ă����J�[�h�̐���߂�l�Ƃ��Ĕ�΂��Ă���
*/
int DrawCard() {
	int num = player[turnPlayer].cardNum;
	int drawCard;
	if (deckTop < DECKCARD) {
		player[turnPlayer].card[num] = deck[deckTop];
		drawCard = deck[deckTop];
		player[turnPlayer].cardNum++;
		deckTop++;
		if (player[turnPlayer].playerKind == HUMAN) {
			if (JudgeColor(drawCard) == BLACK) 
				printf("����%d�Ԃ������܂���\n", GetCardNum(drawCard));
			else 
				printf("����%d�Ԃ������܂���\n", GetCardNum(drawCard));
		}
	}
	else {
		printf("�R�D���Ȃ����߁A�J�[�h�������܂���ł���\n");
		drawCard = -1;
	}
	//if (player[turnPlayer].playerKind != HUMAN)printf("%d\n", drawCard);
	return drawCard;
}
/*�J�[�h�̕��ёւ����s���֐�
�@�������Ŏ󂯎�������l��������Ă����J�[�h�������ڂɓ�������������o���̂������ōs��
*/
void SoatCard(int dc) {
	int i, j, k;
	int numbox, clearbox;
	for (i = 0; i < PLAYER_NUM; i++) {
		for (j = 0; j < player[i].cardNum - 1; j++) {
			for (k = player[i].cardNum - 1; k > j; k--) {
				if (player[i].card[k - 1] > player[i].card[k]) {
					numbox = player[i].card[k - 1];
					clearbox = player[i].clearCard[k - 1];
					player[i].card[k - 1] = player[i].card[k];
					player[i].clearCard[k - 1] = player[i].clearCard[k];
					player[i].card[k] = numbox;
					player[i].clearCard[k] = clearbox;
				}
			}
		}
	}
	for (i = 0; i < player[turnPlayer].cardNum; i++) {
		if (dc == player[turnPlayer].card[i])
			player[turnPlayer].getCard = i;
	}
}
/*�Q�[�����̃��j���[�\��
�@�Ֆʂ̊m�F�ƃA�^�b�N�錾���s����
*/
void GameMenu() {
	char userSelect[LEN];
	bool legalSelect = false;
	do {
		printf("1).�J�[�h�ւ̃A�^�b�N\n");
		printf("2).�Ֆʂ̊m�F\n");
		fgets(userSelect, sizeof(userSelect), stdin);
		LnTrim(userSelect);
		fflush(stdin);
		switch (*userSelect) {
		case '1':
			legalSelect = true;
			Attack();
			break;
		case '2':
			ShowBoard(SHOW_MODE);
			break;
		default:
			legalSelect = false;
			printf("������x��蒼���Ă�������\n");
			break;
		}
	} while (!legalSelect);
}
/*�A�^�b�N�����c�̒��p�_
�@�v���C���[��AI�ŏ�����������邽�߂��̂悤�Ȍ`�ɂȂ���
*/
void Attack() {
	switch (player[turnPlayer].playerKind) {
	case HUMAN:
		GetHumanHand();
		break;
	case RAND_AI:
		GetRandomAIHand();
		break;
	case BASE_AI:
		GetBaseAIHand();
		break;
	case SIDE_AI:
		GetSideAIHand();
		break;
	default:
		printf("�v���C���[%d���ݒ肳��Ă��܂���\n", turnPlayer + 1);	//�����f�����珉��������肭�����Ă��Ȃ����ɂȂ�
		break;
	}
}
/*�^�[���v���C���[���l�Ԃ������ꍇ�̏���
�@�A�^�b�N�������J�[�h�ԍ����w�肵�A���l����͂���
�@���딻��͕ʊ֐��ɂԂ񓊂��Ă��܂�
*/
void GetHumanHand() {
	int getNum = -1;
	int ansNum = -1;
	char UserNum[32];
	char *endp;
	bool legalSelect = false;
	bool Hitting = false;
	bool attackContinue;
	do {
		player[turnPlayer].attacks[ATTACK_NUM]++;
		attackContinue = false;
		printf("�A�^�b�N���������J�[�h�̔ԍ�����͂��Ă�������\n");
		do {
			fgets(UserNum, sizeof(UserNum), stdin);
			LnTrim(UserNum);
			getNum = strtol(UserNum, &endp, 10);
			if (getNum >= player[AgainstPlayer].cardNum) {
				printf("���̔ԍ��̃J�[�h�͂���܂���\n");
				legalSelect = false;
			}
			else if (player[AgainstPlayer].clearCard[getNum] == CLEAR) {
				printf("���̔ԍ��̃J�[�h�͂��łɃI�[�v������Ă��܂�\n");
				legalSelect = false;
			}
			else if (getNum >= 0) {
				printf("%d�Ԃ̃J�[�h���w�肵�܂���\n", getNum);
				legalSelect = true;
			}
			else {
				printf("������x��蒼���Ă�������\n");
				legalSelect = false;
			}
		} while (!legalSelect);
		legalSelect = false;
		printf("%d�Ԃ̃J�[�h�̐�������͂��Ă�������\n", getNum);
		do {
			fgets(UserNum, sizeof(UserNum), stdin);
			LnTrim(UserNum);
			ansNum = strtol(UserNum, &endp, 10);
			if ((getNum >= 0) && (getNum <= 11)) {
				Hitting = JudgeNum(ansNum, getNum);
				legalSelect = true;
			}
			else printf("������x��蒼���Ă�������\n");
		} while (!legalSelect);
		if (JudgeColor(player[AgainstPlayer].card[getNum]) == BLACK) ansNum *= 2;
		else ansNum = ansNum * 2 + 1;
		player[AgainstPlayer].toldCard[player[turnPlayer].attacks[ATTACK_NUM]] = ansNum;
		legalSelect = false;
		if (Hitting) {				//�����������ꍇ�A�����ăA�^�b�N���s����̂ł��̏��������ޕK�v����
			printf("�����ł�\n");
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			player[turnPlayer].attacks[SUCCESS_NUM]++;
			if (CheckClear(AgainstPlayer) < player[AgainstPlayer].cardNum) {
				do {
					printf("������x�A�^�b�N���܂����H\n");
					printf("1),�͂�\n");
					printf("2),������\n");
					printf("3),�Ֆʂ��m�F����\n");
					fgets(UserNum, sizeof(UserNum), stdin);
					LnTrim(UserNum);
					switch (*UserNum)
					{
					case '1':
						printf("������x�A�^�b�N���s���܂�\n");
						attackContinue = true;
						legalSelect = true;
						break;
					case '2':
						printf("�A�^�b�N���s���܂���\n");
						legalSelect = true;
						break;
					case '3':
						ShowBoard(SHOW_MODE);
						break;
					default:
						printf("������x��蒼���Ă�������\n");
						break;
					}
				} while (!legalSelect);
			}
		}
		else {
			printf("�s�����ł�\n");
			player[turnPlayer].clearCard[player[turnPlayer].getCard] = CLEAR;
		}
	} while (attackContinue);
}
/*�Q�[�����I����������m�F���邾���̊֐�
�@�J�[�h���ȏ�Ŕ��肵�Ă邯�Ǔ��l�ł��悩��������
*/
void CheckGameSet() {
	printf("%d,%d\n", CheckClear(AgainstPlayer), player[AgainstPlayer].cardNum);
	if (CheckClear(AgainstPlayer) >= player[AgainstPlayer].cardNum)
		gameSet = true;
}
/*���ʂ��m�F���Ċm�肳����֐�
�@�������ق��͂��ׂĂ̎D�����J����Ă��遁���J�����������ق�������
*/
int CheckResult() {
	int i;
	int clearNum[2];
	for (i = 0; i < PLAYER_NUM; i++) {
		clearNum[i] = CheckClear(i);
	}
	if (clearNum[0] < clearNum[1])
		return PLAYER_1;
	else if (clearNum[0] > clearNum[1])
		return PLAYER_2;
	else
		return -1;
}
/*�v���C���[���
  turnNum/2�̗]��Ń^�[���v���C���[�����߂�
*/
void ChangePlayer() {
	turnNum++;
	turnPlayer = turnNum % PLAYER_NUM;
	AgainstPlayer = (turnNum + 1) % PLAYER_NUM;
}
/*�t�@�C���ւ̏�������
�@AI���Q�����Ă���ꍇ���l���ʊ֐��Ƃ��ċL�q
*/
void WriteBoard() {
	int i, j;
	if (!gameSet) {
		fprintf(file, "�^�[��%d\n", turnNum + 1);
		fprintf(file, "�v���C���[%d�̃^�[���ł�\n", turnPlayer + 1);
	}
	for (i = 0; i < PLAYER_NUM; i++) {
		fprintf(file, "�v���C���[%d:\n", i + 1);
		for (j = 0; j < player[i].cardNum; j++) {
			if (j >= 10) fprintf(file, " %d  ", j);
			else fprintf(file, "  %d  ", j);
		}
		fprintf(file, "\n");
		for (j = 0; j < player[i].cardNum; j++) {
			if (player[i].clearCard[j] == COVERED) {
				if (JudgeColor(player[i].card[j]) == BLACK) fprintf(file, " ��  ");
				else fprintf(file, " ��  ");
			}
			else {
				if (JudgeColor(player[i].card[j]) == BLACK) fprintf(file, "*");
				else fprintf(file, " ");

				if (player[i].card[j] >= 20)fprintf(file, "%d  ", GetCardNum(player[i].card[j]));
				else fprintf(file, " %d  ", GetCardNum(player[i].card[j]));
			}
		}
		fprintf(file, "\n\n");
	}
}
/*�Ֆʂ�\������֐�
�@��������mode���g�����ƂŎ����̃J�[�h�����ׂČ��J���ĕ\�����郂�[�h��
�@�J�[�h�𕚂���ꂽ��Ԃŕ\�����郂�[�h��2��ނ̕\���@���g����
*/
void ShowBoard(int mode) {
	int i, j;
	for (i = 0; i < PLAYER_NUM; i++) {
		for (j = 0; j < player[i].cardNum; j++) {
			if (j >= 10)
				printf(" %d  ", j);
			else
				printf("  %d  ", j);
		}
		printf("\n");
		for (j = 0; j < player[i].cardNum; j++) {
			if (mode == NORMAL_MODE) {
				if ((player[i].clearCard[j] == COVERED)) {
					if (JudgeColor(player[i].card[j]) == BLACK)
						printf(" ���@");
					else if (JudgeColor(player[i].card[j]) == WHITE)
						printf(" ���@");
				}
				else {
					if (JudgeColor(player[i].card[j]) == BLACK)
						printf("*");
					else
						printf(" ");

					if (player[i].card[j] >= 20)
						printf("%d�@", GetCardNum(player[i].card[j]));
					else
						printf(" %d�@", GetCardNum(player[i].card[j]));
				}
			}
			else if (mode == SHOW_MODE) {
				if ((player[i].clearCard[j] == COVERED) && (i != turnPlayer)) {
					if (JudgeColor(player[i].card[j]) == BLACK)
						printf(" ���@");
					else if (JudgeColor(player[i].card[j]) == WHITE)
						printf(" ���@");
				}
				else {
					if (JudgeColor(player[i].card[j]) == BLACK)
						printf("*");
					else
						printf(" ");

					if (player[i].card[j] >= 20)
						printf("%d�@", GetCardNum(player[i].card[j]));
					else
						printf(" %d�@", GetCardNum(player[i].card[j]));
				}
			}
		}
		printf("\n\n");
	}
}