#pragma once
#include"Scene.h"
#include "Effect.h"

//シーンマネージャー
class SceneManager
{
private:
	SceneManager(){}
	~SceneManager(){}
public:
	//唯一のインスタンス取得
	static SceneManager& Instance()
	{
		static SceneManager instance;
		return instance;
	}

	//更新処理
	void Update(float elapsedTime);
	//描画処理
	void Render();
	//シーンクリア
	void Clear();
	//シーン切り替え
	void ChangeScene(Scene* scene);

	Scene* GetCurrentScene() { return currentScene; }
	Scene* GetNextScene() { return nextScene; }
private:
	Scene* currentScene = nullptr;
	Scene* nextScene = nullptr;
	//Effect* sceneChangeEffect = nullptr;
	//bool isOnEffect = false;
};
