#pragma once

#include"Graphics/Sprite.h"
#include"Audio/Audio.h"
#include"Scene.h"
#include"CameraController.h"
#include<thread>
#include<memory>

//���[�h�f�B���O�V�[��
class SceneLoading :public Scene
{
public:
	SceneLoading(Scene*nextScene):nextScene(nextScene){}
	~SceneLoading() override{}

	// ������
	void Initialize() override;

	// �I����
	void Finalize() override;

	// �X�V����
	void Update(float elapsedTime) override;

	// �`�揈��
	void Render() override;
private:
	//���[�f�B���O�X���b�h
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