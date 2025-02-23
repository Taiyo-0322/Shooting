#include "Deliverable_data.hpp"

void Player::playerKey(int Key)
{
	playerX = 2500;
	playerY = 2000;
	if (Key & PAD_INPUT_UP) playerY -= 50;
	if (Key & PAD_INPUT_DOWN) playerY += 50;
	if (Key & PAD_INPUT_RIGHT) playerX += 50; // 右を押していたら右に進む
	if (Key & PAD_INPUT_LEFT) playerX -= 50;	// 左を押していたら左に進む
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

	// 四角君が画面右端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
	if (enemyX > 4000)
	{
		enemyX = 4000;
		enemyMuki = 0;
	}

	// 四角君が画面左端からでそうになっていたら画面内の座標に戻してあげ、移動する方向も反転する
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

	// 使われていないショットを検索
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
		if (enemyShot[i].validFlag == FALSE) break;
	if (i == MAX_ENEMYSHOT) return -1;

	// 新しいショットのデータを初期化
	{
		// 座標セット
		enemyShot[i].x = x;
		enemyShot[i].y = y;

		// サイズセット
		enemyShot[i].size = size;

		// 飛んで行く方向とスピードから、Ｘ軸方向への移動速度とＹ軸方向への移動速度を得る
		enemyShot[i].sx = (int)(cos(angle) * speed);
		enemyShot[i].sy = (int)(sin(angle) * speed);

		// ショットデータの有効フラグを立てる
		enemyShot[i].validFlag = TRUE;

		// ショットの数を増やす
		++enemyShotNum;
	}

	// 終了
	return 0;
}

int Shot::shotType1()
{
	int i;

	// カウンタが４０になったらショットする
	if (enemyCounter == 40)
	{
		double angle, d;

		// 敵から見た自機への方向を取得
		angle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));

		// ３ＷＡＹショット処理
		d = angle - (PI / 3) / 2;
		for (i = 0; i < 3; ++i)
		{
			shotAdd(enemyX, enemyY, 4, d, 30);
			d += (PI / 3) / 2;
		}

		// カウンタを初期化
		enemyCounter = 0;
	}
	else
	{
		// カウンタをインクリメント
		++enemyCounter;
	}

	// 終了
	return 0;
}

int Shot::shotType2()
{
	// int i;

	// カウンタが１０になったらショットする
	if (enemyCounter == 10)
	{
		double angle, d;

		// 敵から見た自機への方向を取得
		angle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));

		// ワイドショット処理、少しずつ角度を変えて一周分ショット
		d = angle - PI;
		for (int i = 0; i < 20; ++i)
		{
			shotAdd(enemyX, enemyY, 4, d, 30);
			d += 2 * PI / 20;
		}

		// カウンタを初期化
		enemyCounter = 0;
	}
	else
	{
		// カウンタをインクリメント
		++enemyCounter;
	}

	// 終了
	return 0;
}

