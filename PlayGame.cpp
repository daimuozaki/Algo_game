#include "PlayGame.h"

extern Player player[PLAYER_NUM];

int deck[DECKCARD];
int deckTop;

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
	for (i = 0; i < PLAYER_NUM; i++) {
		player[i].cardNum = 0;
		for (j = 0; j < MAX_CARD; j++) {
			player[i].card[j] = -1;
			player[i].clearCard[j] = COVERED;
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
	sprintf(fileName, "%s.txt", date);
	printf("%s", fileName);
	file = fopen(fileName, "w");
	if (file == NULL) {
		printf("�t�@�C���̍쐬�Ɏ��s���܂���\n");
		fopen("test.txt", "a");
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
		if (player[turnPlayer].playerKind == HUMAN)
			ShowBoard();
		WriteBoard();
		Attack();
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
			if (JudgeColor(drawCard) == 0) 
				printf("����%d�Ԃ������܂���\n", drawCard / 2);
			else 
				printf("����%d�Ԃ������܂���\n", drawCard / 2);
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
	default:
		printf("�v���C���[%d���ݒ肳��Ă��܂���\n", turnPlayer + 1);
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
		legalSelect = false;
		if (Hitting) {				//�����������ꍇ�A�����ăA�^�b�N���s����̂ł��̏��������ޕK�v����
			printf("�����ł�\n");
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			if (CheckClear(AgainstPlayer) < player[AgainstPlayer].cardNum) {
				printf("������x�A�^�b�N���܂����H\n");
				printf("1),�͂�\n");
				printf("2),������\n");
				do {
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
�@�v���C���[1:0�A�v���C���[2:1�Ȃ̂�
 �@1-0=1�A1-1=0�Ƃ��Č�オ�ł���
  */
void ChangePlayer() {
	AgainstPlayer = turnPlayer;
	turnPlayer = 1 - turnPlayer;
}
/*�Ֆʕ\��
�@�������邱�Ƃ���H
 */
void ShowBoard() {
	int i, j;
	char c;
	for (i = 0; i < PLAYER_NUM; i++) {
		for (j = 0; j < player[i].cardNum; j++) {
			if (j >= 10) 
				printf(" %d  ", j);
			else 
				printf("  %d  ", j);
		}
		printf("\n");
		for (j = 0; j < player[i].cardNum; j++) {
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
		printf("\n\n");
	}
	printf("Enter�L�[�������Ă�������...\n");
	c = _fgetchar();
	fflush(stdin);
}
/*�t�@�C���ւ̏�������
�@AI���Q�����Ă���ꍇ���l���ʊ֐��Ƃ��ċL�q
 */
void WriteBoard() {
	int i, j;
	if (!gameSet)
		fprintf(file, "�v���C���[%d�̃^�[���ł�\n", turnPlayer + 1);
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