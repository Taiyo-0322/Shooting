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

	Enemy enemy;

	//�G�̃V���b�g�p�^�[���i���o�[
	//�G�̃V���b�g�^�C�v�̏�����
	int shotMode = 0;
	Shot shot;

	shot.shotInit();

	// ���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
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
		shot.enemyShooting();

		//�G�̕`��
		{
			shot.enemyShred(shotMode);
			{
				enemy.enemyDrow();
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
				enemy.enemyStruct();

				++shotMode; // Effective C++
				if (shotMode == 5) shotMode = 0;
			}
		}


		// �v���C���[��`�悷��
		player.playerDrow();

		shot.shooting();

		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();
	}
	//DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}
