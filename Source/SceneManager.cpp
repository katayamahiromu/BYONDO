#include"SceneManager.h"

//更新処理
void SceneManager::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		//古いシーンを終了
		Clear();
		//新しいシーンの設定
		currentScene = nextScene;
		nextScene = nullptr;
		//シーンの初期化
		if (!currentScene->IsReady()) {
			currentScene->Initialize();
		}
		//isOnEffect = false;
	}


	if (currentScene != nullptr)
	{
		currentScene->Update(elapsedTime);
	}

}

//描画処理
void SceneManager::Render()
{
	//if (nextScene != nullptr)
	//{
	//	// シーン切り替えエフェクト
	//	if (!isOnEffect)
	//	{
	//		sceneChangeEffect->Play({ 0, 0, 0 }, 2.0f);
	//		isOnEffect = true;
	//	}
	//}

	if (currentScene != nullptr)
	{
		currentScene->Render();
	}
}

//シーンクリア
void SceneManager::Clear()
{
	if (currentScene != nullptr)
	{
		currentScene->Finalize();
		delete currentScene;
		currentScene = nullptr;
		//delete sceneChangeEffect;
	}
}

//シーン切り替え
void SceneManager::ChangeScene(Scene*scene)
{
	//新しいシーンを設定
	nextScene = scene;
}