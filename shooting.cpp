//�V���[�e�B���O�Q�[��
#include "DxLib.h"
#include <math.h>

// �V���b�g�̍ő吔
#define MAX_SHOT	4

//�G�l�~�[�V���b�g
#define MAX_ENEMYSHOT 2000

//��
#define PI 3.151962 //�~����

//�G�V���b�g�̃f�[�^�\���̌^
typedef struct tagENEMYSHOT
{
	int x, y;
	int sx, sy;
	int Size;
	int ValidFlag;
} ENEMYSHOT;

//�G�̃f�[�^�\���̌^
typedef struct tagENEMY
{
	int x, y;
	int Counter;
	int Counter2;
	double Angle;
}ENEMY;

//�G�̃V���b�g�̒ǉ��֐�
int ShotAdd(int x, int y, int Size, double Angle, double Spend);

int ShotType5(void);
int ShotType4(void);
int ShotType3(void);
int ShotType2(void);
int ShotType1(void);

//�G�̃f�[�^
ENEMY Enemy;

// �G�̃V���b�g�f�[�^
ENEMYSHOT EnemyShot[MAX_ENEMYSHOT];

//�G�̃V���b�g�̐�
int EnemyShotNum;

// �G�̃V���b�g�p�^�[���i���o�[
int ShotMode;

// �G�̃V���b�g�����֐��̃|�C���^�z��
int (*EnemyShred[])(void) =
{
	ShotType1,
	ShotType2,
	ShotType3,
	ShotType4,
	ShotType5,
};

int PlayerX, PlayerY, PlayerGraph;	// �v���C���[�̈ʒu

