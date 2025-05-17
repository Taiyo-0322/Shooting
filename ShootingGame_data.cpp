#include "ShootingGame_data.hpp"

void Title::fontHandle()
{
	FontHandle = CreateFontToHandle(NULL, 500, 50);
	FontHandle2 = CreateFontToHandle(NULL, 500, 50);
	FontHandle3 = CreateFontToHandle(NULL, 500, 50);
}

void Title::drawHandle()
{
	DrawStringToHandle(800, 500, "シューティング", GetColor(255, 255, 255), FontHandle);
	DrawStringToHandle(1800, 1200, "ゲーム", GetColor(255, 255, 255), FontHandle2);
	DrawStringToHandle(1500, 2500, "ＬＡＤＹ ?", GetColor(255, 255, 255), FontHandle3);
}

void Title::deleteHandle()
{
	DeleteFontToHandle(FontHandle);
	DeleteFontToHandle(FontHandle2);
	DeleteFontToHandle(FontHandle3);
}

Player::Player(Shot* shot_) : playerX(2500), playerY(2000)
{
	m_shot = shot_;
}

void Player::playerKey(int key)
{
	if (key & PAD_INPUT_UP) playerY -= 50;
	if (key & PAD_INPUT_DOWN) playerY += 50;
	if (key & PAD_INPUT_RIGHT) playerX += 50;
	if (key & PAD_INPUT_LEFT) playerX -= 50;
}

void Player::playerDraw()
{
	playerGraph = LoadGraph("player.png");
	LoadGraphScreen(playerX, playerY, "player.png", TRUE);
}

int Player::plGraph()
{
	playerDraw();
	return playerGraph;
}

int Player::getPlayerX()
{
	return playerX;
}

int Player::getPlayerY()
{
	return playerY;
}

Enemy::Enemy(Shot* shot_) : enemyX(2500), enemyY(80)
{
	m_shot = shot_;
}

void Enemy::enemyDraw(int enemymuki)
{
	enGraph();
	if (enemymuki == 1) enemyX += 30;

	if (enemyX > 4500)
	{
		enemyX = 0;
	}
	LoadGraphScreen(enemyX - 300, enemyY, "enemy.png", TRUE);
}

int Enemy::enGraph()
{
	enemyGraph = LoadGraph("enemy.png");
	return enemyGraph;
}

int Enemy::getEnemyX()
{
	return enemyX;
}

int Enemy::getEnemyY()
{
	return enemyY;
}

Shot::Shot()
{
	m_player = new Player(this);
	m_enemy = new Enemy(this);
	enemyShot[MAX_ENEMYSHOT + 1] = {};
	shotValid[MAX_SHOT + 1] = {};
	shotX[MAX_SHOT + 1] = {};
	shotY[MAX_SHOT + 1] = {};
	shotW = 0;
	shotH = 0;
	enemyShotW = 0;
	enemyShotH = 0;
	playerX = 0;
	playerY = 0;
	enemyX = 0;
	enemyY = 0;
}

int Shot::plGraph()
{
	return m_player->plGraph();
}

int Shot::enGraph()
{
	return m_enemy->enGraph();
}

