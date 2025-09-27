#include"Enemy.h"
#include"Graphics/Graphics.h"
#include"EnemyManager.h"

// ��]����
void Enemy::Turn(float elapsedTime, float vx, float vz, float speed)
{
	speed *= elapsedTime;
	//�i�s�x�N�g�����[���x�N�g���̏ꍇ�͏�������K�v����
	if (vx == 0.0f && vz == 0.0f)return;
	float lenth = sqrtf(vx * vx + vz * vz);

	//�i�s�x�N�g����P�ʃx�N�g����
	vx /= lenth;
	vz /= lenth;

	//���g�̉�]�l����O���������߂�
	float frontX = -sinf(angle.y);
	float frontZ = -cosf(angle.y);

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
		angle.y += rot;
	}
	else
	{
		angle.y -= rot;
	}
}

//�f�o�b�O�v���~�e�B�u�`��
void Enemy::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	//�Փ˔���p�̃f�o�b�O����`��
	//debugRenderer->DrawSphere(position, radius, DirectX::XMFLOAT4(0, 0, 0, 1));
	//�Փ˗p����p�̃f�o�b�O�~����`��
	debugRenderer->DrawCylinder(position, radius, height, DirectX::XMFLOAT4(0, 0, 0, 1));
}

// �f�o�b�O�pGUI�`��
void Enemy::DrawDebugGui()
{
    // �ʒu
    ImGui::InputFloat3("Position", &position.x);
    // ���x
    ImGui::InputFloat3("Velocity", &velocity.x);
    // ���a
    ImGui::InputFloat("Radius", &radius);
    // ����
    ImGui::InputFloat("Height", &height);
    // ���N
    ImGui::InputFloat("Health", &health);
}

//�j��
void Enemy::Destroy() {
	EnemyManager::Instance().Remove(this);
}