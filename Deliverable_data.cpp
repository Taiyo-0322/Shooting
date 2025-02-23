#include "Deliverable_data.hpp"

void Player::playerKey(int Key)
{
	playerX = 2500;
	playerY = 2000;
	if (Key & PAD_INPUT_UP) playerY -= 50;
	if (Key & PAD_INPUT_DOWN) playerY += 50;
	if (Key & PAD_INPUT_RIGHT) playerX += 50; // �E�������Ă�����E�ɐi��
	if (Key & PAD_INPUT_LEFT) playerX -= 50;	// ���������Ă����獶�ɐi��
}

void Player::playerSize()
{
	playerGraph = LoadGraph("Dabi.png");
	GetGraphSize(playerGraph, &playerW, &playerH);
}

void Player::playerDrow()
{
	playerGraph = LoadGraph("Dabi.png");
	LoadGraphScreen(playerX - 350, playerY, "Dabi.png", TRUE);
}

void Enemy::enemyStruct()
{
	enemyCounter = 0;
	enemyCounter2 = 0;
	enemyAngle = 0.0;
}

void Enemy::enemySize()
{
	enemyGraph = LoadGraph("Enzi.png");
	GetGraphSize(enemyGraph, &enemyW, &enemyH);
}

void Enemy::enemyDrow()
{
	enemyGraph = LoadGraph("Enzi.png");
	enemyX = 2500;
	enemyY = 80;
	enemyMuki = 1;
	if (enemyMuki == 1) enemyX += 3;
	if (enemyMuki == 0) enemyX -= 3;

	// �l�p�N����ʉE�[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
	if (enemyX > 4000)
	{
		enemyX = 4000;
		enemyMuki = 0;
	}

	// �l�p�N����ʍ��[����ł����ɂȂ��Ă������ʓ��̍��W�ɖ߂��Ă����A�ړ�������������]����
	if (enemyX < 0)
	{
		enemyX = 0;
		enemyMuki = 1;
	}
	LoadGraphScreen(enemyX - 350, enemyY, "Enzi.png", TRUE);
}

int Shot::enemyShred(int n)
{
	switch (n) {
	case 0:
		return shotType1();
	case 1:
		return shotType2();
	case 2:
		return shotType3();
	case 3:
		return shotType4();
	case 4:
		return shotType5();
	default:
		return -1;
	}
}

int Shot::shotAdd(int x, int y, int size, double angle, double speed)
{
	int i;

	// �g���Ă��Ȃ��V���b�g������
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
		if (enemyShot[i].validFlag == FALSE) break;
	if (i == MAX_ENEMYSHOT) return -1;

	// �V�����V���b�g�̃f�[�^��������
	{
		// ���W�Z�b�g
		enemyShot[i].x = x;
		enemyShot[i].y = y;

		// �T�C�Y�Z�b�g
		enemyShot[i].size = size;

		// ���ōs�������ƃX�s�[�h����A�w�������ւ̈ړ����x�Ƃx�������ւ̈ړ����x�𓾂�
		enemyShot[i].sx = (int)(cos(angle) * speed);
		enemyShot[i].sy = (int)(sin(angle) * speed);

		// �V���b�g�f�[�^�̗L���t���O�𗧂Ă�
		enemyShot[i].validFlag = TRUE;

		// �V���b�g�̐��𑝂₷
		++enemyShotNum;
	}

	// �I��
	return 0;
}

int Shot::shotType1()
{
	int i;

	// �J�E���^���S�O�ɂȂ�����V���b�g����
	if (enemyCounter == 40)
	{
		double angle, d;

		// �G���猩�����@�ւ̕������擾
		angle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));

		// �R�v�`�x�V���b�g����
		d = angle - (PI / 3) / 2;
		for (i = 0; i < 3; ++i)
		{
			shotAdd(enemyX, enemyY, 4, d, 30);
			d += (PI / 3) / 2;
		}

		// �J�E���^��������
		enemyCounter = 0;
	}
	else
	{
		// �J�E���^���C���N�������g
		++enemyCounter;
	}

	// �I��
	return 0;
}

