#pragma once

#include"Graphics/Sprite.h"
#include"Audio/Audio.h"
#include"Scene.h"
#include"CameraController.h"
#include<thread>
#include<memory>

//ロードディングシーン
class SceneLoading :public Scene
{
public:
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}

	// 初期化
	void Initialize() override;

	// 終了化
	void Finalize() override;

	// 更新処理
	void Update(float elapsedTime) override;

	// 描画処理
	void Render() override;
private:
	//ローディングスレッド
	static void LoadingThread(SceneLoading* scene);
private:
	float angle = 0.0f;
	Scene* nextScene = nullptr;
	std::thread* thread = nullptr;
	CameraController* cameraController = nullptr;
private:
	Sprite* explanation1 = nullptr;
	Sprite* explanation2 = nullptr;
	Sprite* explanation3 = nullptr;
private:
	int random = 0;
	int voice_random = 0;
private:
	std::unique_ptr<AudioSource> start_voice_mirai;
	std::unique_ptr<AudioSource> start_voice_kako;
};