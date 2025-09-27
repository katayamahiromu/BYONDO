#include<imgui.h>
#include"Player.h"
#include"Input/Input.h"
#include"Camera.h"
#include"Graphics/Graphics.h"
#include"ProjectileStraight.h"
#include"ProjectileHoming.h"
#include"Bomb.h"
#include"playerManager.h"
#include"GameParameter.h"
#include"UI.h"
#include"SceneGame.h"

static 	int swapCounter = 0;
static Player* instace = nullptr;

//インスタンス取得
Player& Player::Instance()
{
	return *instace;
}

//コンストラクタ
Player::Player(char* file_path) {
	//インスタンスポインタ取得
	instace = this;
	model = new Model(file_path);
	(file_path == "Data/Model/Player_Red/player_red.mdl") ? red = true : red = false;

	//ヒットエフェクト読み込み
	hitEffect = new Effect("Data/Effect/Damage.efk");

	// 切り替え時のエフェクト読み込み
	blueToRedEffect = new Effect("Data/Effect/Blue_to_red.efk");
	redToBlueEffect = new Effect("Data/Effect/Red_to_blue.efk");
	damageEffect = new Effect("Data/Effect/Damage.efk");

	//音楽読み込み
	projectile_sound = Audio::Instance().LoadAudioSource("Data/Audio/player_gun.wav");
	projectile_sound->SetVolume(0.3f);
	swap_sound = Audio::Instance().LoadAudioSource("Data/Audio/playerChange.wav");
	hit_sound = Audio::Instance().LoadAudioSource("Data/Audio/player_hit.wav");
	enemy_hit_sound = Audio::Instance().LoadAudioSource("Data/Audio/enemy_hit.wav");
	enemy_hit_sound->SetVolume(0.5f);
	bomb_sound = Audio::Instance().LoadAudioSource("Data/Audio/bomb.wav");
}

//デストラクタ
Player::~Player() {
	delete model;
	delete hitEffect;
	delete blueToRedEffect;
	delete redToBlueEffect;
	delete damageEffect;

	projectileManager.Clear();
}

//更新処理
void Player::Update(float elapsedTime) {

	if (MoveLimit(Graphics::Instance().GetDeviceContext(), 
		Camera::Instance().GetView(), 
		Camera::Instance().GetProjection()))
	{
		InputMove(elapsedTime);
	}
	else
	{
		switch (move_limit)
		{
		case MOVE_LIMIT::UP:
			position.z -= 0.1f;
			break;
		case MOVE_LIMIT::DAWN:
			position.z += 0.1f;
			break;
		case MOVE_LIMIT::RIGHT:
			position.x -= 0.1f;
			break;
		case MOVE_LIMIT::LEFT:
			position.x += 0.1f;
			break;
		}
	}
	//速力更新
	UpdateVelocity(elapsedTime);
	//無敵時間更新
	UpdateInvinciblTImer(elapsedTime);
	//プレイヤーと敵との衝突判定
	CollisionPlayerVsEnemies();
	//弾丸と敵の衝突判定
	CollisionProjectilesVsEnemies();
	//オブジェクト行列を更新
	UpdateTransform();
	//弾の入力受付
	Update_Input_Projectile();
	//連射のクール時間
	Update_Shooting_Timer(elapsedTime);
	//切り替え入力受付
	Input_Change();
	InputProjectile();
	//弾丸発射処理
	projectileManager.Update(elapsedTime);
	//モデルアニメーションを更新
	model->UpdateAnimation(elapsedTime);
	//モデル行列を更新
	model->UpdateTransform(transform);

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::Change)
	{
		swapCounter++;

		switch (swapCounter % 2)
		{
		case 0:
			redToBlueEffect->Play(GetPosition(), 2.0f);
			break;

		case 1:
			blueToRedEffect->Play(GetPosition(), 2.0f);
			break;
		}
	}

	if (isHit)
	{
		hit_sound->DC_Play();
		isHit = false;
	}

	if (health < 0)
	{
		health = 0;
	}
}

