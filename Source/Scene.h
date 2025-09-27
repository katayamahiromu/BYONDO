#pragma once
#include "Effect.h"
#include "Audio/Audio.h"

//シーン
class Scene
{
public:
	Scene() {};
	virtual ~Scene() {};

	//初期化
	virtual void Initialize() = 0;

	//終了化
	virtual void Finalize() = 0;

	//更新処理
	virtual void Update(float elapsedTime) = 0;

	//描画処理
	virtual void Render() = 0;

	//準備が完了しているか
	bool IsReady()const { return ready; }

	//準備完了設定
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