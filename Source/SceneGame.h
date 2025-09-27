#pragma once
#include"CameraController.h"
#include"Scene.h"
#include"Graphics/Sprite.h"
#include"Player.h"
#include"UIManager.h"
#include"Graphics/Graphics.h"
#include"Audio/Audio.h"
#include<memory>
#include"Stage.h"


// �Q�[���V�[��
class SceneGame:public Scene
{
public:
	~SceneGame() override {}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;
	
	Bomb_Gauge* Get_Bomb_Gauge(){ return bomb_gauge; }
private:
	//�G�l�~�[HP�Q�[�W�`��
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	//�Q�[���N���A
	void GameClear();
	//�Q�[���I�[�o�[
	void Gameover();
private:
	std::unique_ptr<AudioSource> BGM;
	CameraController* cameraController = nullptr;
	Sprite* gauge = nullptr;
	Sprite* UI_Base = nullptr;
	Sprite* separator_left = nullptr;
	Sprite* separator_right = nullptr;
	float screenHeight = static_cast<float>(Graphics::Instance().GetScreenHeight());
	Effect* sceneInEffect = nullptr;
	Effect* sceneEndEffect = nullptr;
	int anime_time;
	Bomb_Gauge* bomb_gauge = nullptr;
	bool effect = false;
	std::unique_ptr<AudioSource> sceneChange;
	int endState = 0;
	Stage* stage = nullptr;
};
