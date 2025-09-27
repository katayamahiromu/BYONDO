#pragma once
#include <DirectXMath.h>

// �Q�[���p�����[�^�[
class GameParameter
{
private:
	GameParameter() = default;
	~GameParameter() = default;

public:
	// �C���X�^���X�擾
	static GameParameter& Instance()
	{
		static GameParameter instance;
		return instance;
	}

	// �X�V����
	void Update(float elapsedTime);

	// �J�����V�F�C�N�ݒ�
	void SetCameraShake(int frame, float range) { cameraShakeFrame = frame + 1; cameraShakeRange = range / cameraShakeFrame; }

	// �J�����V�F�C�N�I�t�Z�b�g�擾
	const DirectX::XMFLOAT3& GetCameraShakeOffset() const { return cameraShakeOffset; }

private:
	int					cameraShakeFrame = 0;
	float				cameraShakeRange = 0;
	DirectX::XMFLOAT3	cameraShakeOffset = { 0, 0, 0 };
};
