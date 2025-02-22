//シューティングゲーム
#include "DxLib.h"
#include <math.h>

// ショットの最大数
#define MAX_SHOT	4

//エネミーショット
#define MAX_ENEMYSHOT 2000

//π
#define PI 3.151962 //円周率

//敵ショットのデータ構造体型
typedef struct tagENEMYSHOT
{
	int x, y;
	int sx, sy;
	int Size;
	int ValidFlag;
} ENEMYSHOT;

//敵のデータ構造体型
typedef struct tagENEMY
{
	int x, y;
	int Counter;
	int Counter2;
	double Angle;
}ENEMY;

//敵のショットの追加関数
int ShotAdd(int x, int y, int Size, double Angle, double Spend);

int ShotType5(void);
int ShotType4(void);
int ShotType3(void);
int ShotType2(void);
int ShotType1(void);

//敵のデータ
ENEMY Enemy;

// 敵のショットデータ
ENEMYSHOT EnemyShot[MAX_ENEMYSHOT];

//敵のショットの数
int EnemyShotNum;

// 敵のショットパターンナンバー
int ShotMode;

// 敵のショット処理関数のポインタ配列
int (*EnemyShred[])(void) =
{
	ShotType1,
	ShotType2,
	ShotType3,
	ShotType4,
	ShotType5,
};

int PlayerX, PlayerY, PlayerGraph;	// プレイヤーの位置

