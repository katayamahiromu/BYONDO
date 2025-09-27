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


// ゲームシーン
class SceneGame:public Scene
{
public:
	~SceneGame() override {}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
	
	Bomb_Gauge* Get_Bomb_Gauge(){ return bomb_gauge; }
private:
	//エネミーHPゲージ描画
	void RenderEnemyGauge(
		ID3D11DeviceContext* dc,
		const DirectX::XMFLOAT4X4& view,
		const DirectX::XMFLOAT4X4& projection
	);

	//ゲームクリア
	void GameClear();
	//ゲームオーバー
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
