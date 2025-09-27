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

//�v���C���[
class Player :public Character
{
public:
	Player(char*file_path);
	~Player() override;

	//�X�V����
	void Update(float elapsedTime);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader);

	//�f�o�b�O�pGUI�`��
	void DrawDebugGui();
	//�X�e�B�b�N���͒l����ړ��x�N�g�����擾
	DirectX::XMFLOAT3 GetMoveVec() const;
	//�ړ����͏���
	bool InputMove(float elapsedTime);
	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	//�v���C���[�ƃG�l�~�[�Ƃ̏Փ˔���
	void CollisionPlayerVsEnemies();
	//�e�ۓ��͏���
	void InputProjectile();
	//�e�ۂƓG�̏Փ˔���
	void CollisionProjectilesVsEnemies();
	//�U�����͏���
	bool InputAttack();
	//�C���X�^���X�擾
	static Player& Instance();
	//�؂�ւ���t
	void Input_Change();
	//�J�����V�F�C�N
	void Camera_Shake();
	//�s������
	bool Cheack_Position();
protected:
	//�_���[�W���󂯂����ɌĂ΂��
	void OnDamaged() override;
	//���S�������ɌĂ΂��
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