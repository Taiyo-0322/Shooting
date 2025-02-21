#ifndef _SHOOTING_DATA_HPP_
#define _SHOOTING_DATA_HPP_

#include <cmath>
//#include <memory>
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
private:
	int PlayerX, PlayerY, PlayerGraph, PlayerW, PlayerH;
public:
	void PlayerSize(int& PlayerX, int& PlayerY, int& PlayerW, int& PlayerH);
	void PlayerDrow();
};

class Enemy
{
private:
	int EnemyX, EnemyY, EnemyMuki, EnemyGraph, EnemyW, EnemyH;
	struct ENEMY
	{
		int x, y;
		int Counter;
		int Counter2;
		int Angle;
	};
	ENEMY Enemy;
public:
	void EnemyStruct(int& EnemyX, int& EnemyY, int& EnemyCounter, int& EnemyCounter2, int& EnemyAngle);
	void EnemySize(int& EnemyW, int& EnemyH);
	void EnemyDrow();
};

class Shot
{
	static constexpr int MAX_SHOT = 4;
	static constexpr int MAX_ENEMYSHOT = 2000;
	static constexpr double PI = 3.151962;
	int ShotValid[MAX_SHOT];	// ショットが存在するか、フラグ
	struct ENEMYSHOT
	{
		int x, y;
		int sx, sy;
		int Size;
		int ValidFlag;
	};
	ENEMYSHOT EnemyShot[MAX_ENEMYSHOT];
	int EnemyShotNum = 0;
	int ShotMode;
	int ShotAdd(int x, int y, int Size, double Angle, double Speed);
	int ShotType5(int EnemyX, int EnemyY, int EnemyCounter, int EnemyAngle);
	int ShotType4(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle);
	int ShotType3(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle);
	int ShotType2(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter);
	int ShotType1(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter);
	int ShotW, ShotH, EnemyShotW, EnemyShotH;
	int ShotX[MAX_SHOT], ShotY[MAX_SHOT];	// ショットの位置
public:
	int EnemyShred(int n, int& PlayerX, int& PlayerY, int& EnemyX, int& EnemyY, int& EnemyCounter, int& EnemyCounter2, int& EnemyAngle);
	void ShotInit();
	void PlayerShot1();
	void PlayerShot2(int& PlayerX, int& PlayerY);
	void EnemyShooting(int& PlayerX, int& PlayerY, int& PlayerW, int& PlayerH);
	void Shooting(int& EnemyX, int& EnemyY, int& EnemyW, int& EnemyH);
};

#endif // !_SHOOTING_DATA_HPP_