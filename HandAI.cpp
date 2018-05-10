#include "HandAI.h"

Player player[PLAYER_NUM];

int turnPlayer;
int AgainstPlayer;

/*ランダムAIに手を入力させる関数
　指定カードこそランダムだが当てる数値はカードの前後の値を参照してそこから乱数
  もちろん非公開なら参照できないようにするつもり
*/
void GetRandomAIHand() {
	int getNum = -1;
	int ansNum = -1;
	int minNum = 24;	//最大値の白11の内部値が23なので内部値が24以上のカードは存在しない
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
		printf("AIプレイヤー%dは%d番のカードを指定しました\n", turnPlayer + 1, getNum);
		legalSelect = false;
		for (i = getNum; i > -1; i--) {
			if ((player[AgainstPlayer].clearCard[i] == CLEAR) && (minNum > player[AgainstPlayer].card[i]))
				minNum = player[AgainstPlayer].card[i];
		}
		if (minNum == 24)
			minNum = getNum;
		printf("%d～", minNum);
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
			printf("AIは%dを宣言し、正解しました\n", ansNum);
			player[AgainstPlayer].clearCard[getNum] = CLEAR;
			if ((CheckClear(AgainstPlayer) < player[AgainstPlayer].cardNum) && (GetRandomNum(0, 1)))
				attackContinue = true;
		}
		else {
			printf("AIは%dを宣言し、不正解でした\n", ansNum);
			player[turnPlayer].clearCard[player[turnPlayer].getCard] = CLEAR;
		}
	} while (attackContinue);
}
/*実験に使うAIのベースとなるAIの関数。
　ランダムAIをベースに2択以下になったカード、白黒の判別機能などを追加予定
　あとここからoutsideCardも本格的に使っていきたいなぁ
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
/*数字当てで正解不正解を判定する関数
　一番上で言ってる通り黒0：0、白0：1...として処理しているため数値の修正を挟む必要があったためこうなった
 　ここでは入力された数値をnとして説明を行うこととする
  */
bool JudgeNum(int anum, int gnum) {
	int cnum;
	if (JudgeColor(player[AgainstPlayer].card[gnum]) == BLACK) cnum = anum * 2;	//黒の場合、内部的には必ず偶数の値をとるため2nとして計算できる(0:0,1:2,2:4...11:22)
	else cnum = anum * 2 + 1;										//同様に、白の場合は2n+1として計算する(0:1,1:3,2:5...11:23)

	//計算後の値を用いて数値の一致不一致を判定する
	if (cnum == player[AgainstPlayer].card[gnum]) return true;
	else return false;
}
/*内部値をカード番号に変換するだけの関数
　制作理由は謎
 */
int GetCardNum(int num) {
	return num / 2;			//要するに2で割るだけ(小数点以下切り捨て)である
}
/*色の判定
　2で割った余りで黒か白かを判定している
 */
int JudgeColor(int card) {
	if (card % 2 == 0) return BLACK;
	else return WHITE;
}
/*公開されたカードの枚数を判定する関数
　仮引数のplはプレイヤー番号で、公開されているカード枚数を戻り値として返す
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
/*乱数生成器
　いつものやつ
 */
int GetRandomNum(int min, int max) {
	return rand() % (max - min + 1) + min;
}
/*ストリーム上に残ったEnterの後始末用関数
　Enterを示す文字列を終端を示すものに変更するだけ
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