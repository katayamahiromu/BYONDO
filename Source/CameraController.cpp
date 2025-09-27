#include <imgui.h>
#include"CameraController.h"
#include"Camera.h"
#include"Input/Input.h"
#include"GameParameter.h"

//�X�V����
void CameraController::Update(float elapsedTime) 
{
	//GamePad& gamePad = Input::Instance().GetGamePad();
	//float ax = gamePad.GetAxisRX();
	//float ay = gamePad.GetAxisRY();
	////�J�����̉�]���x
	//float speed = rollSpeed * elapsedTime;
	////�X�e�B�b�N�̓��͒l�ɂ��킹��X����Y������]
	//angle.x += ay * speed;
	//angle.y += ax * speed;
	////X���̃J������]�𐧌�
	//if (angle.x > maxAngleX) 
	//{
	//	angle.x = maxAngleX;
	//}
	//if (angle.x < mixAngleX)
	//{
	//	angle.x = mixAngleX;
	//}
	////Y���̉�]�l��-3.14 ~ 3.14�Ɏ��܂�悤�ɂ���
	//if (angle.y < -DirectX::XM_PI)
	//{
	//	angle.y += DirectX::XM_2PI;
	//}
	//if (angle.y > DirectX::XM_PI)
	//{
	//	angle.y -= DirectX::XM_2PI;
	//}

	//�J������]�l����]�s��ɕϊ�
	DirectX::XMMATRIX Transform = DirectX::XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);

	//��]�s�񂩂�O�����x�N�g�������o��
	DirectX::XMVECTOR Front = Transform.r[2];
	DirectX::XMFLOAT3 front;
	DirectX::XMStoreFloat3(&front, Front);

	//�����_������x�N�g�������Ɉ�苗�����ꂽ�J�������_�����߂�
	DirectX::XMFLOAT3 eye;
	eye.x = target.x - front.x * range;
	eye.y = target.y - front.y * range;
	eye.z = target.z - front.z * range;

	// �J�����V�F�C�N
	const DirectX::XMFLOAT3& cameraShakeOffset = GameParameter::Instance().GetCameraShakeOffset();
	target.x += cameraShakeOffset.x;
	//target.y += cameraShakeOffset.y;
	//target.z += cameraShakeOffset.z;

	//�J�����̎��_�ƒ����_��ݒ�
	Camera::Instance().SetLookAt(eye, target, DirectX::XMFLOAT3(0, 0, 1));
}

// �f�o�b�O�pGUI�`��
void CameraController::DrawDebugGui()
{
	if (ImGui::CollapsingHeader("CameraController", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// �p�x
		DirectX::XMFLOAT3 a;
		a.x = DirectX::XMConvertToDegrees(angle.x);
		a.y = DirectX::XMConvertToDegrees(angle.y);
		a.z = DirectX::XMConvertToDegrees(angle.z);
		ImGui::InputFloat3("Angle", &a.x);
		angle.x = DirectX::XMConvertToRadians(a.x);
		angle.y = DirectX::XMConvertToRadians(a.y);
		angle.z = DirectX::XMConvertToRadians(a.z);
		// �^�[�Q�b�g
		ImGui::InputFloat3("Target", &target.x);
		// ��]���x
		float rollSpeedDeg = DirectX::XMConvertToDegrees(rollSpeed);
		ImGui::InputFloat("RollSpeed(Deg)", &rollSpeedDeg);
		rollSpeed = DirectX::XMConvertToRadians(rollSpeedDeg);
		// �^�[�Q�b�g����J�����̋���
		ImGui::InputFloat("Range", &range);
		// ��]����
		float minAngleDeg = DirectX::XMConvertToDegrees(minAngleX);
		float maxAngleDeg = DirectX::XMConvertToDegrees(maxAngleX);
		ImGui::InputFloat("MinAngleX(deg)", &minAngleDeg);
		ImGui::InputFloat("MaxAngleX(deg)", &maxAngleDeg);
		minAngleX = DirectX::XMConvertToRadians(minAngleDeg);
		maxAngleX = DirectX::XMConvertToRadians(maxAngleDeg);
	}
}