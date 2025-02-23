#ifndef _DELIVERABLE_DATA_HPP_
#define _DELIVERABLE_DATA_HPP_

#include <cmath>
#include "DxLib.h"

// RAII=��΂ɌĂяo�������������A�f�X�g���N�^�Ƃ���C++���L�̋@�\���g���ČĂяo���Z�p
class DxLibInitializer {
public:
	DxLibInitializer() {
		DxLib_Init();
	}
	~DxLibInitializer() {
		DxLib_End();
	}
};

class Player
{
private:
	int playerX, playerY;
	int playerGraph = LoadGraph("Dabi.png");
public:
	Player();
	void playerKey(int Key);
	int plGraph();
	void playerDraw();
	int getPlayerX();
	int getPlayerY();
};

class Enemy
{
private:
	int enemyX, enemyY;
	int enemyGraph = LoadGraph("Enzi.png");
public:
	Enemy();
	int enGraph();
	void enemyDraw(int enemyMuki);
	int getEnemyX();
	int getEnemyY();
};

class Shot
{
private:
	Player* m_player;
	Enemy* m_enemy;
	static constexpr int MAX_SHOT = 4;
	static constexpr int MAX_ENEMYSHOT = 2000;
	static constexpr double PI = 3.151962;
	int shotValid[MAX_SHOT];	// �V���b�g�����݂��邩�A�t���O
	struct ENEMYSHOT
	{
		int x, y;
		int sx, sy;
		int size;
		int validFlag;
	}enemyShot[MAX_ENEMYSHOT];
	int enemyCounter = 0;
	int enemyCounter2 = 0;
	double enemyAngle = 0.0;
	int enemyShotNum = 0;
	int shotAdd(int x, int y, int size, double angle, double speed);
	int shotW, shotH, enemyShotW, enemyShotH;
	int shotX[MAX_SHOT], shotY[MAX_SHOT];	// �V���b�g�̈ʒu
	int playerX, playerY, enemyX, enemyY;
public:
	Shot();
	Shot(Player* player, Enemy* enemy);
	int enemyShred(int n);
	int shotType1();
	int shotType2();
	int shotType3();
	int shotType4();
	int shotType5();
	void enemyStruct();
	void shotInit();
	void playerShot1();
	void playerShot2();
	void enemyShooting(int playerW, int playerH);
	void shooting(int enemyW, int enemyH);
};
#endif // !_DELIVERABLE_DATA_HPP_