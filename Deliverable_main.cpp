#include "Deliverable_data.hpp"
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;	// �O�̃L�[���͏��

	SetGraphMode(5000, 3500, 16);
	/*if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}*/
	DxLibInitializer dxlibInit;

	// �`����ʂ𗠉�ʂɃZ�b�g
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

	//�G�̃V���b�g�p�^�[���i���o�[
	//�G�̃V���b�g�^�C�v�̏�����
	int shotMode = 0;

	Shot shot(Player * player, Enemy * enemy);

	shot(Player * player, Enemy * enemy).shotInit();

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

			player.playerKey(Key);

			shot.playerShot1();

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
				enemy.enemyDraw(enemyMuki);
			}
		}

		// �G�̃V���b�g�̐؂�ւ�����
		{
			char C;

			// ���͂��ꂽ�������擾
			C = GetInputChar(TRUE);

			// �b�L�[�������ꂽ��G�̃V���b�g���[�h��ύX����
			if (C == 'C' || C == 'c')
			{
				shot.enemyStruct();

				++shotMode; // Effective C++
				if (shotMode == 5) shotMode = 0;
			}
		}


		// �v���C���[��`�悷��
		player.playerDraw();

		shot.shooting(enemyW, enemyH);

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();
	}
	//DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}
