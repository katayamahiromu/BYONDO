#include"Character.h"
#include"Mathf.h"
#include "Graphics/Graphics.h"

//�X�V�s��
void Character::UpdateTransform() {
	//�X�P�[���s����쐬
	DirectX::XMMATRIX S = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	//��]�s����쐬

	DirectX::XMMATRIX X = DirectX::XMMatrixRotationX(angle.x);
	DirectX::XMMATRIX Y = DirectX::XMMatrixRotationY(angle.y);
	DirectX::XMMATRIX Z = DirectX::XMMatrixRotationZ(angle.z);
	DirectX::XMMATRIX R = Y * X * Z;

	//�ʒu�s����쐬
	DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	//�R�̍s���g�ݍ��킹�A���[���h�s����쐬
	DirectX::XMMATRIX W = S * R * T;
	//�v�Z�������[���h�����o��
	DirectX::XMStoreFloat4x4(&transform, W);
}

//�ړ�����
void Character::Move(float vx, float vz, float speed) {
	//�������x�N�g����ݒ�
	moveVecX = vx;
	moveVecZ = vz;
	//�ő呬�x�ݒ�
	maxMoveSpeed = speed;
}

//���񏈗�
void Character::Turn(float elapsedTime, float vx, float vz, float speed) {
	speed *= elapsedTime;
	//����͂��Ȃ��ꍇ�͐����ɖ߂�
	if (vx == 0.0f)
	{
		if (angle.z == 0.0f)return;

		if (angle.z > 0.0f)
		{
			angle.z += DirectX::XMConvertToRadians(80) * speed;
			if (angle.z < 0.0f) {
				angle.z = 0.0f;
			}
		}

		if (angle.z < 0.0f)
		{
			angle.z -= DirectX::XMConvertToRadians(80) * speed;
			if (angle.z > 0.0f) {
				angle.z = 0.0f;
			}
		}

		return;
	}
	float lenth = sqrtf(vx * vx + vz * vz);

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= lenth;
	vz /= lenth;

	//���g�̉�]�l����O���������߂�
	float frontX = sinf(angle.z);
	float frontZ = cosf(angle.z);

	//��]�p�����߂邽�߁A�Q�̒P�ʃx�N�g���̓��ς��v�Z����
	float dot = (vx * frontX) + (vz * frontZ); //����

	//���ϒl��-1.0~1.0�ŕ\������Ă���A�Q�̒P�ʃx�N�g���̊p�x��
	//�������ق�1.0�ɋ߂Â��Ƃ��������𗘗p���ĉ�]���x�𒲐�����
	float rot = 1.0f - dot; //�␳�l
	if (rot > speed) rot = speed;

	//���E������s�����߂ɂQ�̒P�ʃx�N�g���̊O�ς��v�Z����
	float cross = (vz * frontX) - (vx * frontZ);

	//2D�̊O�ϒl�����̏ꍇ�����̏ꍇ�ɂ���č��E������s����
	//���E������s�����ɂ���č��E��]��I������
	if (cross < 0.0f)
	{
		angle.z += rot;
	}
	else
	{
		angle.z -= rot;
	}

	//��]�̐���
	if (angle.z > DirectX::XMConvertToRadians(40))
	{
		angle.z = DirectX::XMConvertToRadians(40);
	}

	if (angle.z < DirectX::XMConvertToRadians(-40))
	{
		angle.z = DirectX::XMConvertToRadians(-40);
	}
	
}

//�W�����v����
void Character::Junp(float speed) {
	//������̗͂�ݒ�
	velocity.y = speed;
}

//�Ռ���^����
void Character::AddImpulse(const DirectX::XMFLOAT3& impulse)
{
	//���͂�͂ɉ�����
	velocity.x += impulse.x;
	velocity.y += impulse.y;
	velocity.z += impulse.z;
}

//���͏����X�V
void Character::UpdateVelocity(float elapsedTime) {
	//�o�߃t���[��
	float elapsedFrame = 60.0f * elapsedTime;

	//�������͍X�V����
	//UpdateVerticalVelocity(elapsedFrame);
	//�������͍X�V����
	UpdateHorizontalVelocity(elapsedFrame);
	//�����ړ��X�V����
	//UpdateVerticalMove(elapsedTime);
	//�����ړ��X�V����
	UpdateHorizontalMove(elapsedTime);
}

bool Character::ApplyDamage(float damage,float invincibleTime)
{
	//�_���[�W���O�̏ꍇ�͌��N��Ԃ�ύX����K�v���Ȃ�
	if (damage == 0.0f) return false;
	//���S���Ă���ꍇ�͌��N��Ԃ�ύX���Ȃ�
	if (health <= 0) return false;
	//�_���[�W����
	if (invincibleTime > 0.0f) {
		health -= damage;
		//���S�ʒm
		if (health <= 0.0f) {
			OnDead();
		}
		//�_���[�W�ʒm
		else {
			OnDamaged();
			invicibleTime = invincibleTime;
		}
	}
	//���N��Ԃ��ύX�����ꍇ��true��Ԃ�
	return true;
}

void Character::UpdateInvinciblTImer(float elapsedTime)
{
	if (invicibleTime > 0.0f) {
		invicibleTime -= elapsedTime;
	}
}

