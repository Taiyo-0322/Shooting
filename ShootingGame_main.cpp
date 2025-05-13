#include "ShootingGame_data.hpp"
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;	// �O�̃L�[���͏��
	int TimeCount;
	int nTime = 1;

	SetGraphMode(5000, 3500, 16);
	/*if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}*/
	DxLibInitializer dxlibInit;

	TimeCount = GetNowCount();

	Title title;

	title.fontHandle();
	title.drawHandle();

	//�L�[���͑҂�
	WaitKey();

	title.deleteHandle();

	// �`����ʂ𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	Shot shot;

	//Player player;
	int playerGraph, playerW, playerH;
	playerGraph = shot.plGraph();
	GetGraphSize(playerGraph, &playerW, &playerH);

	//Enemy enemy;
	int enemyGraph, enemyW, enemyH;
	int enemyMuki = 1;
	enemyGraph = shot.enGraph();
	GetGraphSize(enemyGraph, &enemyW, &enemyH);

	//�G�̃V���b�g�p�^�[���i���o�[
	//�G�̃V���b�g�^�C�v�̏�����
	int shotMode = 0;

	shot.shotInit();

	// ���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		{ // �f�o�b�O�����₷�����邽�߂̃R�[�h
			HWND hWnd = GetMainWindowHandle();
			SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, (SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW));
		}

		// ��ʂ�����������
		ClearDrawScreen();
		{
			// �L�[���͎擾
			OldKey = Key;
			Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			shot.playerShot1(Key);

			// �V���b�g�{�^���������Ă�����V���b�g���o��
			// ��O�̃��[�v�ŃV���b�g�{�^���������Ă�����V���b�g�͏o���Ȃ�
			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				shot.playerShot2();
			}
		}
		
		//�e�̏���
		shot.enemyShooting(playerW, playerH);

		//�G�̕`��
		{
			shot.enemyShred(shotMode);
			{
				shot.enemyDraw_(enemyMuki);
			}
		}

		// �G�̃V���b�g�̐؂�ւ�����
		if(nTime * 5000 < GetNowCount() - TimeCount)
		{
			shot.enemyStruct();

			++shotMode; // Effective C++
			++nTime;
			if (shotMode == 5)
			{
				shotMode = 0;
			}
		}

		shot.shooting(enemyW, enemyH);

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();

		if (60000 < GetNowCount() - TimeCount)
		{
			break;
		}
	}
	//DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}
