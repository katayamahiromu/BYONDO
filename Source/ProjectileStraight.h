#pragma once

#include"Graphics/Model.h"
#include"Projectile.h"

//垂直弾丸
class ProjectileStraight :public Projectile 
{
public:
	ProjectileStraight(ProjectileManager*manager);
	~ProjectileStraight() override;

	//更新処理
	void Update(float elapsedTime) override;
	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* sheder) override;
	//発射
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
	//水平移動
	void UpdateHorizontalMove(float elapsedTime);
protected:
	Model* model = nullptr;
private:
	float speed = 20.0f;
	float lifeTimer = 10.0f;
	float destoroy_pos = 45.0f;
};

// プレイヤー青(ミライ)
class PBlueProjectileStraight : public ProjectileStraight
{
public:
	PBlueProjectileStraight(ProjectileManager* manager);
};

// プレイヤー赤(カコ)
class PRedProjectileStraight : public ProjectileStraight
{
public:
	PRedProjectileStraight(ProjectileManager* manager);
};

// 敵
class EProjectileStraight : public ProjectileStraight
{
public:
	EProjectileStraight(ProjectileManager* manager);
};