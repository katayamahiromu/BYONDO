#pragma once

#include"Graphics/Model.h"
#include"Projectile.h"

//�����e��
class ProjectileStraight :public Projectile 
{
public:
	ProjectileStraight(ProjectileManager*manager);
	~ProjectileStraight() override;

	//�X�V����
	void Update(float elapsedTime) override;
	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* sheder) override;
	//����
	void Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position);
	//�����ړ�
	void UpdateHorizontalMove(float elapsedTime);
protected:
	Model* model = nullptr;
private:
	float speed = 20.0f;
	float lifeTimer = 10.0f;
	float destoroy_pos = 45.0f;
};

// �v���C���[��(�~���C)
class PBlueProjectileStraight : public ProjectileStraight
{
public:
	PBlueProjectileStraight(ProjectileManager* manager);
};

// �v���C���[��(�J�R)
class PRedProjectileStraight : public ProjectileStraight
{
public:
	PRedProjectileStraight(ProjectileManager* manager);
};

// �G
class EProjectileStraight : public ProjectileStraight
{
public:
	EProjectileStraight(ProjectileManager* manager);
};