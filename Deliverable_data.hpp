#ifndef _DELIVERABLE_DATA_HPP_
#define _DELIVERABLE_DATA_HPP_

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

class Player
{
protected:
	int playerX, playerY, playerGraph, playerW, playerH;
public:
	void playerKey(int Key);
	void playerSize();
	void playerDrow();
};

class Enemy
{
protected:
	int enemyX, enemyY, enemyCounter, enemyCounter2, enemyW, enemyH, enemyGraph, enemyMuki;
	double enemyAngle;
public:
	void enemyStruct();
	void enemySize();
	void enemyDrow();
};

class Shot : public Player, public Enemy
{
private:
	static constexpr int MAX_SHOT = 4;
	static constexpr int MAX_ENEMYSHOT = 2000;
	static constexpr double PI = 3.151962;
	int shotValid[MAX_SHOT];	// ショットが存在するか、フラグ
	struct ENEMYSHOT
	{
		int x, y;
		int sx, sy;
		int size;
		int validFlag;
	}enemyShot[MAX_ENEMYSHOT];
	//enemyShot[MAX_ENEMYSHOT];
	int enemyShotNum = 0;
	int shotAdd(int x, int y, int size, double angle, double speed);
	int shotType1();
	int shotType2();
	int shotType3();
	int shotType4();
	int shotType5();
	int shotW, shotH, enemyShotW, enemyShotH;
	int shotX[MAX_SHOT], shotY[MAX_SHOT];	// ショットの位置
public:
	int enemyShred(int n);
	void shotInit();
	void playerShot1();
	void playerShot2();
	void enemyShooting();
	void shooting();
};
#endif // !_DELIVERABLE_DATA_HPP_