//移動入力処理
bool Player::InputMove(float elapsedTime) {
	//進行ベクトル取得
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//移動処理
	Move(moveVec.x, moveVec.z, moveSpeed);
	//旋回処理
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	return (moveVec.x != 0.0f || moveVec.z != 0.0f) ? true : false;
}

//描画処理
void Player::Render(ID3D11DeviceContext* dc, Shader* shader) {
	if (!isDraw) return;

	shader->Draw(dc, model);
	//弾丸描画処理
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGui() {
	if (ImGui::CollapsingHeader("Player"))
	{
		// デバッグプリミティブ表示
		ImGui::Checkbox("VisibleDebugPrimitive", &visibleDebugPrimitive);
		// 位置
		ImGui::InputFloat3("Position", &position.x);
		// 回転
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// 移動速度
		ImGui::InputFloat3("Velocity", &velocity.x);
		// スケール
		ImGui::InputFloat3("Scale", &scale.x);
		// 回転速度
		float turnDeg = DirectX::XMConvertToDegrees(turnSpeed);
		ImGui::InputFloat("TurnSpeed(Deg)", &turnDeg);
		turnSpeed = DirectX::XMConvertToRadians(turnDeg);
		// 半径
		ImGui::InputFloat("Radius", &radius);
		// 高さ
		ImGui::InputFloat("Height", &height);
		// 命
		ImGui::InputFloat("Health", &health);
		// 摩擦
		ImGui::InputFloat("Friction", &friction);
		// 加速度
		ImGui::InputFloat("Acceleration", &acceleration);
		// 空中制御係数
		ImGui::InputFloat("AirControl", &airControl);
	}

	// 弾丸
	projectileManager.DrawDebugGui();
}

//スティック入力値から移動ベクトルを取得
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//入力情報を取得 
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//カメラ方向とスティックの入力値によって進行方向を計算する
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//移動ベクトルはXZ平面に水平なベクトルになるようにする

	//カメラ右方向ベクトルをXZ単位ベクトルに変換
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

	if(cameraRightLength > 0.0f)
	{
		//単位ベクトル化
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//カメラ前方向ベクトルをXZ単位ベクトルに変換
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

	if (cameraFrontLength > 0.0f)
	{
		//単位ベクトル化
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//ステックの水平入力値をカメラ右方向に反映し、
	//ステックの垂直入力値をカメラ前方向に反映し、
	//進行ベクトルを計算する
	DirectX::XMFLOAT3 vec;
	vec.x = cameraFrontX * ay + cameraRightX * ax;
	vec.z = cameraFrontZ * ay + cameraRightZ * ax;
	//Y方向は移動しない
	vec.y = 0.0f;

	return vec;
}

void Player::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////衝突判定用のデバッグ球を描画
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//衝突用判定用のデバッグ円柱を描画
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.DrawDebugPrimitive();

	//攻撃衝突用の左手ノードのデバッグ球を描画
	if (attackCollisionFlag) {
		Model::Node* leftHandBone = model->FindNode("mixamorig:LeftHand");
		debugRenderer->DrawSphere(DirectX::XMFLOAT3(
			leftHandBone->worldTransform._41,
			leftHandBone->worldTransform._42,
			leftHandBone->worldTransform._43),
			leftHandRadius,
			DirectX::XMFLOAT4(1, 0, 0, 1)
		);
	}
}

void Player::CollisionPlayerVsEnemies() {
	EnemyManager& enemyManager = EnemyManager::Instance();
	//全ての敵と総当たりで衝突処理
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) {
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;

		//衝突判定
		if (Collision::IntersectCylinderVsCylinder(
			position,
			radius,
			height,
			enemy->GetPosition(),
			enemy->GetRadius(),
			enemy->GetHeight(),
			outPosition
		)) {
			ApplyDamage(1.0f-Get_Defence(), 1.0f);
			enemy->ApplyDamage(Get_Attack(), 0.5f);
			hitEffect->Play(position);
			enemy_hit_sound->DC_Play();
			Camera_Shake();
		}
	};
}

//ダメージを受けた時に呼ばれる
void Player::OnDamaged()
{
	hit_sound->DC_Play();
	damage_voice->DC_Play();
}

