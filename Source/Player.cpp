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

//�C���X�^���X�擾
Player& Player::Instance()
{
	return *instace;
}

//�R���X�g���N�^
Player::Player(char* file_path) {
	//�C���X�^���X�|�C���^�擾
	instace = this;
	model = new Model(file_path);
	(file_path == "Data/Model/Player_Red/player_red.mdl") ? red = true : red = false;

	//�q�b�g�G�t�F�N�g�ǂݍ���
	hitEffect = new Effect("Data/Effect/Damage.efk");

	// �؂�ւ����̃G�t�F�N�g�ǂݍ���
	blueToRedEffect = new Effect("Data/Effect/Blue_to_red.efk");
	redToBlueEffect = new Effect("Data/Effect/Red_to_blue.efk");
	damageEffect = new Effect("Data/Effect/Damage.efk");

	//���y�ǂݍ���
	projectile_sound = Audio::Instance().LoadAudioSource("Data/Audio/player_gun.wav");
	projectile_sound->SetVolume(0.3f);
	swap_sound = Audio::Instance().LoadAudioSource("Data/Audio/playerChange.wav");
	hit_sound = Audio::Instance().LoadAudioSource("Data/Audio/player_hit.wav");
	enemy_hit_sound = Audio::Instance().LoadAudioSource("Data/Audio/enemy_hit.wav");
	enemy_hit_sound->SetVolume(0.5f);
	bomb_sound = Audio::Instance().LoadAudioSource("Data/Audio/bomb.wav");
}

//�f�X�g���N�^
Player::~Player() {
	delete model;
	delete hitEffect;
	delete blueToRedEffect;
	delete redToBlueEffect;
	delete damageEffect;

	projectileManager.Clear();
}

//�X�V����
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
	//���͍X�V
	UpdateVelocity(elapsedTime);
	//���G���ԍX�V
	UpdateInvinciblTImer(elapsedTime);
	//�v���C���[�ƓG�Ƃ̏Փ˔���
	CollisionPlayerVsEnemies();
	//�e�ۂƓG�̏Փ˔���
	CollisionProjectilesVsEnemies();
	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//�e�̓��͎�t
	Update_Input_Projectile();
	//�A�˂̃N�[������
	Update_Shooting_Timer(elapsedTime);
	//�؂�ւ����͎�t
	Input_Change();
	InputProjectile();
	//�e�۔��ˏ���
	projectileManager.Update(elapsedTime);
	//���f���A�j���[�V�������X�V
	model->UpdateAnimation(elapsedTime);
	//���f���s����X�V
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

//�ړ����͏���
bool Player::InputMove(float elapsedTime) {
	//�i�s�x�N�g���擾
	DirectX::XMFLOAT3 moveVec = GetMoveVec();
	//�ړ�����
	Move(moveVec.x, moveVec.z, moveSpeed);
	//���񏈗�
	Turn(elapsedTime, moveVec.x, moveVec.z, turnSpeed);

	return (moveVec.x != 0.0f || moveVec.z != 0.0f) ? true : false;
}

//�`�揈��
void Player::Render(ID3D11DeviceContext* dc, Shader* shader) {
	if (!isDraw) return;

	shader->Draw(dc, model);
	//�e�ە`�揈��
	projectileManager.Render(dc, shader);
}

void Player::DrawDebugGui() {
	if (ImGui::CollapsingHeader("Player"))
	{
		// �f�o�b�O�v���~�e�B�u�\��
		ImGui::Checkbox("VisibleDebugPrimitive", &visibleDebugPrimitive);
		// �ʒu
		ImGui::InputFloat3("Position", &position.x);
		// ��]
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// �ړ����x
		ImGui::InputFloat3("Velocity", &velocity.x);
		// �X�P�[��
		ImGui::InputFloat3("Scale", &scale.x);
		// ��]���x
		float turnDeg = DirectX::XMConvertToDegrees(turnSpeed);
		ImGui::InputFloat("TurnSpeed(Deg)", &turnDeg);
		turnSpeed = DirectX::XMConvertToRadians(turnDeg);
		// ���a
		ImGui::InputFloat("Radius", &radius);
		// ����
		ImGui::InputFloat("Height", &height);
		// ��
		ImGui::InputFloat("Health", &health);
		// ���C
		ImGui::InputFloat("Friction", &friction);
		// �����x
		ImGui::InputFloat("Acceleration", &acceleration);
		// �󒆐���W��
		ImGui::InputFloat("AirControl", &airControl);
	}

	// �e��
	projectileManager.DrawDebugGui();
}

