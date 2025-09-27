#include"SceneManager.h"

//�X�V����
void SceneManager::Update(float elapsedTime)
{
	if (nextScene != nullptr)
	{
		//�Â��V�[�����I��
		Clear();
		//�V�����V�[���̐ݒ�
		currentScene = nextScene;
		nextScene = nullptr;
		//�V�[���̏�����
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

//�`�揈��
void SceneManager::Render()
{
	//if (nextScene != nullptr)
	//{
	//	// �V�[���؂�ւ��G�t�F�N�g
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

//�V�[���N���A
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

//�V�[���؂�ւ�
void SceneManager::ChangeScene(Scene*scene)
{
	//�V�����V�[����ݒ�
	nextScene = scene;
}