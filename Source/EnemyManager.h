#pragma once
#include<vector>
#include<set>
#include"Enemy.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "Audio/Audio.h"

class EnemyManager
{
private:
	EnemyManager();
	~EnemyManager();
public:
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}
	//更新処理
	void Update(float elapsedTime, ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	//エネミー登録
	void Register(Enemy* enemy);
	//エネミー削除
	void Remove(Enemy* enemy);
	//エネミー全削除
	void Clear();
	//デバッグプリミティブ描画
	void DrawDebugPrimitive();
	// デバッグ用GUI描画
	void DrawDebugGui();
	//エネミー数取得
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
	//エネミー取得
	Enemy* GetEnemy(int index) { return enemies.at(index); }
	//エネミー同士の衝突判定
	void CollisionEnemyVsEnemy();
	// プレイヤーとエネミーの弾の衝突判定
	void CollisionPlayerVsEnemyProjectiles();

public:
	ProjectileManager projectileManager;

private:
	std::vector<Enemy*> enemies;
	std::set<Enemy*>removes;

	bool visibleDebugPrimitive = true;
	Effect* hitEffect = nullptr;
	//std::unique_ptr<AudioSource> playerHit;
};