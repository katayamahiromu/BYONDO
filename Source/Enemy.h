#pragma once
#include"Graphics/Shader.h"
#include"Character.h"

//�G�l�~�[
class Enemy :public Character
{
public:
	~Enemy()override{}
	//�X�V����
	virtual void Update(float elapsedTime) = 0;
	//�`�揈��
	virtual void Render(ID3D11DeviceContext* dc, Shader* sheder) = 0;
	//��]����
	void Turn(float elapsedTime, float vx, float vz, float speed) override;
	//�f�o�b�O�v���~�e�B�u�`��
	virtual void DrawDebugPrimitive();
	// �f�o�b�O�pGUI�`��
	virtual void DrawDebugGui();
	//�j��
	void Destroy();
};