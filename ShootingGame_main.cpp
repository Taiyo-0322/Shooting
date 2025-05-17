#include "ShootingGame_data.hpp"
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;
	int TimeCount;
	int nTime = 1;

	SetGraphMode(5000, 3500, 16);
	DxLibInitializer dxlibInit;

	TimeCount = GetNowCount();

	Title title;

	title.fontHandle();
	title.drawHandle();

	WaitKey();

	title.deleteHandle();

	SetDrawScreen(DX_SCREEN_BACK);

	Shot shot;

	int playerGraph, playerW, playerH;
	playerGraph = shot.plGraph();
	GetGraphSize(playerGraph, &playerW, &playerH);

	int enemyGraph, enemyW, enemyH;
	int enemyMuki = 1;
	enemyGraph = shot.enGraph();
	GetGraphSize(enemyGraph, &enemyW, &enemyH);

	int shotMode = 0;

	shot.shotInit();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		{
			HWND hWnd = GetMainWindowHandle();
			SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
		}

		ClearDrawScreen();
		{
			OldKey = Key;
			Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			shot.playerShot1(Key);

			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				shot.playerShot2();
			}
		}
		
		shot.enemyShooting(playerW, playerH);

		{
			shot.enemyShred(shotMode);
			{
				shot.enemyDraw_(enemyMuki);
			}
		}

		if(nTime * 5000 < GetNowCount() - TimeCount)
		{
			shot.enemyStruct();

			++shotMode;
			++nTime;
			if (shotMode == 5)
			{
				shotMode = 0;
			}
		}

		shot.shooting(enemyW, enemyH);

		ScreenFlip();

		if (60000 < GetNowCount() - TimeCount)
		{
			break;
		}
	}

	return 0;					
}