// ���S����
void Character::CheckDeath()
{
	// �X�N���[���̕`��͈͂���傫������Ă�����E��
	//if (screenPosition.x <= -100.0f || screenPosition.x >= Graphics::Instance().GetScreenWidth() + 100.0f
	//	|| screenPosition.y <= -100.0f || screenPosition.y >= Graphics::Instance().GetScreenHeight() + 100.0f)
	//{
	//	OnDead();
	//}

	if (position.x <= -100.0f || position.x >= 70.0f
		|| position.z <= -100.0f || position.z >= 100.0f)
	{
		OnDead();
	}
}

// �`�攻��
void Character::CheckDraw()
{
	// �X�N���[���̕`��͈͂���o�Ă���Ώ���
	//if (screenPosition.x <= 140 + radius * 2 || screenPosition.x >= 860 - radius * 2
	//	|| screenPosition.y <= -100.0f || screenPosition.y >= Graphics::Instance().GetScreenHeight() + 100.0f)
	//{
	//	isDraw = false;
	//}
	//else isDraw = true;

	//if (position.x <= -52.0f + radius || position.x >= 21.0f - radius
	//	|| position.z <= -50.0f || position.z >= 50.0f)
	//{
	//	isDraw = false;
	//}
	//else isDraw = true;

	isDraw = true;
}

// �s���͈͐���
bool Character::MoveLimit(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	DirectX::XMFLOAT2 screenPosition;
	screenPosition = ToScreenCoordinate(dc, view, projection);

	if (screenPosition.y < 0.0f)
	{
		move_limit = MOVE_LIMIT::UP;
		return false;
	}
	
	if (screenPosition.y > 720.0f)
	{
		move_limit = MOVE_LIMIT::DAWN;
		return false;
	}
	if (screenPosition.x > 800.0f)
	{
		move_limit = MOVE_LIMIT::RIGHT;
		return false;
	}

	if (screenPosition.x < 100.0f)
	{
		move_limit = MOVE_LIMIT::LEFT;
		return false;
	}

	move_limit = MOVE_LIMIT::NONE;
	return true;
}


DirectX::XMFLOAT2 Character::ToScreenCoordinate(ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	// �r���[�|�[�g
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	// �ϊ��s��
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	// ���[���h���W����X�N���[�����W�֕ϊ�
	DirectX::XMFLOAT3 wP = position;
	wP.y += height;
	DirectX::XMVECTOR WorldPosition = DirectX::XMLoadFloat3(&wP);
	float ViewportX = viewport.TopLeftX;
	float ViewportY = viewport.TopLeftY;
	float ViewportWidth = viewport.Width;
	float ViewportHeight = viewport.Height;
	float ViewportMinZ = 0.0f;
	float ViewportMaxZ = 1.0f;

	DirectX::XMVECTOR ScreenPosition;
	ScreenPosition = DirectX::XMVector3Project(
		WorldPosition,
		ViewportX,
		ViewportY,
		ViewportWidth,
		ViewportHeight,
		ViewportMinZ,
		ViewportMaxZ,
		Projection,
		View,
		World);

	DirectX::XMStoreFloat2(&screenPosition, ScreenPosition);

	return screenPosition;
}

//�������͍X�V����
void Character::UpdateHorizontalVelocity(float elapsedFrame)
{
	//XZ���ʂ̑��͂���������
	float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
	if (length > 0.0f) 
	{
		//���C��
		float friction = this->friction * elapsedFrame;
		//�󒆂ɂ���Ƃ��͖��C�͂����炷
		//if (!IsGround()) friction -= this->airControl;

		//���C�ɂ�鉡�����̌�������
		if(length > friction)
		{
			float vx = velocity.x / length;
			float vz = velocity.z / length;
			velocity.x -= vx * friction;
			velocity.z -= vz * friction;
		}
		//�������̑��͂����C�͈ȉ��ɂȂ����̂ő��͂𖳌���
		else
		{
			velocity.x = 0;
			velocity.z = 0;
		}
	}

	//XZ���ʂ̑��͂���������
	if (length <= maxMoveSpeed)
	{
		//�ړ��x�N�g�����[���x�N�g���łȂ��Ȃ��������
		float moveVecLength = sqrtf((moveVecX * moveVecX) + (moveVecZ * moveVecZ));
		if(moveVecLength > 0.0f)
		{
			//������
			float acceleration = this->acceleration * elapsedFrame;
			////�󒆂ɂ���Ƃ��͉����͂����炷
			//if (!IsGround()) acceleration -= this->airControl;
			////�ړ��x�N�g���ɂ���������
			velocity.x += moveVecX * acceleration;
			velocity.z += moveVecZ * acceleration;

			//�ő呬�x����
			float length = sqrtf((velocity.x * velocity.x) + (velocity.z * velocity.z));
			if (length > maxMoveSpeed)
			{
				velocity.x *= maxMoveSpeed / length;
				velocity.z *= maxMoveSpeed / length;
			}
		}

		////����K�^�K�^���Ȃ��悤�ɂ���
		//if (IsGround() && slopeRate > 0.0f)
		//{
		//	velocity.y -= length * slopeRate;
		//}
	}
	//�ړ��x�N�g�������Z�b�g
	moveVecX = 0.0f;
	moveVecZ = 0.0f;
}

//�����ړ��X�V����
void Character::UpdateHorizontalMove(float elapsedTime)
{
	//�ړ�����
	position.x += velocity.x * elapsedTime;
	position.z += velocity.z * elapsedTime;
}