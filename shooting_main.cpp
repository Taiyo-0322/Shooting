//シューティングゲーム
#include "DxLib.h"
#include "shooting_data.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;	// 前のキー入力状態

	SetGraphMode(5000, 3500, 16);
	/*if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}*/
	DxLibInitializer dxlibInit;

	// 描画先画面を裏画面にセット
	SetDrawScreen(DX_SCREEN_BACK);

	int PlayerX, PlayerY, PlayerW, PlayerH;
	Player player;
	player.PlayerSize(PlayerX, PlayerY, PlayerW, PlayerH);

	int EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle, EnemyW, EnemyH;
	Enemy enemy;
	enemy.EnemyStruct(EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle);
	enemy.EnemySize(EnemyW, EnemyH);

	//敵のショットパターンナンバー
	//敵のショットタイプの初期化
	int ShotMode = 0;
	Shot shot;

	shot.ShotInit();

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

			shot.PlayerShot1();

			// ショットボタンを押していたらショットを出す
			// 一つ前のループでショットボタンを押していたらショットは出さない
			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				shot.PlayerShot2(PlayerX, PlayerY);
			}
		}

		//弾の処理
		shot.EnemyShooting(PlayerX, PlayerY, PlayerW, PlayerH);

		//敵の描画
		{
			shot.EnemyShred(ShotMode, PlayerX, PlayerY, EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle);
			{
				enemy.EnemyDrow();
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
				EnemyCounter2 = 0;
				EnemyCounter = 0;
				EnemyAngle = 0.0;

				++ShotMode; // Effective C++
				if (ShotMode == 5) ShotMode = 0;
			}
		}


		// プレイヤーを描画する
		player.PlayerDrow();

		shot.Shooting(EnemyX, EnemyY, EnemyW, EnemyH);
		
		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}
	//DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}

