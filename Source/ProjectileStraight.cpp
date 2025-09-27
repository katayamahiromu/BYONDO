#include"ProjectileStraight.h"

//�R���X�g���N�^
ProjectileStraight::ProjectileStraight(ProjectileManager* manager):Projectile(manager)
{
	//model = new Model("Data/Model/SpikeBall/SpikeBall.mdl");
	//model = new Model("Data/Model/Sword/Sword.mdl");
	//�\���T�C�Y�𒲐�
	//scale.x = scale.y = scale.z = 0.5f;
	//scale.x = scale.y = scale.z = 3.0f;
	int i = 0;
}

//�f�X�g���N�^
ProjectileStraight::~ProjectileStraight() {
	delete model;
}

//�X�V����
void ProjectileStraight::Update(float elapsedTime)
{
	//��������
	lifeTimer -= elapsedTime;
	if (lifeTimer < 0) {
		//�������폜
		Destroy();
	}
	if (position.z > destoroy_pos)
	{
		Destroy();
	}
	//�����ړ�����
	UpdateHorizontalMove(elapsedTime);

	//�I�u�W�F�N�g�s����X�V
	UpdateTransform();
	//���f���s��X�V
	model->UpdateTransform(transform);
}

//�`�揈��
void ProjectileStraight::Render(ID3D11DeviceContext*dc, Shader* shader)
{
	shader->Draw(dc, model);
}

void ProjectileStraight::Launch(const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT3& position)
{
	this->direction = direction;
	this->position = position;
}

void ProjectileStraight::UpdateHorizontalMove(float elapsedTime)
{
	float speed = this->speed * elapsedTime;
	float mx = direction.x * speed;
	float mz = direction.z * speed;

	//DirectX::XMFLOAT3 start = { position.x,position.y + stepOffset,position.z };
	//DirectX::XMFLOAT3 end = { position.x + mx,position.y + stepOffset,position.z + mz };
	//HitResult hit;
	//if (StageManager::Instance().RayCast(start,end,hit)) {

	//	//���˃x�N�g��
	//	DirectX::XMVECTOR Position = DirectX::XMLoadFloat3(&position);

	//	//�ǂ܂ł̃x�N�g��
	//	DirectX::XMVECTOR Start = DirectX::XMLoadFloat3(&hit.position);
	//	DirectX::XMVECTOR End = DirectX::XMLoadFloat3(&end);
	//	DirectX::XMVECTOR Vec = DirectX::XMVectorSubtract(End, Start);
	//	//�ǂ̖@��
	//	DirectX::XMVECTOR Normal = DirectX::XMLoadFloat3(&hit.normal);

	//	//���˃x�N�g����@���Ɏˉe
	//	DirectX::XMVECTOR Dot = DirectX::XMVector3Dot(Vec, Normal);

	//	//�␳�ʒu�̌v�Z
	//	float distance = 0;
	//	DirectX::XMStoreFloat(&distance, Dot);

	//	// ���˃x�N�g���̌v�Z
	//	DirectX::XMVECTOR Reflected = DirectX::XMVectorSubtract(Vec, DirectX::XMVectorScale(Normal, 2.0f * distance));

	//	//�@���̑傫���������ɍ��킹��
	//	DirectX::XMVECTOR R = DirectX::XMVectorAdd(Vec, DirectX::XMVectorScale(Dot,2));
	//	DirectX::XMFLOAT3 r; 
	//	DirectX::XMStoreFloat3(&r, R);

	//	Position = DirectX::XMVectorAdd(Position, DirectX::XMVectorScale(R,speed));
	//	DirectX::XMStoreFloat3(&position, Position);
	//	this->direction = DirectX::XMFLOAT3(DirectX::XMVectorGetX(Reflected), 0.0f, DirectX::XMVectorGetZ(Reflected));
	//}
	//else
	//{
		//�ړ�
	//}
	position.x += mx;
	position.z += mz;
}

// �v���C���[��(�~���C)
PBlueProjectileStraight::PBlueProjectileStraight(ProjectileManager* manager)
	: ProjectileStraight(manager)
{
	model = new Model("Data/Model/tama_ao/tama_ao.mdl");
	scale.x = scale.y = scale.z = 0.5f;
}

// �v���C���[��(�J�R)
PRedProjectileStraight::PRedProjectileStraight(ProjectileManager* manager)
	: ProjectileStraight(manager)
{
	model = new Model("Data/Model/tama_aka/tama_aka.mdl");
	scale.x = scale.y = scale.z = 0.5f;
}

// �G
EProjectileStraight::EProjectileStraight(ProjectileManager* manager)
	: ProjectileStraight(manager)
{
	model = new Model("Data/Model/teki_tama/teki_tama.mdl");
	//model = new Model("Data/Model/tama_ao/tama_ao.mdl");
	scale.x = scale.y = scale.z = 0.7f;	
}
