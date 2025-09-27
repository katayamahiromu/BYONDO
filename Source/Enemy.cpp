#include"Enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"

// 回転処理
void Enemy::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	//進行ベクトルがゼロベクトルの場合は処理する必要無し
	if (vx == 0.0f && vz == 0.0f)return;
	float lenth = sqrtf(vx * vx + vz * vz);

	//進行ベクトルを単位ベクトル化
	vx /= lenth;
	vz /= lenth;

	//自身の回転値から前方向を求める
	float frontX = -sinf(angle.y);
	float frontZ = -cosf(angle.y);

	//回転角を求めるため、２つの単位ベクトルの内積を計算する
	float dot = (vx * frontX) + (vz * frontZ); //内積

	//内積値は-1.0~1.0で表現されており、２つの単位ベクトルの角度が
	//小さいほど1.0に近づくという性質を利用して回転速度を調整する
	float rot = 1.0f - dot; //補正値
	if (rot > speed) rot = speed;

	//左右判定を行うために２つの単位ベクトルの外積を計算する
	float cross = (vz * frontX) - (vx * frontZ);

	//2Dの外積値が正の場合か負の場合によって左右判定を行える
	//左右判定を行う事によって左右回転を選択する
	if (cross < 0.0f)
	{
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

//デバッグプリミティブ描画
void Enemy::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	//衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//衝突用判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// デバッグ用GUI描画
void Enemy::DrawDebugGui()
{
    // 位置
    ImGui::InputFloat3("Position", &position.x);
    // 速度
    ImGui::InputFloat3("Velocity", &velocity.x);
    // 半径
    ImGui::InputFloat("Radius", &radius);
    // 高さ
    ImGui::InputFloat("Height", &height);
    // 健康
    ImGui::InputFloat("Health", &health);
}

//破棄
void Enemy::Destroy() {
	EnemyManager::Instance().Remove(this);
}