int Shot::shotType2()
{
	// int i;

	// �J�E���^���P�O�ɂȂ�����V���b�g����
	if (enemyCounter == 10)
	{
		double angle, d;

		// �G���猩�����@�ւ̕������擾
		angle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));

		// ���C�h�V���b�g�����A�������p�x��ς��Ĉ�����V���b�g
		d = angle - PI;
		for (int i = 0; i < 20; ++i)
		{
			shotAdd(enemyX, enemyY, 4, d, 30);
			d += 2 * PI / 20;
		}

		// �J�E���^��������
		enemyCounter = 0;
	}
	else
	{
		// �J�E���^���C���N�������g
		++enemyCounter;
	}

	// �I��
	return 0;
}

int Shot::shotType3()
{
	// �J�E���^�Q�̒l�ɂ���ď����𕪊�
	switch (enemyCounter2)
	{
	case 0:
		// �҂�����

		// �J�E���^���C���N�������g
		++enemyCounter;

		// �J�E���^���U�O�ɂȂ�����J�E���^�Q�̒l�𑝂₵�āA�V���b�g�����Ɉڂ�
		if (enemyCounter > 60)
		{
			++enemyCounter2;
			enemyCounter = 0;

			// ���̂Ƃ��̎��@�ւ̕�����ۑ�
			enemyAngle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));
		}
		break;

	case 1:
		// �V���b�g����

		// �J�E���^���T�̔{���̎��̂݃V���b�g����
		if (enemyCounter % 5 == 0)
		{
			shotAdd(enemyX, enemyY, 4, enemyAngle, 30 + enemyCounter / 15);
		}

		// �J�E���^���C���N�������g�A�J�E���g���T�O�ɂȂ�����҂��ɖ߂�
		++enemyCounter;
		if (enemyCounter == 50)
		{
			enemyCounter2 = 0;
			enemyCounter = 0;
		}
		break;
	}

	// �I��
	return 0;
}

int Shot::shotType4()
{
	// �J�E���^�Q�̒l�ɂ���ď����𕪊�
	switch (enemyCounter2)
	{
	case 0:
		// �҂�����

		// �J�E���^���C���N�������g
		++enemyCounter;

		// �J�E���^���U�O�ɂȂ�����J�E���^�Q�̒l�𑝂₵�āA�V���b�g�����Ɉڂ�
		if (enemyCounter > 60)
		{
			++enemyCounter2;
			enemyCounter = 0;

			// ���̂Ƃ��̎��@�ւ̕�����ۑ�
			enemyAngle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));
		}
		break;

	case 1:
		//�V���b�g����

		// �J�E���^���Q�̔{���̎��̂݃V���b�g����
		if (enemyCounter % 2 == 0)
		{
			double angle;

			// ��΂������ɂԂ������
			angle = enemyAngle + (PI / 3600 * (GetRand(800) - 400));

			// �V���b�g
			shotAdd(enemyX, enemyY, 5, angle, 30);
		}

		// �J�E���^���C���N�������g�A�J�E���g���T�O�ɂȂ�����҂��ɖ߂�
		++enemyCounter;
		if (enemyCounter == 50)
		{
			enemyCounter2 = 0;
			enemyCounter = 0;
		}
		break;
	}

	// �I��
	return 0;
}

int Shot::shotType5()
{
	int i;

	// �J�E���^���Q�ɂȂ�����V���b�g�������s��
	if (enemyCounter == 2)
	{
		// �V���b�g�̕�����ύX
		enemyAngle += 0.2;

		// ���x�̈Ⴄ�܂̃V���b�g�𔭎�
		for (i = 0; i < 5; ++i)
		{
			shotAdd(enemyX, enemyY, 6, enemyAngle, 30 + (i * 8000));
		}

		// �J�E���^������������
		enemyCounter = 0;
	}
	else
	{
		++enemyCounter;
	}

	// �I��
	return 0;
}

void Shot::shotInit()
{
	// �V���b�g�̑��݂�����������
	for (int i = 0; i < MAX_SHOT; ++i)
		shotValid[i] = 0;
}

