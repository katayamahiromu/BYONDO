#pragma once

#include"Graphics/Shader.h"
#include"Collision.h"

struct StageEnemy
{
	int time;
	int enemyType;
	DirectX::XMFLOAT3 position = {};
	bool appear;
};

//�X�e�[�W
class Stage
{
public:
	Stage(){}
	~Stage(){}

	//�X�V����
	void Update(float elapsedTime);

	void stageUpdate(float elapsedTime);

	void Clear();

private:
	int state = 0;
	float timer = 0;
	StageEnemy* enemy = nullptr;
};