//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
DirectX::XMFLOAT3 Player::GetMoveVec() const
{
	//���͏����擾 
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ax = gamePad.GetAxisLX();
	float ay = gamePad.GetAxisLY();

	//�J���������ƃX�e�B�b�N�̓��͒l�ɂ���Đi�s�������v�Z����
	Camera& camera = Camera::Instance();
	const DirectX::XMFLOAT3& cameraRight = camera.GetRight();
	const DirectX::XMFLOAT3& cameraFront = camera.GetFront();

	//�ړ��x�N�g����XZ���ʂɐ����ȃx�N�g���ɂȂ�悤�ɂ���

	//�J�����E�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraRightX = cameraRight.x;
	float cameraRightZ = cameraRight.z;
	float cameraRightLength = sqrtf(cameraRightX * cameraRightX + cameraRightZ * cameraRightZ);

	if(cameraRightLength > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraRightX /= cameraRightLength;
		cameraRightZ /= cameraRightLength;
	}

	//�J�����O�����x�N�g����XZ�P�ʃx�N�g���ɕϊ�
	float cameraFrontX = cameraFront.x;
	float cameraFrontZ = cameraFront.z;
	float cameraFrontLength = sqrtf(cameraFrontX * cameraFrontX + cameraFrontZ * cameraFrontZ);

	if (cameraFrontLength > 0.0f)
	{
		//�P�ʃx�N�g����
		cameraFrontX /= cameraFrontLength;
		cameraFrontZ /= cameraFrontLength;
	}

	//�X�e�b�N�̐������͒l���J�����E�����ɔ��f���A
	//�X�e�b�N�̐������͒l���J�����O�����ɔ��f���A
	//�i�s�x�N�g�����v�Z����
	DirectX::XMFLOAT3 vec;
	vec.x = cameraFrontX * ay + cameraRightX * ax;
	vec.z = cameraFrontZ * ay + cameraRightZ * ax;
	//Y�����͈ړ����Ȃ�
	vec.y = 0.0f;

	return vec;
}

void Player::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	////�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�Փ˗p����p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
	//projectileManager.DrawDebugPrimitive();

	//�U���Փ˗p�̍���m�[�h�̃f�o�b�O����`��
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
	//�S�Ă̓G�Ƒ�������ŏՓˏ���
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < enemyCount;++i) {
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMFLOAT3 outPosition;

		//�Փ˔���
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

//�_���[�W���󂯂����ɌĂ΂��
void Player::OnDamaged()
{
	hit_sound->DC_Play();
	damage_voice->DC_Play();
}

//���S�������ɌĂ΂��
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

//�e�ۓ��͏���
void Player::InputProjectile() 
{

	//���i�e�۔���
	if (intput_projectile && shooting_timer < 0.0f)
	{
		//�O����
		DirectX::XMFLOAT3 dir;
		dir.x = sinf(angle.y);
		dir.y = 0;
		dir.z = cosf(angle.y);
		//���ˈʒu�i�v���C���[�̍�������j
		DirectX::XMFLOAT3 pos = GetPosition();
		pos.x = GetPosition().x + right_projectile_offset;
		pos.y += 1;//��������̂��ɏC��

		//����
		ProjectileStraight* projectile_right;
		if (red)	projectile_right = new PRedProjectileStraight(&projectileManager);
		else projectile_right = new PBlueProjectileStraight(&projectileManager);
		projectile_right->Launch(dir, pos);

		//���ˈʒu�i�v���C���[�̍�������j
		pos.x = GetPosition().x + left_projectile_offset;

		//����
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
			//�����i���j
			Bomb_Gauge::Instance().Use_Flag();
			Bomb* bomb = new Bomb(&projectileManager);
			bomb_voice->DC_Play();
			bomb_sound->DC_Play();
		}
	}
}

//�e�ۂƓG�̏Փ˔���
void Player::CollisionProjectilesVsEnemies(){
	EnemyManager& enemyManager = EnemyManager::Instance();
	//�S�Ă̒e�ۂƑS�Ă̓G�𑍓�����ŏՓ˔���
	int projectileCount = projectileManager.GetProjectileCount();
	int enemyCount = enemyManager.GetEnemyCount();
	for (int i = 0;i < projectileCount;++i) {
		Projectile* projectile = projectileManager.GetProjectile(i);
		for (int j = 0;j < enemyCount;++j) {
			Enemy* enemy = enemyManager.GetEnemy(j);
			//�Փˏ���
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
				//�_���[�W��^����
				if (enemy->ApplyDamage(Get_Attack(), 0.1f))
				{
					//������΂�
					/*DirectX::XMFLOAT3 impulse;
					impulse.x = sinf(angle.y) * 0.5f;
					impulse.y = 0.0f;
					impulse.z = cosf(angle.y) * 0.5f;
					enemy->AddImpulse(impulse);*/

					//�q�b�g�G�t�F�N�g�Đ�
					DirectX::XMFLOAT3 e = enemy->GetPosition();
					e.y += enemy->GetHeight() * 0.5f;
					hitEffect->Play(e);
					enemy_hit_sound->DC_Play();

					//�e�۔j��
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