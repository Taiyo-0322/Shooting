#include "shooting_data.hpp"


void Player::PlayerSize(int& PlayerX, int& PlayerY, int& PlayerW, int& PlayerH)
{
	PlayerGraph = LoadGraph("Dabi.png");
	PlayerX = 2500;
	PlayerY = 2000;
	GetGraphSize(PlayerGraph, &PlayerW, &PlayerH);
}

void Player::PlayerDrow()
{
	PlayerGraph = LoadGraph("Dabi.png");
	PlayerX = 2500;
	PlayerY = 2000;
	LoadGraphScreen(PlayerX - 350, PlayerY, "Dabi.png", TRUE);
}

void Enemy::EnemyStruct(int& EnemyX, int& EnemyY, int& EnemyCounter, int& EnemyCounter2, int& EnemyAngle)
{
	Enemy.x = 2500;
	Enemy.y = 80;
	Enemy.Counter = 0;
	Enemy.Counter2 = 0;
	Enemy.Angle = 0.0;
}

void Enemy::EnemySize(int& EnemyW, int& EnemyH)
{
	EnemyGraph = LoadGraph("Enzi.png");
	GetGraphSize(EnemyGraph, &EnemyW, &EnemyH);
}

void Enemy::EnemyDrow()
{
	EnemyMuki = 1;
	Enemy.x = 2500;
	Enemy.y = 80;
	EnemyGraph = LoadGraph("Enzi.png");
	EnemyMuki = 1;
	if (EnemyMuki == 1) Enemy.x += 3;
	if (EnemyMuki == 0) Enemy.x -= 3;
	// �l�p�N����ʉE�[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
	if (Enemy.x > 4000)
	{
		Enemy.x = 4000;
		EnemyMuki = 0;
	}
	if (Enemy.x < 0)
	{
		Enemy.x = 0;
		EnemyMuki = 1;
	}
	LoadGraphScreen(Enemy.x - 350, Enemy.y, "Enzi.png", TRUE);
}

/*
// �����ɐi�ނ����̒e�̎�����������
int RazerShot::shot(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	return 0;
}

// �z�[�~���O����e�̎�����������
int HomingShot::shot(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	return 0;
}

std::unique_ptr<AbstractShot> AbstractShot::newInstance(int type)
{
	switch (type) {
	case 0:
		return std::unique_ptr<AbstractShot>(new RazerShot());
	case 1:
		return std::unique_ptr<AbstractShot>(new HomingShot());
	}
}
*/

int Shot::EnemyShred(int n, int& PlayerX, int& PlayerY, int& EnemyX, int& EnemyY, int& EnemyCounter, int& EnemyCounter2, int& EnemyAngle)
{
	switch (n) {
	case 0:
		return ShotType1(PlayerX, PlayerY, EnemyX, EnemyY, EnemyCounter);
	case 1:
		return ShotType2(PlayerX, PlayerY, EnemyX, EnemyY, EnemyCounter);
	case 2:
		return ShotType3(PlayerX, PlayerY, EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle);
	case 3:
		return ShotType4(PlayerX, PlayerY, EnemyX, EnemyY, EnemyCounter, EnemyCounter2, EnemyAngle);
	case 4:
		return ShotType5(EnemyX, EnemyY, EnemyCounter, EnemyAngle);
	default:
		return -1;
	}
}

int Shot::ShotAdd(int x, int y, int Size, double Angle, double Speed)
{
	int i;

	// �g���Ă��Ȃ��V���b�g������
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
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
		++EnemyShotNum;
	}

	// �I��
	return 0;
}

int Shot::ShotType1(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter)
{
	int i;

	// �J�E���^���S�O�ɂȂ�����V���b�g����
	if (EnemyCounter == 40)
	{
		double Angle, d;

		// �G���猩�����@�ւ̕������擾
		Angle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));

		// �R�v�`�x�V���b�g����
		d = Angle - (PI / 3) / 2;
		for (i = 0; i < 3; ++i)
		{
			ShotAdd(EnemyX, EnemyY, 4, d, 30);
			d += (PI / 3) / 2;
		}

		// �J�E���^��������
		EnemyCounter = 0;
	}
	else
	{
		// �J�E���^���C���N�������g
		++EnemyCounter;
	}

	// �I��
	return 0;
}

int Shot::ShotType2(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter)
{
	// int i;

	// �J�E���^���P�O�ɂȂ�����V���b�g����
	if (EnemyCounter == 10)
	{
		double Angle, d;

		// �G���猩�����@�ւ̕������擾
		Angle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));

		// ���C�h�V���b�g�����A�������p�x��ς��Ĉ�����V���b�g
		d = Angle - PI;
		for (int i = 0; i < 20; ++i)
		{
			ShotAdd(EnemyX, EnemyY, 4, d, 30);
			d += 2 * PI / 20;
		}

		// �J�E���^��������
		EnemyCounter = 0;
	}
	else
	{
		// �J�E���^���C���N�������g
		++EnemyCounter;
	}

	// �I��
	return 0;
}