void Shot::enemyStruct()
{
	enemyCounter = 0;
	enemyCounter2 = 0;
	enemyAngle = 0.0;
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

void Shot::enemyDraw_(int enemyMuki)
{
	m_enemy->enemyDraw(enemyMuki);
}

int Shot::shotAdd(int x, int y, int size, double angle, double speed)
{
	int i;

	for (i = 0; i < MAX_ENEMYSHOT; ++i)
		if (enemyShot[i].validFlag == FALSE) break;
	if (i == MAX_ENEMYSHOT) return -1;

	{
		enemyShot[i].x = x;
		enemyShot[i].y = y;

		enemyShot[i].size = size;

		enemyShot[i].sx = (int)(cos(angle) * speed);
		enemyShot[i].sy = (int)(sin(angle) * speed);

		enemyShot[i].validFlag = TRUE;

		++enemyShotNum;
	}

	return 0;
}

int Shot::shotType1()
{
	playerX = m_player->getPlayerX();
	playerY = m_player->getPlayerY();
	enemyX = m_enemy->getEnemyX();
	enemyY = m_enemy->getEnemyY();
	int i;

	if (enemyCounter == 40)
	{
		double angle, d;

		angle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));

		d = angle - (PI / 3) / 2;
		for (i = 0; i < 3; ++i)
		{
			shotAdd(enemyX, enemyY, 4, d, 30);
			d += (PI / 3) / 2;
		}

		enemyCounter = 0;
	}
	else
	{
		++enemyCounter;
	}

	return 0;
}

int Shot::shotType2()
{
	playerX = m_player->getPlayerX();
	playerY = m_player->getPlayerY();
	enemyX = m_enemy->getEnemyX();
	enemyY = m_enemy->getEnemyY();

	if (enemyCounter == 10)
	{
		double angle, d;

		angle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));

		d = angle - PI;
		for (int i = 0; i < 20; ++i)
		{
			shotAdd(enemyX, enemyY, 4, d, 30);
			d += 2 * PI / 20;
		}

		enemyCounter = 0;
	}
	else
	{
		++enemyCounter;
	}

	return 0;
}

int Shot::shotType3()
{
	playerX = m_player->getPlayerX();
	playerY = m_player->getPlayerY();
	enemyX = m_enemy->getEnemyX();
	enemyY = m_enemy->getEnemyY();

	switch (enemyCounter2)
	{
	case 0:
		++enemyCounter;

		if (enemyCounter > 60)
		{
			++enemyCounter2;
			enemyCounter = 0;

			// このときの自機への方向を保存
			enemyAngle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));
		}
		break;

	case 1:
		if (enemyCounter % 5 == 0)
		{
			shotAdd(enemyX, enemyY, 4, enemyAngle, 30 + enemyCounter / 15);
		}

		++enemyCounter;
		if (enemyCounter == 50)
		{
			enemyCounter2 = 0;
			enemyCounter = 0;
		}
		break;
	}

	return 0;
}

int Shot::shotType4()
{
	playerX = m_player->getPlayerX();
	playerY = m_player->getPlayerY();
	enemyX = m_enemy->getEnemyX();
	enemyY = m_enemy->getEnemyY();

	switch (enemyCounter2)
	{
	case 0:
		++enemyCounter;

		if (enemyCounter > 60)
		{
			++enemyCounter2;
			enemyCounter = 0;

			enemyAngle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));
		}
		break;

	case 1:
		if (enemyCounter % 2 == 0)
		{
			double angle;

			angle = enemyAngle + (PI / 3600 * (GetRand(800) - 400));

			shotAdd(enemyX, enemyY, 5, angle, 30);
		}

		++enemyCounter;
		if (enemyCounter == 50)
		{
			enemyCounter2 = 0;
			enemyCounter = 0;
		}
		break;
	}

	// 終了
	return 0;
}

int Shot::shotType5()
{
	playerX = m_player->getPlayerX();
	playerY = m_player->getPlayerY();
	enemyX = m_enemy->getEnemyX();
	enemyY = m_enemy->getEnemyY();

	int i;

	if (enemyCounter == 2)
	{
		enemyAngle += 0.2;

		for (i = 0; i < 5; ++i)
		{
			shotAdd(enemyX, enemyY, 6, enemyAngle, 30 + (i * 8000));
		}

		enemyCounter = 0;
	}
	else
	{
		++enemyCounter;
	}

	return 0;
}

void Shot::shotInit()
{
	for (int i = 0; i < MAX_SHOT; ++i)
		shotValid[i] = 0;
}