void Shot::playerShot1()
{
	// �V���b�g�̈ړ�����
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// �V���b�g�f�[�^��������������X�L�b�v
		if (shotValid[j] == 0) continue;

		// �ʒu����ɂ��炷
		shotY[j] -= 80;

		// ��ʊO�ɏo�Ă�����V���b�g�f�[�^�𖳌��ɂ���
		if (shotY[j] < -32) shotValid[j] = 0;
	}
}

void Shot::playerShot2()
{
	int j;
	// �g���Ă��Ȃ��V���b�g�f�[�^��T��
	for (j = 0; j < MAX_SHOT; ++j)
	{
		if (shotValid[j] == 0) break;
	}

	// �����g���Ă��Ȃ��V���b�g�f�[�^����������V���b�g���o��
	if (j != MAX_SHOT)
	{
		// �V���b�g�̈ʒu��ݒ�
		shotX[j] = playerX + 260;
		shotY[j] = playerY;

		// �V���b�g�f�[�^���g�p���ɃZ�b�g
		shotValid[j] = 1;
	}
}

void Shot::enemyShooting()
{
	void playerSize();
	// �e�̏���
	int i, con, num;
	// �e�̐������ړ��������J��Ԃ�
	con = 0;
	num = enemyShotNum;
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
	{
		// �e�̃f�[�^���L���ȏꍇ�͏���
		if (enemyShot[i].validFlag == TRUE)
		{
			// �ړ�����
			enemyShot[i].x += enemyShot[i].sx;
			enemyShot[i].y += enemyShot[i].sy;

			// ��ʊO�ɏo����e������������
			if (((enemyShot[i].x) < -2000) ||
				((enemyShot[i].x) > 6600) ||
				((enemyShot[i].y) < -2000) ||
				((enemyShot[i].y) > 5000))
			{
				// �f�[�^�̗L���t���O��|��
				enemyShot[i].validFlag = FALSE;

				// �e�̐������炷
				--enemyShotNum;
			}

			// �e�̕`��
			{
				int x, y;

				x = enemyShot[i].x;
				y = enemyShot[i].y;

				DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE);
				// �e�̃O���t�B�b�N�̃T�C�Y������
				GetGraphSize(DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE), &enemyShotW, &enemyShotH);
				// Player�Ƃ̓����蔻��
				if (((enemyShot[i].x > playerX && enemyShot[i].x < playerX + playerW) ||
					(playerX > enemyShot[i].x && playerX < enemyShot[i].x + enemyShotW)) &&
					((enemyShot[i].y > playerY && enemyShot[i].y < playerY + playerH) ||
						(playerY > enemyShot[i].y && playerY < enemyShot[i].y + enemyShotH)))
				{
					// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
					enemyShot[i].validFlag = FALSE;;
				}
			}

			// ���������e�̐����C���N�������g
			++con;

			// ���������e�̐����A���݂��Ă���e�̐��Ɠ����ɂȂ����ꍇ�̓��[�v�𔲂���
			if (num == con) break;
		}
	}
}

void Shot::shooting()
{
	void enemySize();
	// �V���b�g��`�悷��
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// �V���b�g�f�[�^���L���Ȏ��̂ݕ`��
		if (shotValid[j] == 1)
		{
			DrawBox(shotX[j] - 350, shotY[j], shotX[j] - 250, shotY[j] + 160,
				GetColor(0, 0, 255), TRUE);
			// �e�̃O���t�B�b�N�̃T�C�Y������
			GetGraphSize(DrawBox(shotX[j] - 350, shotY[j], shotX[j] - 250, shotY[j] + 160,
				GetColor(0, 0, 255), TRUE), &shotW, &shotH);
			// �l�p�N�Ƃ̓����蔻��
			if (((shotX[j] > enemyX && shotX[j] < enemyX + enemyW) ||
				(enemyX > shotX[j] && enemyX < shotX[j] + shotW)) &&
				((shotY[j] > enemyY && shotY[j] < enemyY + enemyH) ||
					(enemyY > shotY[j] && enemyY < shotY[j] + shotH)))
			{
				// �ڐG���Ă���ꍇ�͓��������e�̑��݂�����
				shotValid[j] = 0;
			}
		}

	}
}