int EnemyX, EnemyY, EnemyMuki, EnemyGraph;
int ShotGraph;
int PlayerW, PlayerH, EnemyW, EnemyH, ShotW, ShotH, EnemyShotW, EnemyShotH;
int ShotValid[MAX_SHOT];	// �V���b�g�����݂��邩�A�t���O
int ShotX[MAX_SHOT], ShotY[MAX_SHOT];	// �V���b�g�̈ʒu

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	int Key = 0;
	int OldKey;	// �O�̃L�[���͏��
	int i, j;

	SetGraphMode(5000, 3500, 16);
	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	// �`����ʂ𗠉�ʂɃZ�b�g
	SetDrawScreen(DX_SCREEN_BACK);

	// �e�̃O���t�B�b�N���������Ƀ��[�h
	ShotGraph = LoadGraph("Hinotama_BULE.png");

	// �e�̐��̏�����
	EnemyShotNum = 0;

	// �G�̃f�[�^�̏�����
	Enemy.x = 2500;
	Enemy.y = 80;
	Enemy.Counter = 0;
	Enemy.Counter2 = 0;
	Enemy.Angle = 0.0;

	// �v���C���[�̏����ʒu���Z�b�g
	PlayerGraph = LoadGraph("Dabi.png");
	PlayerX = 2500;
	PlayerY = 2000;

	// �l�p�N�̃O���t�B�b�N���������Ƀ��[�h���\�����W���Z�b�g
	EnemyGraph = LoadGraph("Enzi.png");
	EnemyX = Enemy.x;
	EnemyY = Enemy.y;

	// �l�p�N�̃O���t�B�b�N�̃T�C�Y�𓾂�
	GetGraphSize(PlayerGraph, &PlayerW, &PlayerH);
	GetGraphSize(EnemyGraph, &EnemyW, &EnemyH);

	//�G�̃V���b�g�^�C�v�̏�����
	ShotMode = 0;

	// �V���b�g�̑��݂�����������
	for (i = 0; i < MAX_SHOT; i++)
		ShotValid[i] = 0;

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

			// �V���b�g�̈ړ�����
			for (j = 0; j < MAX_SHOT; j++)
			{
				// �V���b�g�f�[�^��������������X�L�b�v
				if (ShotValid[j] == 0) continue;

				// �ʒu����ɂ��炷
				ShotY[j] -= 80;

				// ��ʊO�ɏo�Ă�����V���b�g�f�[�^�𖳌��ɂ���
				if (ShotY[j] < -32) ShotValid[j] = 0;
			}

			// �V���b�g�{�^���������Ă�����V���b�g���o��
			// ��O�̃��[�v�ŃV���b�g�{�^���������Ă�����V���b�g�͏o���Ȃ�
			if ((Key & ~OldKey) & PAD_INPUT_A)
			{
				// �g���Ă��Ȃ��V���b�g�f�[�^��T��
				for (j = 0; j < MAX_SHOT; j++)
				{
					if (ShotValid[j] == 0) break;
				}

				// �����g���Ă��Ȃ��V���b�g�f�[�^����������V���b�g���o��
				if (j != MAX_SHOT)
				{
					// �V���b�g�̈ʒu��ݒ�
					ShotX[j] = PlayerX + 260;
					ShotY[j] = PlayerY;

					// �V���b�g�f�[�^���g�p���ɃZ�b�g
					ShotValid[j] = 1;
				}
			}
		}

		// �e�̏���
		{
			int i, Con, Num;

			// �e�̐������ړ��������J��Ԃ�
			Con = 0;
			Num = EnemyShotNum;
			for (i = 0; i < MAX_ENEMYSHOT; i++)
			{
				// �e�̃f�[�^���L���ȏꍇ�͏���
				if (EnemyShot[i].ValidFlag == TRUE)
				{
					// �ړ�����
					EnemyShot[i].x += EnemyShot[i].sx;
					EnemyShot[i].y += EnemyShot[i].sy;

					// ��ʊO�ɏo����e������������
					if (((EnemyShot[i].x) < -2000) ||
						((EnemyShot[i].x) > 6600) ||
						((EnemyShot[i].y) < -2000) ||
						((EnemyShot[i].y) > 5000))
					{
						// �f�[�^�̗L���t���O��|��
						EnemyShot[i].ValidFlag = FALSE;

						// �e�̐������炷
						EnemyShotNum--;
					}

					// �e�̕`��
					{
						int x, y;

						x = EnemyShot[i].x;
						y = EnemyShot[i].y;

						DrawCircle(x, y+300, 50, GetColor(255, 0, 0), TRUE);
						// �e�̃O���t�B�b�N�̃T�C�Y������
						GetGraphSize(DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE), &EnemyShotW, &EnemyShotH);
						// Player�Ƃ̓����蔻��
						if (((EnemyShot[i].x > PlayerX && EnemyShot[i].x < PlayerX + PlayerW) ||
							(PlayerX > EnemyShot[i].x && PlayerX < EnemyShot[i].x + EnemyShotW)) &&
							((EnemyShot[i].y > PlayerY && EnemyShot[i].y < PlayerY + PlayerH) ||
								(PlayerY > EnemyShot[i].y && PlayerY < EnemyShot[i].y + EnemyShotH)))
						{
							// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
							EnemyShot[i].ValidFlag = FALSE;;
						}
					}

					// ���������e�̐����C���N�������g
					Con++;

					// ���������e�̐����A���݂��Ă���e�̐��Ɠ����ɂȂ����ꍇ�̓��[�v�𔲂���
					if (Num == Con) break;
				}
			}
		}

		// �G�̏���
		{
			// �G�̃V���b�g����
			EnemyShred[ShotMode]();
			EnemyMuki = 1;

			// �G�̕`��
			{
				int x, y;

				x = Enemy.x;
				y = Enemy.y;

				if (EnemyMuki == 1) x += 3;
				if (EnemyMuki == 0) x -= 3;

				// �l�p�N����ʉE�[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
				if (x > 4000)
				{
					x = 4000;
					EnemyMuki = 0;
				}

				// �l�p�N����ʍ��[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
				if (x < 0)
				{
					x = 0;
					EnemyMuki = 1;
				}

				LoadGraphScreen(x-350, y, "Enzi.png", TRUE);
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
				Enemy.Counter2 = 0;
				Enemy.Counter = 0;
				Enemy.Angle = 0.0;

				ShotMode++;
				if (ShotMode == 5) ShotMode = 0;
			}
		}


		// �v���C���[��`�悷��
		//DrawBox( PlayerX , PlayerY ,PlayerX + 48 , PlayerY + 48 , GetColor( 255 , 0 , 0 ) , TRUE ) ;
		LoadGraphScreen(PlayerX-350, PlayerY, "Dabi.png", TRUE);

		// �V���b�g��`�悷��
		for (j = 0; j < MAX_SHOT; j++)
		{
			// �V���b�g�f�[�^���L���Ȏ��̂ݕ`��
			if (ShotValid[j] == 1)
			{
				DrawBox(ShotX[j] - 350, ShotY[j], ShotX[j] - 250, ShotY[j] + 160,
					GetColor(0, 0, 255), TRUE);
				// �e�̃O���t�B�b�N�̃T�C�Y������
				GetGraphSize(DrawBox(ShotX[j] - 350, ShotY[j], ShotX[j] - 250, ShotY[j] + 160,
					GetColor(0, 0, 255), TRUE), &ShotW, &ShotH);
				// �l�p�N�Ƃ̓����蔻��
				if (((ShotX[j] > EnemyX && ShotX[j] < EnemyX + EnemyW) ||
					(EnemyX > ShotX[j] && EnemyX < ShotX[j] + ShotW)) &&
					((ShotY[j] > EnemyY && ShotY[j] < EnemyY + EnemyH) ||
						(EnemyY > ShotY[j] && EnemyY < ShotY[j] + ShotH)))
				{
					// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
					ShotValid[j] = 0;
				}
			}

		}
		// ����ʂ̓��e��\��ʂɔ��f������
		ScreenFlip();
	}
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;					// �\�t�g�̏I��
}