int Shot::shotType3()
{
	// カウンタ２の値によって処理を分岐
	switch (enemyCounter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		++enemyCounter;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (enemyCounter > 60)
		{
			++enemyCounter2;
			enemyCounter = 0;

			// このときの自機への方向を保存
			enemyAngle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));
		}
		break;

	case 1:
		// ショット処理

		// カウンタが５の倍数の時のみショットする
		if (enemyCounter % 5 == 0)
		{
			shotAdd(enemyX, enemyY, 4, enemyAngle, 30 + enemyCounter / 15);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
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

int Shot::shotType4()
{
	// カウンタ２の値によって処理を分岐
	switch (enemyCounter2)
	{
	case 0:
		// 待ち処理

		// カウンタをインクリメント
		++enemyCounter;

		// カウンタが６０になったらカウンタ２の値を増やして、ショット処理に移る
		if (enemyCounter > 60)
		{
			++enemyCounter2;
			enemyCounter = 0;

			// このときの自機への方向を保存
			enemyAngle = atan2((double)(playerY - enemyY), (double)(playerX - enemyX));
		}
		break;

	case 1:
		//ショット処理

		// カウンタが２の倍数の時のみショットする
		if (enemyCounter % 2 == 0)
		{
			double angle;

			// 飛ばす方向にぶれをつける
			angle = enemyAngle + (PI / 3600 * (GetRand(800) - 400));

			// ショット
			shotAdd(enemyX, enemyY, 5, angle, 30);
		}

		// カウンタをインクリメント、カウントが５０になったら待ちに戻る
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
	int i;

	// カウンタが２になったらショット処理を行う
	if (enemyCounter == 2)
	{
		// ショットの方向を変更
		enemyAngle += 0.2;

		// 速度の違う五つのショットを発射
		for (i = 0; i < 5; ++i)
		{
			shotAdd(enemyX, enemyY, 6, enemyAngle, 30 + (i * 8000));
		}

		// カウンタを初期化する
		enemyCounter = 0;
	}
	else
	{
		++enemyCounter;
	}

	// 終了
	return 0;
}

void Shot::shotInit()
{
	// ショットの存在を初期化する
	for (int i = 0; i < MAX_SHOT; ++i)
		shotValid[i] = 0;
}

void Shot::playerShot1()
{
	// ショットの移動処理
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// ショットデータが無効だったらスキップ
		if (shotValid[j] == 0) continue;

		// 位置を上にずらす
		shotY[j] -= 80;

		// 画面外に出ていたらショットデータを無効にする
		if (shotY[j] < -32) shotValid[j] = 0;
	}
}

void Shot::playerShot2()
{
	int j;
	// 使われていないショットデータを探す
	for (j = 0; j < MAX_SHOT; ++j)
	{
		if (shotValid[j] == 0) break;
	}

	// もし使われていないショットデータがあったらショットを出す
	if (j != MAX_SHOT)
	{
		// ショットの位置を設定
		shotX[j] = playerX + 260;
		shotY[j] = playerY;

		// ショットデータを使用中にセット
		shotValid[j] = 1;
	}
}

void Shot::enemyShooting()
{
	void playerSize();
	// 弾の処理
	int i, con, num;
	// 弾の数だけ移動処理を繰り返す
	con = 0;
	num = enemyShotNum;
	for (i = 0; i < MAX_ENEMYSHOT; ++i)
	{
		// 弾のデータが有効な場合は処理
		if (enemyShot[i].validFlag == TRUE)
		{
			// 移動処理
			enemyShot[i].x += enemyShot[i].sx;
			enemyShot[i].y += enemyShot[i].sy;

			// 画面外に出たら弾情報を消去する
			if (((enemyShot[i].x) < -2000) ||
				((enemyShot[i].x) > 6600) ||
				((enemyShot[i].y) < -2000) ||
				((enemyShot[i].y) > 5000))
			{
				// データの有効フラグを倒す
				enemyShot[i].validFlag = FALSE;

				// 弾の数を減らす
				--enemyShotNum;
			}

			// 弾の描画
			{
				int x, y;

				x = enemyShot[i].x;
				y = enemyShot[i].y;

				DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE);
				// 弾のグラフィックのサイズをえる
				GetGraphSize(DrawCircle(x, y + 300, 50, GetColor(255, 0, 0), TRUE), &enemyShotW, &enemyShotH);
				// Playerとの当たり判定
				if (((enemyShot[i].x > playerX && enemyShot[i].x < playerX + playerW) ||
					(playerX > enemyShot[i].x && playerX < enemyShot[i].x + enemyShotW)) &&
					((enemyShot[i].y > playerY && enemyShot[i].y < playerY + playerH) ||
						(playerY > enemyShot[i].y && playerY < enemyShot[i].y + enemyShotH)))
				{
					// 接触している場合は当たった弾の存在を消す
					enemyShot[i].validFlag = FALSE;;
				}
			}

			// 処理した弾の数をインクリメント
			++con;

			// 処理した弾の数が、存在している弾の数と同じになった場合はループを抜ける
			if (num == con) break;
		}
	}
}

void Shot::shooting()
{
	void enemySize();
	// ショットを描画する
	for (int j = 0; j < MAX_SHOT; ++j)
	{
		// ショットデータが有効な時のみ描画
		if (shotValid[j] == 1)
		{
			DrawBox(shotX[j] - 350, shotY[j], shotX[j] - 250, shotY[j] + 160,
				GetColor(0, 0, 255), TRUE);
			// 弾のグラフィックのサイズをえる
			GetGraphSize(DrawBox(shotX[j] - 350, shotY[j], shotX[j] - 250, shotY[j] + 160,
				GetColor(0, 0, 255), TRUE), &shotW, &shotH);
			// 四角君との当たり判定
			if (((shotX[j] > enemyX && shotX[j] < enemyX + enemyW) ||
				(enemyX > shotX[j] && enemyX < shotX[j] + shotW)) &&
				((shotY[j] > enemyY && shotY[j] < enemyY + enemyH) ||
					(enemyY > shotY[j] && enemyY < shotY[j] + shotH)))
			{
				// 接触している場合は当たった弾の存在を消す
				shotValid[j] = 0;
			}
		}

	}
}