void Shot::playerShot1(int Key)
{
	int key = Key;
	m_player->playerKey(key);
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		if (shotValid[j] == 0) continue;

		shotY[j] -= 80;

		if (shotY[j] < -32) shotValid[j] = 0;
	}
}

void Shot::playerShot2()
{
	m_player->getPlayerX();
	m_player->getPlayerY();
	int j;
	for (j = 0; j < MAX_SHOT; ++j)
	{
		if (shotValid[j] == 0) break;
	}

	if (j != MAX_SHOT)
	{
		shotX[j] = playerX + 455;
		shotY[j] = playerY;

		shotValid[j] = 1;
	}
}

void Shot::enemyShooting(int playerW, int playerH)
{
	m_player->getPlayerX();
	m_player->getPlayerY();

	int i, con, num;
	con = 0;
	num = enemyShotNum;
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
	{
		if (enemyShot[i].validFlag == TRUE)
		{
			enemyShot[i].x += enemyShot[i].sx;
			enemyShot[i].y += enemyShot[i].sy;

			if (((enemyShot[i].x) < -2000) ||
				((enemyShot[i].x) > 6600) ||
				((enemyShot[i].y) < -2000) ||
				((enemyShot[i].y) > 5000))
			{
				enemyShot[i].validFlag = FALSE;

				--enemyShotNum;
			}

			{
				int x, y;

				x = enemyShot[i].x;
				y = enemyShot[i].y;

				DrawCircle(x + 100, y + 300, 50, GetColor(255, 0, 0), TRUE);
				
				GetGraphSize(DrawCircle(x + 100, y + 300, 50, GetColor(255, 0, 0), TRUE), &enemyShotW, &enemyShotH);
				
				if (((enemyShot[i].x > playerX && enemyShot[i].x < playerX + playerW) ||
					(playerX > enemyShot[i].x && playerX < enemyShot[i].x + enemyShotW)) &&
					((enemyShot[i].y > playerY && enemyShot[i].y < playerY + playerH) ||
						(playerY > enemyShot[i].y && playerY < enemyShot[i].y + enemyShotH)))
				{
					enemyShot[i].validFlag = FALSE;
					--playerHP;
					if (playerHP <= 0)
					{
						int FontHandle4;
						FontHandle4 = CreateFontToHandle(NULL, 500, 50);
						DrawStringToHandle(1500, 1200, "ゲームオーバー", GetColor(255, 255, 255), FontHandle4);
						WaitKey();
						DxLib_End();
					}
				}
			}

			++con;

			if (num == con) break;
		}
	}
}

void Shot::shooting(int enemyW, int enemyH)
{
	m_player->playerDraw();
	m_enemy->getEnemyX();
	m_enemy->getEnemyY();

	for (int j = 0; j < MAX_SHOT; ++j)
	{
		if (shotValid[j] == 1)
		{
			DrawBox(shotX[j] - 350, shotY[j], shotX[j] - 250, shotY[j] + 160,
				GetColor(0, 0, 255), TRUE);
			GetGraphSize(DrawBox(shotX[j] - 350, shotY[j], shotX[j] - 250, shotY[j] + 160,
				GetColor(0, 0, 255), TRUE), &shotW, &shotH);
			if (((shotX[j] > enemyX && shotX[j] < enemyX + enemyW) ||
				(enemyX > shotX[j] && enemyX < shotX[j] + shotW)) &&
				((shotY[j] > enemyY && shotY[j] < enemyY + enemyH) ||
					(enemyY > shotY[j] && enemyY < shotY[j] + shotH)))
			{
				shotValid[j] = 0;
				--enemyHP;
				if (enemyHP <= 0)
				{
					int FontHandle5;
					FontHandle5 = CreateFontToHandle(NULL, 500, 50);
					DrawStringToHandle(1800, 1200, "クリア", GetColor(255, 255, 255), FontHandle5);
					WaitKey();
					DxLib_End();
				}
			}
		}

	}
}
