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
	// 四角君が画面右端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
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
// 直線に進むだけの弾の実装をここに
int RazerShot::shot(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	return 0;
}

// ホーミングする弾の実装をここに
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

	// 使われていないショットを検索
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
		if (EnemyShot[i].ValidFlag == FALSE) break;
	if (i == MAX_ENEMYSHOT) return -1;

	// 新しいショットのデータを初期化
	{
		// 座標セット
		EnemyShot[i].x = x;
		EnemyShot[i].y = y;

		// サイズセット
		EnemyShot[i].Size = Size;

		// 飛んで行く方向とスピードから、Ｘ軸方向への移動速度とＹ軸方向への移動速度を得る
		EnemyShot[i].sx = (int)(cos(Angle) * Speed);
		EnemyShot[i].sy = (int)(sin(Angle) * Speed);

		// ショットデータの有効フラグを立てる
		EnemyShot[i].ValidFlag = TRUE;

		// ショットの数を増やす
		++EnemyShotNum;
	}

	// 終了
	return 0;
}

int Shot::ShotType1(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter)
{
	int i;

	// カウンタが４０になったらショットする
	if (EnemyCounter == 40)
	{
		double Angle, d;

		// 敵から見た自機への方向を取得
		Angle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));

		// ３ＷＡＹショット処理
		d = Angle - (PI / 3) / 2;
		for (i = 0; i < 3; ++i)
		{
			ShotAdd(EnemyX, EnemyY, 4, d, 30);
			d += (PI / 3) / 2;
		}

		// カウンタを初期化
		EnemyCounter = 0;
	}
	else
	{
		// カウンタをインクリメント
		++EnemyCounter;
	}

	// 終了
	return 0;
}

int Shot::ShotType2(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter)
{
	// int i;

	// カウンタが１０になったらショットする
	if (EnemyCounter == 10)
	{
		double Angle, d;

		// 敵から見た自機への方向を取得
		Angle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));

		// ワイドショット処理、少しずつ角度を変えて一周分ショット
		d = Angle - PI;
		for (int i = 0; i < 20; ++i)
		{
			ShotAdd(EnemyX, EnemyY, 4, d, 30);
			d += 2 * PI / 20;
		}

		// カウンタを初期化
		EnemyCounter = 0;
	}
	else
	{
		// カウンタをインクリメント
		++EnemyCounter;
	}

	// 終了
	return 0;
}

int Shot::ShotType3(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	// カウンタ２の値によって処理を分岐
	switch (EnemyCounter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		++EnemyCounter;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (EnemyCounter > 60)
		{
			++EnemyCounter2;
			EnemyCounter = 0;

			// このときの自機への方向を保存
			EnemyAngle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));
		}
		break;

	case 1:
		// ショット処理

		// カウンタが５の倍数の時のみショットする
		if (EnemyCounter % 5 == 0)
		{
			ShotAdd(EnemyX, EnemyY, 4, EnemyAngle, 30 + EnemyCounter / 15);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
		++EnemyCounter;
		if (EnemyCounter == 50)
		{
			EnemyCounter2 = 0;
			EnemyCounter = 0;
		}
		break;
	}

	// 終了
	return 0;
}

int Shot::ShotType4(int PlayerX, int PlayerY, int EnemyX, int EnemyY, int EnemyCounter, int EnemyCounter2, int EnemyAngle)
{
	// カウンタ２の値によって処理を分岐
	switch (EnemyCounter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		++EnemyCounter;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (EnemyCounter > 60)
		{
			++EnemyCounter2;
			EnemyCounter = 0;

			// このときの自機への方向を保存
			EnemyAngle = atan2((double)(PlayerY - EnemyY), (double)(PlayerX - EnemyX));
		}
		break;

	case 1:
		//ショット処理

		// カウンタが２の倍数の時のみショットする
		if (EnemyCounter % 2 == 0)
		{
			double Angle;

			// 飛ばす方向にぶれをつける
			Angle = EnemyAngle + (PI / 3600 * (GetRand(800) - 400));

			// ショット
			ShotAdd(EnemyX, EnemyY, 5, Angle, 30);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
		EnemyCounter++;
		if (EnemyCounter == 50)
		{
			EnemyCounter2 = 0;
			EnemyCounter = 0;
		}
		break;
	}

	// 終了
	return 0;
}

int Shot::ShotType5(int EnemyX, int EnemyY, int EnemyCounter, int EnemyAngle)
{
	int i;

	// カウンタが２になったらショット処理を行う
	if (EnemyCounter == 2)
	{
		// ショットの方向を変更
		EnemyAngle += 0.2;

		// 速度の違う五つのショットを発射
		for (i = 0; i < 5; ++i)
		{
			ShotAdd(EnemyX, EnemyY, 6, EnemyAngle, 30 + (i * 8000));
		}

		// カウンタを初期化する
		EnemyCounter = 0;
	}
	else
	{
		EnemyCounter++;
	}

	// 終了
	return 0;
}

void Shot::ShotInit()
{
	// ショットの存在を初期化する
	for (int i = 0; i < MAX_SHOT; ++i)
		ShotValid[i] = 0;
}

void Shot::PlayerShot1()
{
	// ショットの移動処理
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// ショットデータが無効だったらスキップ
		if (ShotValid[j] == 0) continue;

		// 位置を上にずらす
		ShotY[j] -= 80;

		// 画面外に出ていたらショットデータを無効にする
		if (ShotY[j] < -32) ShotValid[j] = 0;
	}
}

