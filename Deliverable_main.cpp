#include "Deliverable_data.hpp"
#include "DxLib.h"

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

	Player player;
	int playerGraph, playerW, playerH;
	playerGraph = player.plGraph();
	GetGraphSize(playerGraph, &playerW, &playerH);

	Enemy enemy;
	int enemyGraph, enemyW, enemyH;
	int enemyMuki = 1;
	enemyGraph = enemy.enGraph();
	GetGraphSize(enemyGraph, &enemyW, &enemyH);

	//敵のショットパターンナンバー
	//敵のショットタイプの初期化
	int shotMode = 0;

	Shot shot;

	shot.shotInit();

	// ループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		{ // デバッグをしやすくするためのコード
			HWND hWnd = GetMainWindowHandle();
			SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
		}

		// 画面を初期化する
		ClearDrawScreen();
		{
			// キー入力取得
			OldKey = Key;
			Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			player.playerKey(Key);

			shot.playerShot1();

			// ショットボタンを押していたらショットを出す
			// 一つ前のループでショットボタンを押していたらショットは出さない
			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				shot.playerShot2();
			}
		}

		//弾の処理
		shot.enemyShooting(playerW, playerH);

		//敵の描画
		{
			shot.enemyShred(shotMode);
			{
				enemy.enemyDraw(enemyMuki);
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
				shot.enemyStruct();

				++shotMode; // Effective C++
				if (shotMode == 5) shotMode = 0;
			}
		}


		// プレイヤーを描画する
		player.playerDraw();

		shot.shooting(enemyW, enemyH);

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();
	}
	//DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;					// ソフトの終了
}
