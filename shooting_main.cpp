//�V���[�e�B���O�Q�[��
#include "DxLib.h"
#include "shooting_data.hpp"

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

	int PlayerX, PlayerY, PlayerW, PlayerH;
	Player player;
	player.PlayerSize(PlayerX, PlayerY, PlayerW, PlayerH);

	int EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle, EnemyW, EnemyH;
	Enemy enemy;
	enemy.EnemyStruct(EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle);
	enemy.EnemySize(EnemyW, EnemyH);

	//�G�̃V���b�g�p�^�[���i���o�[
	//�G�̃V���b�g�^�C�v�̏�����
	int ShotMode = 0;
	Shot shot;

	shot.ShotInit();

	// ���[�v
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		// ��ʂ�����������
		ClearDrawScreen();
		{
			// �L�[���͎擾
			OldKey = Key;
			Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

			if (Key & PAD_INPUT_UP) PlayerY -= 50;
			if (Key & PAD_INPUT_DOWN) PlayerY += 50;
			if (Key & PAD_INPUT_RIGHT) PlayerX += 50; // �E�������Ă�����E�ɐi��
			if (Key & PAD_INPUT_LEFT) PlayerX -= 50;	// ���������Ă����獶�ɐi��

			shot.PlayerShot1();

			// �V���b�g�{�^���������Ă�����V���b�g���o��
			// ��O�̃��[�v�ŃV���b�g�{�^���������Ă�����V���b�g�͏o���Ȃ�
			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				shot.PlayerShot2(PlayerX, PlayerY);
			}
		}

		//�e�̏���
		shot.EnemyShooting(PlayerX, PlayerY, PlayerW, PlayerH);

		//�G�̕`��
		{
			shot.EnemyShred(ShotMode, PlayerX, PlayerY, EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle);
			{
				enemy.EnemyDrow();
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
				EnemyCounter2 = 0;
				EnemyCounter = 0;
				EnemyAngle = 0.0;

				++ShotMode; // Effective C++
				if (ShotMode == 5) ShotMode = 0;
			}
		}


		// �v���C���[��`�悷��
		player.PlayerDrow();

		shot.Shooting(EnemyX, EnemyY, EnemyW, EnemyH);
		
		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();
	}
	//DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}