void Shot::PlayerShot2(int& PlayerX, int& PlayerY)
{
	int j;
	// 使われていないショットデータを探す
	for (j = 0; j < MAX_SHOT; ++j)
	{
		if (ShotValid[j] == 0) break;
	}

	// もし使われていないショットデータがあったらショットを出す
	if (j != MAX_SHOT)
	{
		// ショットの位置を設定
		ShotX[j] = PlayerX + 260;
		ShotY[j] = PlayerY;

		// ショットデータを使用中にセット
		ShotValid[j] = 1;
	}
}

void Shot::EnemyShooting(int& PlayerX, int& PlayerY, int& PlayerW, int& PlayerH)
{
	// 弾の処理
	int i, Con, Num;

	// 弾の数だけ移動処理を繰り返す
	Con = 0;
	Num = EnemyShotNum;
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
	{
		// 弾のデータが有効な場合は処理
		if (EnemyShot[i].ValidFlag == TRUE)
		{
			// 移動処理
			EnemyShot[i].x += EnemyShot[i].sx;
			EnemyShot[i].y += EnemyShot[i].sy;

			// 画面外に出たら弾情報を消去する
			if (((EnemyShot[i].x) < -2000) ||
				((EnemyShot[i].x) > 6600) ||
				((EnemyShot[i].y) < -2000) ||
				((EnemyShot[i].y) > 5000))
			{
				// データの有効フラグを倒す
				EnemyShot[i].ValidFlag = FALSE;

				// 弾の数を減らす
				EnemyShotNum--;
			}

			// 弾の描画
			{
				int x, y;

				x = EnemyShot[i].x;
				y = EnemyShot[i].y;

				DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE);
				// 弾のグラフィックのサイズをえる
				GetGraphSize(DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE), &EnemyShotW, &EnemyShotH);
				// Playerとの当たり判定
				if (((EnemyShot[i].x > PlayerX && EnemyShot[i].x < PlayerX + PlayerW) ||
					(PlayerX > EnemyShot[i].x && PlayerX < EnemyShot[i].x + EnemyShotW)) &&
					((EnemyShot[i].y > PlayerY && EnemyShot[i].y < PlayerY + PlayerH) ||
						(PlayerY > EnemyShot[i].y && PlayerY < EnemyShot[i].y + EnemyShotH)))
				{
					// 接触している場合は当たった弾の存在を消す
					EnemyShot[i].ValidFlag = FALSE;;
				}
			}

			// 処理した弾の数をインクリメント
			Con++;

			// 処理した弾の数が、存在している弾の数と同じになった場合はループを抜ける
			if (Num == Con) break;
		}
	}
}

void Shot::Shooting(int& EnemyX, int& EnemyY, int& EnemyW, int& EnemyH)
{
	// ショットを描画する
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// ショットデータが有効な時のみ描画
		if (ShotValid[j] == 1)
		{
			DrawBox(ShotX[j] - 350, ShotY[j], ShotX[j] - 250, ShotY[j] + 160,
				GetColor(0, 0, 255), TRUE);
			// 弾のグラフィックのサイズをえる
			GetGraphSize(DrawBox(ShotX[j] - 350, ShotY[j], ShotX[j] - 250, ShotY[j] + 160,
				GetColor(0, 0, 255), TRUE), &ShotW, &ShotH);
			// 四角君との当たり判定
			if (((ShotX[j] > EnemyX && ShotX[j] < EnemyX + EnemyW) ||
				(EnemyX > ShotX[j] && EnemyX < ShotX[j] + ShotW)) &&
				((ShotY[j] > EnemyY && ShotY[j] < EnemyY + EnemyH) ||
					(EnemyY > ShotY[j] && EnemyY < ShotY[j] + ShotH)))
			{
				// 接触している場合は当たった弾の存在を消す
				ShotValid[j] = 0;
			}
		}

	}
}