#pragma once
#include"Graphics/Sprite.h"
#include"Graphics/Graphics.h"
class Back
{
public:
	Back(DirectX::XMFLOAT3);
	~Back();

	//�X�V����
	void Update(float elapsedTiem);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc);

private:
	float screenHeight = static_cast<float>(Graphics::Instance().GetScreenHeight());
	DirectX::XMFLOAT3 Back_Pos = {0, 0, 0};
	float shift = 0; //�ʒu�����炷
	Sprite* back = nullptr;
	const float speed = 100;	// �X�N���[�����x
	int state_ = 0;
};
