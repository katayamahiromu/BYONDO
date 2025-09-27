#pragma once

#include"Graphics/Shader.h"
#include"Graphics/Model.h"
#include"Character.h"
#include"EnemyManager.h"
#include"Collision.h"
#include"ProjectileManager.h"
#include"Effect.h"
#include"Graphics/Sprite.h"
#include"Audio/Audio.h"
#include<memory>

//プレイヤー
class Player :public Character
{
public:
	Player(char*file_path);
	~Player() override;

	//更新処理
	void Update(float elapsedTime);

	//描画処理
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//デバッグ用GUI描画
	void DrawDebugGui();
	//スティック入力値から移動ベクトルを取得
	DirectX::XMFLOAT3 GetMoveVec() const;
	//移動入力処理
	bool InputMove(float elapsedTime);
	//デバッグプリミティブ描画
	void DrawDebugPrimitive();
	//プレイヤーとエネミーとの衝突判定
	void CollisionPlayerVsEnemies();
	//弾丸入力処理
	void InputProjectile();
	//弾丸と敵の衝突判定
	void CollisionProjectilesVsEnemies();
	//攻撃入力処理
	bool InputAttack();
	//インスタンス取得
	static Player& Instance();
	//切り替え受付
	void Input_Change();
	//カメラシェイク
	void Camera_Shake();
	//行動制限
	bool Cheack_Position();
protected:
	//ダメージを受けた時に呼ばれる
	void OnDamaged() override;
	//死亡した時に呼ばれる
	void OnDead()override;
private:
	Model* model = nullptr;
	float turnSpeed = DirectX::XMConvertToRadians(-30);
	float JumpSpeed = 20.0f;
	int jumpCount = 0;
	int jumpLimit = 2;
	Effect* hitEffect = nullptr;
	float leftHandRadius = 0.4f;
	bool attackCollisionFlag = false;
	float right_projectile_offset = 1.0;
	float left_projectile_offset = -1.0;
	bool intput_projectile = false;
	float shooting_timer = 0.0f;
	float interval_shooting_time = 0.3f;
	Effect* blueToRedEffect = nullptr;
	Effect* redToBlueEffect = nullptr;
	Effect* damageEffect = nullptr;
public:
	void Set_Speed(float speed) { moveSpeed = speed; }
	void Set_Attack(float attack_) { attack = attack_; }
	void Set_Defence(float defence_) { defence = defence_; }
	float Get_Attack() { return attack; }
	float Get_Defence() { return defence; }
private:
	float moveSpeed = 10.0f;
	float attack = 0.0f;
	float defence = 0.0f;
	bool visibleDebugPrimitive = true;
	bool red = false;
private:
	void Update_Input_Projectile();
	void Update_Shooting_Timer(float elapsedTime);

protected:
		int		hitStopFrame = 6;
		int		cameraShakeFrame = 10;
		float	cameraShakeRange = 0.1f;
private:
	std::unique_ptr<AudioSource> projectile_sound;
	std::unique_ptr<AudioSource> swap_sound;
	std::unique_ptr<AudioSource> hit_sound;
	std::unique_ptr<AudioSource> enemy_hit_sound;
	std::unique_ptr<AudioSource> bomb_sound;
public:
	bool isHit = false;
	ProjectileManager projectileManager;
	std::unique_ptr<AudioSource> bomb_voice;
	std::unique_ptr<AudioSource> damage_voice;
	std::unique_ptr<AudioSource> dead_voice;
	std::unique_ptr<AudioSource> change_voice;
	std::unique_ptr<AudioSource> victory_voice;
	std::unique_ptr<AudioSource> boss_away_voice;
};