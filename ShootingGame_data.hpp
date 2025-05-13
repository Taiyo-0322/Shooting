#ifndef _SHOOTINGGAME_DATA_HPP_
#define _SHOTINGGAME_DATA_HPP_

#include <cmath>
#include "DxLib.h"

// RAII=絶対に呼び出したい処理を、デストラクタというC++特有の機能を使って呼び出す技術
class DxLibInitializer {
public:
	DxLibInitializer() {
		DxLib_Init();
	}
	~DxLibInitializer() {
		DxLib_End();
	}
};

class Title
{
private:
	int FontHandle, FontHandle2, FontHandle3;
public:
	void fontHandle();
	void drawHandle();
	void deleteHandle();
};

class Shot;

class Player
{
private:
	Shot* m_shot;
	int playerX, playerY;
	int playerGraph = LoadGraph("Dabi.png");
public:
	Player(Shot* shot_);
	void playerKey(int Key);
	int plGraph();
	void playerDraw();
	int getPlayerX();
	int getPlayerY();
};

class Enemy
{
private:
	Shot* m_shot;
	int enemyX, enemyY;
	int enemyGraph = LoadGraph("Enzi.png");
public:
	Enemy(Shot* shot_);
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
	int shotValid[MAX_SHOT + 1];	// ショットが存在するか、フラグ
	struct ENEMYSHOT
	{
		int x, y;
		int sx, sy;
		int size;
		int validFlag;
	}enemyShot[MAX_ENEMYSHOT + 1];
	int enemyCounter = 0;
	int enemyCounter2 = 0;
	double enemyAngle = 0.0;
	int enemyShotNum = 0;
	int shotAdd(int x, int y, int size, double angle, double speed);
	int shotW, shotH, enemyShotW, enemyShotH;
	int shotX[MAX_SHOT + 1], shotY[MAX_SHOT + 1];	// ショットの位置
	int playerX, playerY, enemyX, enemyY;
	int playerHP = 20;
	int enemyHP = 20;
public:
	Shot();
	int enemyShred(int n);
	void enemyDraw_(int enemyMuki);
	int plGraph();
	int enGraph();
	int shotType1();
	int shotType2();
	int shotType3();
	int shotType4();
	int shotType5();
	void enemyStruct();
	void shotInit();
	void playerShot1(int Key);
	void playerShot2();
	void enemyShooting(int playerW, int playerH);
	void shooting(int enemyW, int enemyH);
};
#endif // !_SHOTINGGAME_DATAHPP_