int EnemyX, EnemyY, EnemyMuki, EnemyGraph;
int ShotGraph;
int PlayerW, PlayerH, EnemyW, EnemyH, ShotW, ShotH, EnemyShotW, EnemyShotH;
int ShotValid[MAX_SHOT];	// ショットが存在するか、フラグ
int ShotX[MAX_SHOT], ShotY[MAX_SHOT];	// ショットの位置

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;	// 前のキー入力状態
	int i, j;

	SetGraphMode(5000, 3500, 16);
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	// 弾のグラフィックをメモリにロード
	ShotGraph = LoadGraph("Hinotama_BULE.png");

	// 弾の数の初期化
	EnemyShotNum = 0;

	// 敵のデータの初期化
	Enemy.x = 2500;
	Enemy.y = 80;
	Enemy.Counter = 0;
	Enemy.Counter2 = 0;
	Enemy.Angle = 0.0;

	// プレイヤーの初期位置をセット
	PlayerGraph = LoadGraph("Dabi.png");
	PlayerX = 2500;
	PlayerY = 2000;

	// 四角君のグラフィックをメモリにロード＆表示座標をセット
	EnemyGraph = LoadGraph("Enzi.png");
	EnemyX = Enemy.x;
	EnemyY = Enemy.y;

	// 四角君のグラフィックのサイズを得る
	GetGraphSize(PlayerGraph, &PlayerW, &PlayerH);
	GetGraphSize(EnemyGraph, &EnemyW, &EnemyH);

	//敵のショットタイプの初期化
	ShotMode = 0;

	// ショットの存在を初期化する
	for (i = 0; i < MAX_SHOT; i++)
		ShotValid[i] = 0;

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// 画面を初期化する
		ClearDrawScreen();

		{
			// キー入力取得
			OldKey = Key;
			Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			if (Key & PAD_INPUT_UP) PlayerY -= 50;
			if (Key & PAD_INPUT_DOWN) PlayerY += 50;
			if (Key & PAD_INPUT_RIGHT) PlayerX += 50; // 右を押していたら右に進む
			if (Key & PAD_INPUT_LEFT) PlayerX -= 50;	// 左を押していたら左に進む

			// ショットの移動処理
			for (j = 0; j < MAX_SHOT; j++)
			{
				// ショットデータが無効だったらスキップ
				if (ShotValid[j] == 0) continue;

				// 位置を上にずらす
				ShotY[j] -= 80;

				// 画面外に出ていたらショットデータを無効にする
				if (ShotY[j] < -32) ShotValid[j] = 0;
			}

			// ショットボタンを押していたらショットを出す
			// 一つ前のループでショットボタンを押していたらショットは出さない
			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				// 使われていないショットデータを探す
				for (j = 0; j < MAX_SHOT; j++)
				{
					if (ShotValid[j] == 0) break;
				}

				// もし使われていないショットデータがあったらショットを出す
				if (j != MAX_SHOT)
				{
					// ショットの位置を設定
					ShotX[j] = PlayerX + 260;
					ShotY[j] = PlayerY;

					// ショットデータを使用中にセット
					ShotValid[j] = 1;
				}
			}
		}

		// 弾の処理
		{
			int i, Con, Num;

			// 弾の数だけ移動処理を繰り返す
			Con = 0;
			Num = EnemyShotNum;
			for (i = 0; i < MAX_ENEMYSHOT; i++)
			{
				// 弾のデータが有効な場合は処理
				if (EnemyShot[i].ValidFlag == TRUE)
				{
					// 移動処理
					EnemyShot[i].x += EnemyShot[i].sx;
					EnemyShot[i].y += EnemyShot[i].sy;

					// 画面外に出たら弾情報を消去する
					if (((EnemyShot[i].x) < -2000) ||
						((EnemyShot[i].x) > 6600) ||
						((EnemyShot[i].y) < -2000) ||
						((EnemyShot[i].y) > 5000))
					{
						// データの有効フラグを倒す
						EnemyShot[i].ValidFlag = FALSE;

						// 弾の数を減らす
						EnemyShotNum--;
					}

					// 弾の描画
					{
						int x, y;

						x = EnemyShot[i].x;
						y = EnemyShot[i].y;

						DrawCircle(x, y+300, 50, GetColor(255, 0, 0), TRUE);
						// 弾のグラフィックのサイズをえる
						GetGraphSize(DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE), &EnemyShotW, &EnemyShotH);
						// Playerとの当たり判定
						if (((EnemyShot[i].x > PlayerX && EnemyShot[i].x < PlayerX + PlayerW) ||
							(PlayerX > EnemyShot[i].x && PlayerX < EnemyShot[i].x + EnemyShotW)) &&
							((EnemyShot[i].y > PlayerY && EnemyShot[i].y < PlayerY + PlayerH) ||
								(PlayerY > EnemyShot[i].y && PlayerY < EnemyShot[i].y + EnemyShotH)))
						{
							// 接触している場合は当たった弾の存在を消す
							EnemyShot[i].ValidFlag = FALSE;;
						}
					}

					// 処理した弾の数をインクリメント
					Con++;

					// 処理した弾の数が、存在している弾の数と同じになった場合はループを抜ける
					if (Num == Con) break;
				}
			}
		}

		// 敵の処理
		{
			// 敵のショット処理
			EnemyShred[ShotMode]();
			EnemyMuki = 1;

			// 敵の描画
			{
				int x, y;

				x = Enemy.x;
				y = Enemy.y;

				if (EnemyMuki == 1) x += 3;
				if (EnemyMuki == 0) x -= 3;

				// 四角君が画面右端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
				if (x > 4000)
				{
					x = 4000;
					EnemyMuki = 0;
				}

				// 四角君が画面左端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
				if (x < 0)
				{
					x = 0;
					EnemyMuki = 1;
				}

				LoadGraphScreen(x-350, y, "Enzi.png", TRUE);
			}
		}

		// 敵のショットの切り替え処理
		{
			char C;

			// 入力された文字を取得
			C = GetInputChar(TRUE);

			// Ｃキーが押されたら敵のショットモードを変更する
			if (C == 'C' || C == 'c')
			{
				Enemy.Counter2 = 0;
				Enemy.Counter = 0;
				Enemy.Angle = 0.0;

				ShotMode++;
				if (ShotMode == 5) ShotMode = 0;
			}
		}


		// プレイヤーを描画する
		//DrawBox( PlayerX , PlayerY ,PlayerX + 48 , PlayerY + 48 , GetColor( 255 , 0 , 0 ) , TRUE ) ;
		LoadGraphScreen(PlayerX-350, PlayerY, "Dabi.png", TRUE);

		// ショットを描画する
		for (j = 0; j < MAX_SHOT; j++)
		{
			// ショットデータが有効な時のみ描画
			if (ShotValid[j] == 1)
			{
				DrawBox(ShotX[j] - 350, ShotY[j], ShotX[j] - 250, ShotY[j] + 160,
					GetColor(0, 0, 255), TRUE);
				// 弾のグラフィックのサイズをえる
				GetGraphSize(DrawBox(ShotX[j] - 350, ShotY[j], ShotX[j] - 250, ShotY[j] + 160,
					GetColor(0, 0, 255), TRUE), &ShotW, &ShotH);
				// 四角君との当たり判定
				if (((ShotX[j] > EnemyX && ShotX[j] < EnemyX + EnemyW) ||
					(EnemyX > ShotX[j] && EnemyX < ShotX[j] + ShotW)) &&
					((ShotY[j] > EnemyY && ShotY[j] < EnemyY + EnemyH) ||
						(EnemyY > ShotY[j] && EnemyY < ShotY[j] + ShotH)))
				{
					// 接触している場合は当たった弾の存在を消す
					ShotValid[j] = 0;
				}
			}

		}
		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}

