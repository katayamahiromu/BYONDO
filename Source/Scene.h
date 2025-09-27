#pragma once
#include "Effect.h"
#include "Audio/Audio.h"

//�V�[��
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	//������
	virtual void Initialize() = 0;

	//�I����
	virtual void Finalize() = 0;

	//�X�V����
	virtual void Update(float elapsedTime) = 0;

	//�`�揈��
	virtual void Render() = 0;

	//�������������Ă��邩
	bool IsReady()const { return ready; }

	//���������ݒ�
	void SetReady() { ready = true; }
public:
	bool ready = false;
public:
	bool isGameClear = false;
	bool isGameover = false;
public:
	int state_ = 0;
	int timer_ = 0;
	bool isOnEffect = false;
	Effect* sceneInEffect = nullptr;
	Effect* sceneEndEffect = nullptr;
};