// Algo_game.cpp : アプリケーションのエントリ ポイントを定義します。
//

/*
　とりあえず組んでるアルゴのプログラム。
　黒0を0として23が白11となるように番号は振り分ける。
　ちなみにアタック時に入力された数をnとして黒:2n、白:2n+1で正誤判定を行えるようになるというメリットがある(ex.黒6:12->黒なので2*6=12)
　反対に、内部での処理値からゲーム上でのカードの値に変換するなら2で割るだけで行ける(ex.白5:11->11/2=5.5,小数点以下切り捨てで5)
　人間対人間はコンソールの都合上試合が全く機能しないため実質デバッグツール
　記録とかメモとかはLog.txtに引っ越し
  P.S.
  なんか一部のキーの反応が鈍い気がするけど気のせいかな
*/

#include"Algo_game.h"

extern int deck[DECKCARD];
extern Player player[PLAYER_NUM];

int gameNum;

extern FILE *file;

bool exitGame;

int main()
{
	int i, Result;
	mkdir("gameLog");
	while (true) {
		Initialize();
		ShowMenu();
		if (exitGame)
			break;
		for (i = 0; i < gameNum; i++) {
			Result = PlayGame();
			ShowResult(Result);
			fclose(file);
		}
		WriteFinalResult();
	}
	system("pause");
	return 0;
}
/*初期化
　exitGameと乱数のシード値をリセット、ゲームリセットは別関数(別ファイル)でやってます
*/
void Initialize() {
	int i, j;
	exitGame = false;
	srand((unsigned)time(NULL));
	for (i = 0; i < PLAYER_NUM; i++) {
		player[i].WinNum = 0;
		for (j = 0; j < sizeof(player[i].attacks) / sizeof(player[i].attacks[0]); j++)
			player[i].attacks[j] = 0;
	}
}
/*メインメニュー
　3はデバッグ用のコマンドです
*/
void ShowMenu() {
	char userSelect[LEN];
	bool legalSelect = false;
	printf("algo ゲーム\n");
	printf("1):はじめる\n");
	printf("2):おえる\n");
	do {
		fgets(userSelect, sizeof(userSelect), stdin);
		LnTrim(userSelect);
		fflush(stdin);
		switch (*userSelect) {
		case '1':
			SettingGame();
			legalSelect = true;
			break;
		case '2':
			exitGame = true;
			legalSelect = true;
			break;
		//case '3':
		//	CheckNum();
		//	break;
		default:
			printf("もう一度やり直してください\n");
			break;
		}
	} while (!legalSelect);

}
/*結果表示
　結果を仮引数で受け取ってswitch文で処理
*/
void ShowResult(int wp) {
	char c;
	switch (wp) {
	case PLAYER_1:
		printf("プレイヤー1の勝ちです\n");
		fprintf(file, "プレイヤー1の勝ちです\n");
		player[wp].WinNum++;
		break;
	case PLAYER_2:
		printf("プレイヤー2の勝ちです\n");
		fprintf(file, "プレイヤー2の勝ちです\n");
		player[wp].WinNum++;
		break;
	default:
		printf("引き分け\n");
		fprintf(file, "引き分け\n");
		break;
	}
	ShowBoard(NORMAL_MODE);
	WriteBoard();
	system("pause");
}
/*ゲームの準備
　プレイヤーの種類とゲーム回数(どちらかが人間の場合は1回)を設定する
*/
void SettingGame() {
	char userNum[LEN];
	bool legalSelect = false;
	printf("プレイヤー1は人間とAIどちらで参加しますか?\n");
	do {
		printf("1).人間\n");
		printf("2).ランダムAI\n");
		printf("3).基本AI\n");
		printf("4).端からAI\n");
		fgets(userNum, sizeof(userNum), stdin);
		LnTrim(userNum);
		fflush(stdin);
		switch (*userNum)
		{
		case '1':
			printf("プレイヤー1は人間で始めます\n");
			player[PLAYER_1].playerKind = HUMAN;
			legalSelect = true;
			break;
		case '2':
			printf("プレイヤー1はランダムAIで始めます\n");
			player[PLAYER_1].playerKind = RAND_AI;
			legalSelect = true;
			break;
		case '3':
			printf("プレイヤー1は基本AIで始めます\n");
			player[PLAYER_1].playerKind = BASE_AI;
			legalSelect = true;
			break;
		case '4':
			printf("プレイヤー1は端からAIで始めます\n");
			player[PLAYER_1].playerKind = SIDE_AI;
			legalSelect = true;
			break;
		default:
			printf("もう一度やり直してください\n");
			break;
		}
	} while (!legalSelect);
	legalSelect = false;
	printf("プレイヤー2は人間とAIどちらで参加しますか?\n");
	do {
		printf("1).人間\n");
		printf("2).ランダムAI\n");
		printf("3).基本AI\n");
		printf("4).端からAI\n");
		fgets(userNum, sizeof(userNum), stdin);
		LnTrim(userNum);
		fflush(stdin);
		switch (*userNum)
		{
		case '1':
			printf("プレイヤー2は人間で始めます\n");
			player[PLAYER_2].playerKind = HUMAN;
			legalSelect = true;
			break;
		case '2':
			printf("プレイヤー2はランダムAIで始めます\n");
			player[PLAYER_2].playerKind = RAND_AI;
			legalSelect = true;
			break;
		case '3':
			printf("プレイヤー2は基本AIで始めます\n");
			player[PLAYER_2].playerKind = BASE_AI;
			legalSelect = true;
			break;
		case '4':
			printf("プレイヤー2は端からAIで始めます\n");
			player[PLAYER_2].playerKind = SIDE_AI;
			legalSelect = true;
			break;
		default:
			printf("もう一度やり直してください\n");
			break;
		}
	} while (!legalSelect);
	legalSelect = false;
	if ((player[PLAYER_1].playerKind != HUMAN) && (player[1].playerKind != HUMAN)) {
		printf("ゲーム回数を決定してください\n");
		do {
			fgets(userNum, sizeof(userNum), stdin);
			LnTrim(userNum);
			gameNum = atoi(userNum);
			if (gameNum >= 1) {
				legalSelect = true;
				printf("ゲーム回数:%d回\n", gameNum);
			}
			else printf("もう一度やり直してください\n");
		} while (!legalSelect);
	}
	else gameNum = 1;
}
/*デバッグ用関数
　デッキの中身と各プレイヤーのカード枚数と数値を内部値で表示している
*/
//void CheckNum() {
//	for (int i = 0; i < DECKCARD; i++) {
//		printf("%d:%d\n", i, deck[i]);
//	}
//	for (int j = 0; j < PLAYER_NUM; j++) {
//		printf("%d\n", player[j].cardNum);
//		for (int k = 0; k < MAX_CARD; k++) {
//			printf("%d,", player[j].card[k]);
//		}
//		printf("\n");
//	}
//}
/*最終結果を書き出す関数
　ないと不便だろうなと思った
*/
void WriteFinalResult() {
	int i;
	MakeFile();
	fprintf(file, "総ゲーム数:%d回\n", gameNum);
	fprintf(file, "プレイヤー1:%d勝-%d勝:プレイヤー2\n", player[PLAYER_1].WinNum, player[PLAYER_2].WinNum);
	for (i = 0; i < PLAYER_NUM; i++) {
		fprintf(file, "プレイヤー%d:\n", i + 1);
		fprintf(file, "総アタック数:%d回\n", player[i].attacks[ATTACK_NUM]);
		fprintf(file, "アタック成功数:%d回\n", player[i].attacks[SUCCESS_NUM]);
	}
	fclose(file);
}