// ショットの追加関数
int ShotAdd(int x, int y, int Size, double Angle, double Speed)
{
	int i;

	// 使われていないショットを検索
	for (i = 0; i < MAX_ENEMYSHOT; i++)
		if (EnemyShot[i].ValidFlag == FALSE) break;
	if (i == MAX_ENEMYSHOT) return -1;

	// 新しいショットのデータを初期化
	{
		// 座標セット
		EnemyShot[i].x = x;
		EnemyShot[i].y = y;

		// サイズセット
		EnemyShot[i].Size = Size;

		// 飛んで行く方向とスピードから、Ｘ軸方向への移動速度とＹ軸方向への移動速度を得る
		EnemyShot[i].sx = (int)(cos(Angle) * Speed);
		EnemyShot[i].sy = (int)(sin(Angle) * Speed);

		// ショットデータの有効フラグを立てる
		EnemyShot[i].ValidFlag = TRUE;

		// ショットの数を増やす
		EnemyShotNum++;
	}

	// 終了
	return 0;
}


// 回転ショット
int ShotType5(void)
{
	int i;

	// カウンタが２になったらショット処理を行う
	if (Enemy.Counter == 2)
	{
		// ショットの方向を変更
		Enemy.Angle += 0.2;

		// 速度の違う五つのショットを発射
		for (i = 0; i < 5; i++)
		{
			ShotAdd(Enemy.x, Enemy.y, 6, Enemy.Angle, 30 + (i * 8000));
		}

		// カウンタを初期化する
		Enemy.Counter = 0;
	}
	else
	{
		Enemy.Counter++;
	}

	// 終了
	return 0;
}


// ばら撒きショット
int ShotType4(void)
{
	// カウンタ２の値によって処理を分岐
	switch (Enemy.Counter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		Enemy.Counter++;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (Enemy.Counter > 60)
		{
			Enemy.Counter2++;
			Enemy.Counter = 0;

			// このときの自機への方向を保存
			Enemy.Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));
		}
		break;

	case 1:
		//ショット処理

		// カウンタが２の倍数の時のみショットする
		if (Enemy.Counter % 2 == 0)
		{
			double Angle;

			// 飛ばす方向にぶれをつける
			Angle = Enemy.Angle + (PI / 3600 * (GetRand(800) - 400));

			// ショット
			ShotAdd(Enemy.x, Enemy.y, 5, Angle, 30);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
		Enemy.Counter++;
		if (Enemy.Counter == 50)
		{
			Enemy.Counter2 = 0;
			Enemy.Counter = 0;
		}
		break;
	}

	// 終了
	return 0;
}


// 一点手中時間差ショット
int ShotType3(void)
{
	// カウンタ２の値によって処理を分岐
	switch (Enemy.Counter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		Enemy.Counter++;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (Enemy.Counter > 60)
		{
			Enemy.Counter2++;
			Enemy.Counter = 0;

			// このときの自機への方向を保存
			Enemy.Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));
		}
		break;

	case 1:
		// ショット処理

		// カウンタが５の倍数の時のみショットする
		if (Enemy.Counter % 5 == 0)
		{
			ShotAdd(Enemy.x, Enemy.y, 4, Enemy.Angle, 30 + Enemy.Counter / 15);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
		Enemy.Counter++;
		if (Enemy.Counter == 50)
		{
			Enemy.Counter2 = 0;
			Enemy.Counter = 0;
		}
		break;
	}

	// 終了
	return 0;
}


// 全方向ショットの関数
int ShotType2(void)
{
	int i;

	// カウンタが１０になったらショットする
	if (Enemy.Counter == 10)
	{
		double Angle, d;

		// 敵から見た自機への方向を取得
		Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));

		// ワイドショット処理、少しずつ角度を変えて一周分ショット
		d = Angle - PI;
		for (i = 0; i < 20; i++)
		{
			ShotAdd(Enemy.x, Enemy.y, 4, d, 30);
			d += 2 * PI / 20;
		}

		// カウンタを初期化
		Enemy.Counter = 0;
	}
	else
	{
		// カウンタをインクリメント
		Enemy.Counter++;
	}

	// 終了
	return 0;
}


// ３ＷＡＹショットの関数
int ShotType1(void)
{
	int i;

	// カウンタが４０になったらショットする
	if (Enemy.Counter == 40)
	{
		double Angle, d;

		// 敵から見た自機への方向を取得
		Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));

		// ３ＷＡＹショット処理
		d = Angle - (PI / 3) / 2;
		for (i = 0; i < 3; i++)
		{
			ShotAdd(Enemy.x, Enemy.y, 4, d, 30);
			d += (PI / 3) / 2;
		}

		// カウンタを初期化
		Enemy.Counter = 0;
	}
	else
	{
		// カウンタをインクリメント
		Enemy.Counter++;
	}

	// 終了
	return 0;
}
