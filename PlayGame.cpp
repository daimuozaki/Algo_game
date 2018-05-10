#include "PlayGame.h"

extern Player player[PLAYER_NUM];

int deck[DECKCARD];
int deckTop;

bool gameSet;
FILE *file;

extern int turnPlayer;
extern int AgainstPlayer;

/*ゲームの初期化
　再戦時に毎回呼び出すのでいっそ別関数でいいんじゃねと思った
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
/*ファイル作成
　日時で初期化できるようになりました
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
		printf("ファイルの作成に失敗しました\n");
		fopen("test.txt", "a");
	}
}
/*山札のシャッフル
　ダブりなしで乱数を生成することで疑似シャッフルとして利用
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
/*ゲームをプレイする
　手札の初期化を行ったのちゲームを始める、要は中継点
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
/*開始時のカードを4枚引く処理
　i,2共にint型なので切り捨てになり、deck[0,1]->player[].card[0]...となる
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
/*カードを1枚引く処理
　引いてきたカードの数を戻り値として飛ばしている
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
				printf("黒の%d番を引きました\n", drawCard / 2);
			else 
				printf("白の%d番を引きました\n", drawCard / 2);
		}
	}
	else {
		printf("山札がないため、カードを引けませんでした\n");
		drawCard = -1;
	}
	//if (player[turnPlayer].playerKind != HUMAN)printf("%d\n", drawCard);
	return drawCard;
}
/*カードの並び替えを行う関数
　仮引数で受け取った数値から引いてきたカードが何枚目に入ったかを割り出すのもここで行う
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
/*アタック処理…の中継点
　プレイヤーとAIで処理が分かれるためこのような形になった
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
		printf("プレイヤー%dが設定されていません\n", turnPlayer + 1);
		break;
	}
}
/*ターンプレイヤーが人間だった場合の処理
　アタックしたいカード番号を指定し、数値を入力する
 　正誤判定は別関数にぶん投げています
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
		printf("アタックをしたいカードの番号を入力してください\n");
		do {
			fgets(UserNum, sizeof(UserNum), stdin);
			LnTrim(UserNum);
			getNum = strtol(UserNum, &endp, 10);
			if (getNum >= player[AgainstPlayer].cardNum) {
				printf("その番号のカードはありません\n");
				legalSelect = false;
			}
			else if (player[AgainstPlayer].clearCard[getNum] == CLEAR) {
				printf("その番号のカードはすでにオープンされています\n");
				legalSelect = false;
			}
			else if (getNum >= 0) {
				printf("%d番のカードを指定しました\n", getNum);
				legalSelect = true;
			}
			else {
				printf("もう一度やり直してください\n");
				legalSelect = false;
			}
		} while (!legalSelect);
		legalSelect = false;
		printf("%d番のカードの数字を入力してください\n", getNum);
		do {
			fgets(UserNum, sizeof(UserNum), stdin);
			LnTrim(UserNum);
			ansNum = strtol(UserNum, &endp, 10);
			if ((getNum >= 0) && (getNum <= 11)) {
				Hitting = JudgeNum(ansNum, getNum);
				legalSelect = true;
			}
			else printf("もう一度やり直してください\n");
		} while (!legalSelect);
		legalSelect = false;
		if (Hitting) {				//正解だった場合連続してアタックを行えるのでその処理を挟む必要あり
			printf("正解です\n");
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			if (CheckClear(AgainstPlayer) < player[AgainstPlayer].cardNum) {
				printf("もう一度アタックしますか？\n");
				printf("1),はい\n");
				printf("2),いいえ\n");
				do {
					fgets(UserNum, sizeof(UserNum), stdin);
					LnTrim(UserNum);
					switch (*UserNum)
					{
					case '1':
						printf("もう一度アタックを行います\n");
						attackContinue = true;
						legalSelect = true;
						break;
					case '2':
						printf("アタックを行いません\n");
						legalSelect = true;
						break;
					default:
						printf("もう一度やり直してください\n");
						break;
					}
				} while (!legalSelect);
			}
		}
		else {
			printf("不正解です\n");
			player[turnPlayer].clearCard[player[turnPlayer].getCard] = CLEAR;
		}
	} while (attackContinue);
}
/*ゲームが終わったかを確認するだけの関数
　カード数以上で判定してるけど同値でもよかったかも
 */
void CheckGameSet() {
	if (CheckClear(AgainstPlayer) >= player[AgainstPlayer].cardNum)
		gameSet = true;
}
/*結果を確認して確定させる関数
　負けたほうはすべての札が公開されている＝公開枚数が多いほうが負け
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
/*プレイヤー交代
　プレイヤー1:0、プレイヤー2:1なので
 　1-0=1、1-1=0として交代ができる
  */
void ChangePlayer() {
	AgainstPlayer = turnPlayer;
	turnPlayer = 1 - turnPlayer;
}
/*盤面表示
　説明することある？
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
					printf(" 黒　");
				else if (JudgeColor(player[i].card[j]) == WHITE) 
					printf(" 白　");
			}
			else {
				if (JudgeColor(player[i].card[j]) == BLACK)
					printf("*");
				else
					printf(" ");

				if (player[i].card[j] >= 20) 
					printf("%d　", GetCardNum(player[i].card[j]));
				else
					printf(" %d　", GetCardNum(player[i].card[j]));
			}
		}
		printf("\n\n");
	}
	printf("Enterキーを押してください...\n");
	c = _fgetchar();
	fflush(stdin);
}
/*ファイルへの書き込み
　AIが参加している場合も考え別関数として記述
 */
void WriteBoard() {
	int i, j;
	if (!gameSet)
		fprintf(file, "プレイヤー%dのターンです\n", turnPlayer + 1);
	for (i = 0; i < PLAYER_NUM; i++) {
		fprintf(file, "プレイヤー%d:\n", i + 1);
		for (j = 0; j < player[i].cardNum; j++) {
			if (j >= 10) fprintf(file, " %d  ", j);
			else fprintf(file, "  %d  ", j);
		}
		fprintf(file, "\n");
		for (j = 0; j < player[i].cardNum; j++) {
			if (player[i].clearCard[j] == COVERED) {
				if (JudgeColor(player[i].card[j]) == BLACK) fprintf(file, " 黒  ");
				else fprintf(file, " 白  ");
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