int Shot::ShotType3(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	// �J�E���^�Q�̒l�ɂ���ď����𕪊�
	switch (EnemyCounter2)
	{
	case 0:
		// �҂�����

		// �J�E���^���C���N�������g
		++EnemyCounter;

		// �J�E���^���U�O�ɂȂ�����J�E���^�Q�̒l�𑝂₵�āA�V���b�g�����Ɉڂ�
		if (EnemyCounter > 60)
		{
			++EnemyCounter2;
			EnemyCounter = 0;

			// ���̂Ƃ��̎��@�ւ̕�����ۑ�
			EnemyAngle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));
		}
		break;

	case 1:
		// �V���b�g����

		// �J�E���^���T�̔{���̎��̂݃V���b�g����
		if (EnemyCounter % 5 == 0)
		{
			ShotAdd(EnemyX, EnemyY, 4, EnemyAngle, 30 + EnemyCounter / 15);
		}

		// �J�E���^���C���N�������g�A�J�E���g���T�O�ɂȂ�����҂��ɖ߂�
		++EnemyCounter;
		if (EnemyCounter == 50)
		{
			EnemyCounter2 = 0;
			EnemyCounter = 0;
		}
		break;
	}

	// �I��
	return 0;
}

int Shot::ShotType4(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	// �J�E���^�Q�̒l�ɂ���ď����𕪊�
	switch (EnemyCounter2)
	{
	case 0:
		// �҂�����

		// �J�E���^���C���N�������g
		++EnemyCounter;

		// �J�E���^���U�O�ɂȂ�����J�E���^�Q�̒l�𑝂₵�āA�V���b�g�����Ɉڂ�
		if (EnemyCounter > 60)
		{
			++EnemyCounter2;
			EnemyCounter = 0;

			// ���̂Ƃ��̎��@�ւ̕�����ۑ�
			EnemyAngle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));
		}
		break;

	case 1:
		//�V���b�g����

		// �J�E���^���Q�̔{���̎��̂݃V���b�g����
		if (EnemyCounter % 2 == 0)
		{
			double Angle;

			// ��΂������ɂԂ������
			Angle = EnemyAngle + (PI / 3600 * (GetRand(800) - 400));

			// �V���b�g
			ShotAdd(EnemyX, EnemyY, 5, Angle, 30);
		}

		// �J�E���^���C���N�������g�A�J�E���g���T�O�ɂȂ�����҂��ɖ߂�
		EnemyCounter++;
		if (EnemyCounter == 50)
		{
			EnemyCounter2 = 0;
			EnemyCounter = 0;
		}
		break;
	}

	// �I��
	return 0;
}

int Shot::ShotType5(int EnemyX, int EnemyY, int EnemyCounter, int EnemyAngle)
{
	int i;

	// �J�E���^���Q�ɂȂ�����V���b�g�������s��
	if (EnemyCounter == 2)
	{
		// �V���b�g�̕�����ύX
		EnemyAngle += 0.2;

		// ���x�̈Ⴄ�܂̃V���b�g�𔭎�
		for (i = 0; i < 5; ++i)
		{
			ShotAdd(EnemyX, EnemyY, 6, EnemyAngle, 30 + (i * 8000));
		}

		// �J�E���^������������
		EnemyCounter = 0;
	}
	else
	{
		EnemyCounter++;
	}

	// �I��
	return 0;
}

void Shot::ShotInit()
{
	// �V���b�g�̑��݂�����������
	for (int i = 0; i < MAX_SHOT; ++i)
		ShotValid[i] = 0;
}

void Shot::PlayerShot1()
{
	// �V���b�g�̈ړ�����
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// �V���b�g�f�[�^��������������X�L�b�v
		if (ShotValid[j] == 0) continue;

		// �ʒu����ɂ��炷
		ShotY[j] -= 80;

		// ��ʊO�ɏo�Ă�����V���b�g�f�[�^�𖳌��ɂ���
		if (ShotY[j] < -32) ShotValid[j] = 0;
	}
}

void Shot::PlayerShot2(int& PlayerX, int& PlayerY)
{
	int j;
	// �g���Ă��Ȃ��V���b�g�f�[�^��T��
	for (j = 0; j < MAX_SHOT; ++j)
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

void Shot::EnemyShooting(int& PlayerX, int& PlayerY, int& PlayerW, int& PlayerH)
{
	// �e�̏���
	int i, Con, Num;

	// �e�̐������ړ��������J��Ԃ�
	Con = 0;
	Num = EnemyShotNum;
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
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

				DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE);
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

void Shot::Shooting(int& EnemyX, int& EnemyY, int& EnemyW, int& EnemyH)
{
	// �V���b�g��`�悷��
	for (int j = 0; j < MAX_SHOT; ++j)
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
}