// �V���b�g�̒ǉ��֐�
int ShotAdd(int x, int y, int Size, double Angle, double Speed)
{
	int i;

	// �g���Ă��Ȃ��V���b�g������
	for (i = 0; i < MAX_ENEMYSHOT; i++)
		if (EnemyShot[i].ValidFlag == FALSE) break;
	if (i == MAX_ENEMYSHOT) return -1;

	// �V�����V���b�g�̃f�[�^��������
	{
		// ���W�Z�b�g
		EnemyShot[i].x = x;
		EnemyShot[i].y = y;

		// �T�C�Y�Z�b�g
		EnemyShot[i].Size = Size;

		// ���ōs�������ƃX�s�[�h����A�w�������ւ̈ړ����x�Ƃx�������ւ̈ړ����x�𓾂�
		EnemyShot[i].sx = (int)(cos(Angle) * Speed);
		EnemyShot[i].sy = (int)(sin(Angle) * Speed);

		// �V���b�g�f�[�^�̗L���t���O�𗧂Ă�
		EnemyShot[i].ValidFlag = TRUE;

		// �V���b�g�̐��𑝂₷
		EnemyShotNum++;
	}

	// �I��
	return 0;
}


// ��]�V���b�g
int ShotType5(void)
{
	int i;

	// �J�E���^���Q�ɂȂ�����V���b�g�������s��
	if (Enemy.Counter == 2)
	{
		// �V���b�g�̕�����ύX
		Enemy.Angle += 0.2;

		// ���x�̈Ⴄ�܂̃V���b�g�𔭎�
		for (i = 0; i < 5; i++)
		{
			ShotAdd(Enemy.x, Enemy.y, 6, Enemy.Angle, 30 + (i * 8000));
		}

		// �J�E���^������������
		Enemy.Counter = 0;
	}
	else
	{
		Enemy.Counter++;
	}

	// �I��
	return 0;
}