//死亡した時に呼ばれる
void Player::OnDead()
{
	if(damage_voice->IsPlay())
	{
		damage_voice->Stop();
	}
	dead_voice->Play(false);
}

void Player::Update_Shooting_Timer(float elapsedTime)
{
	if (shooting_timer > 0.0f) {
		shooting_timer -= elapsedTime;
	}
	else
	{
		shooting_timer = interval_shooting_time;
	}
}

void Player::Update_Input_Projectile()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::Attack)
	{
		intput_projectile = true;
	}
	if (gamePad.GetButtonUp() & GamePad::Attack)
	{
		intput_projectile = false;
	}
}

//弾丸入力処理
void Player::InputProjectile() 
{

	//直進弾丸発射
	if (intput_projectile && shooting_timer < 0.0f)
	{
		//前方向
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);
		//発射位置（プレイヤーの腰あたり）
		DirectX::XMFLOAT3 pos = GetPosition();
		pos.x = GetPosition().x + right_projectile_offset;
		pos.y += 1;//腰当たりのちに修正

		//発射
		ProjectileStraight* projectile_right;
		if (red)	projectile_right = new PRedProjectileStraight(&projectileManager);
		else projectile_right = new PBlueProjectileStraight(&projectileManager);
		projectile_right->Launch(dir, pos);

		//発射位置（プレイヤーの腰あたり）
		pos.x = GetPosition().x + left_projectile_offset;

		//発射
		ProjectileStraight* projectile_left;
		if (red)	projectile_left = new PRedProjectileStraight(&projectileManager);
		else projectile_left = new PBlueProjectileStraight(&projectileManager);
		projectile_left->Launch(dir, pos);

		projectile_sound->DC_Play();
	}

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::Bomb)
	{
		bool bomb_flag = Bomb_Gauge::Instance().Get_Flag();
		if (bomb_flag) {
			//爆発（仮）
			Bomb_Gauge::Instance().Use_Flag();
			Bomb* bomb = new Bomb(&projectileManager);
			bomb_voice->DC_Play();
			bomb_sound->DC_Play();
		}
	}
}

//弾丸と敵の衝突判定
void Player::CollisionProjectilesVsEnemies(){
	EnemyManager& enemyManager = EnemyManager::Instance();
	//全ての弾丸と全ての敵を総当たりで衝突判定
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < projectileCount;++i) {
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0;j < enemyCount;++j) {
			Enemy* enemy = enemyManager.GetEnemy(j);
			//衝突処理
			DirectX::XMFLOAT3 outPosition;
			if (Collision::IntersctSphereVsCylinder(
				projectile->GetPosition(),
				projectile->GetRadius(),
				enemy->GetPosition(),
				enemy->GetRadius(),
				enemy->GetHeight(),
				outPosition)
				)
			{
				//ダメージを与える
				if (enemy->ApplyDamage(Get_Attack(), 0.1f))
				{
					//吹き飛ばす
					/*DirectX::XMFLOAT3 impulse;
					impulse.x = sinf(angle.y) * 0.5f;
					impulse.y = 0.0f;
					impulse.z = cosf(angle.y) * 0.5f;
					enemy->AddImpulse(impulse);*/

					//ヒットエフェクト再生
					DirectX::XMFLOAT3 e = enemy->GetPosition();
					e.y += enemy->GetHeight() * 0.5f;
					hitEffect->Play(e);
					enemy_hit_sound->DC_Play();

					//弾丸破棄
					projectile->Destroy();
				}
			}
		}
	}
}

void Player::Input_Change()
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::Change)
	{
		swap_sound->DC_Play();
		change_voice->Play(false);
		PlayerManager::Instance().Player_Swap();
	}
}

void Player::Camera_Shake()
{
	GameParameter::Instance().SetCameraShake(cameraShakeFrame, cameraShakeRange);
}

bool Player::InputAttack()
{
	/*GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_B)
	{
		return true;
	}*/
	return false;
}

bool Player::Cheack_Position()
{
	return true;
}