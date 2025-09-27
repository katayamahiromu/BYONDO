#pragma once

#include<DirectXMath.h>
#include<Effekseer.h>
#include<EffekseerRendererDX11.h>
#include<memory.h>

//�G�t�F�N�g
class Effect
{
public:
	Effect(const char* filename);
	~Effect();

	//�Đ�
	Effekseer::Handle Play(const DirectX::XMFLOAT3& position, float scale = 1.0f);

	//��~
	void Stop(Effekseer::Handle handle);

	//�I�����Ă��邩
	bool IsPlay(Effekseer::Handle handle);

	//���W�ݒ�
	void SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position);

	//�X�P�[���ݒ�
	void SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale);

	Effekseer::Handle Get_Last_Handle();
	std::vector<Effekseer::Handle> Get_Handle_Manager() { return handle_Manager; }
private:
	Effekseer::EffectRef effekseerEffect;
	std::vector<Effekseer::Handle>handle_Manager;
};