// �΂�T���V���b�g
int ShotType4(void)
{
	// �J�E���^�Q�̒l�ɂ���ď����𕪊�
	switch (Enemy.Counter2)
	{
	case 0:
		// �҂�����

		// �J�E���^���C���N�������g
		Enemy.Counter++;

		// �J�E���^���U�O�ɂȂ�����J�E���^�Q�̒l�𑝂₵�āA�V���b�g�����Ɉڂ�
		if (Enemy.Counter > 60)
		{
			Enemy.Counter2++;
			Enemy.Counter = 0;

			// ���̂Ƃ��̎��@�ւ̕�����ۑ�
			Enemy.Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));
		}
		break;

	case 1:
		//�V���b�g����

		// �J�E���^���Q�̔{���̎��̂݃V���b�g����
		if (Enemy.Counter % 2 == 0)
		{
			double Angle;

			// ��΂������ɂԂ������
			Angle = Enemy.Angle + (PI / 3600 * (GetRand(800) - 400));

			// �V���b�g
			ShotAdd(Enemy.x, Enemy.y, 5, Angle, 30);
		}

		// �J�E���^���C���N�������g�A�J�E���g���T�O�ɂȂ�����҂��ɖ߂�
		Enemy.Counter++;
		if (Enemy.Counter == 50)
		{
			Enemy.Counter2 = 0;
			Enemy.Counter = 0;
		}
		break;
	}

	// �I��
	return 0;
}


// ��_�蒆���ԍ��V���b�g
int ShotType3(void)
{
	// �J�E���^�Q�̒l�ɂ���ď����𕪊�
	switch (Enemy.Counter2)
	{
	case 0:
		// �҂�����

		// �J�E���^���C���N�������g
		Enemy.Counter++;

		// �J�E���^���U�O�ɂȂ�����J�E���^�Q�̒l�𑝂₵�āA�V���b�g�����Ɉڂ�
		if (Enemy.Counter > 60)
		{
			Enemy.Counter2++;
			Enemy.Counter = 0;

			// ���̂Ƃ��̎��@�ւ̕�����ۑ�
			Enemy.Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));
		}
		break;

	case 1:
		// �V���b�g����

		// �J�E���^���T�̔{���̎��̂݃V���b�g����
		if (Enemy.Counter % 5 == 0)
		{
			ShotAdd(Enemy.x, Enemy.y, 4, Enemy.Angle, 30 + Enemy.Counter / 15);
		}

		// �J�E���^���C���N�������g�A�J�E���g���T�O�ɂȂ�����҂��ɖ߂�
		Enemy.Counter++;
		if (Enemy.Counter == 50)
		{
			Enemy.Counter2 = 0;
			Enemy.Counter = 0;
		}
		break;
	}

	// �I��
	return 0;
}


// �S�����V���b�g�̊֐�
int ShotType2(void)
{
	int i;

	// �J�E���^���P�O�ɂȂ�����V���b�g����
	if (Enemy.Counter == 10)
	{
		double Angle, d;

		// �G���猩�����@�ւ̕������擾
		Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));

		// ���C�h�V���b�g�����A�������p�x��ς��Ĉ�����V���b�g
		d = Angle - PI;
		for (i = 0; i < 20; i++)
		{
			ShotAdd(Enemy.x, Enemy.y, 4, d, 30);
			d += 2 * PI / 20;
		}

		// �J�E���^��������
		Enemy.Counter = 0;
	}
	else
	{
		// �J�E���^���C���N�������g
		Enemy.Counter++;
	}

	// �I��
	return 0;
}


// �R�v�`�x�V���b�g�̊֐�
int ShotType1(void)
{
	int i;

	// �J�E���^���S�O�ɂȂ�����V���b�g����
	if (Enemy.Counter == 40)
	{
		double Angle, d;

		// �G���猩�����@�ւ̕������擾
		Angle = atan2((double)(PlayerY - Enemy.y), (double)(PlayerX - Enemy.x));

		// �R�v�`�x�V���b�g����
		d = Angle - (PI / 3) / 2;
		for (i = 0; i < 3; i++)
		{
			ShotAdd(Enemy.x, Enemy.y, 4, d, 30);
			d += (PI / 3) / 2;
		}

		// �J�E���^��������
		Enemy.Counter = 0;
	}
	else
	{
		// �J�E���^���C���N�������g
		Enemy.Counter++;
	}

	// �I��
	return 0;
}
