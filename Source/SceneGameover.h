#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include"CameraController.h"
#include"Audio/Audio.h"

//�N���A�V�[��
class SceneGameover : public Scene
{
public:
    SceneGameover() {};
    ~SceneGameover() {};

    //������
    virtual void Initialize() override;

    //�I����
    virtual void Finalize() override;

    //�X�V����
    virtual void Update(float elapsedTime) override;

    //�`�揈��
    virtual void Render() override;

private:
    void Scene_Change();
    void Update_intarval(float elapsedTime);
private:
    Sprite* gameover = nullptr;
    bool effect = false;
    float intarval = 20.0f;
    int voice_play_count = 0;
    CameraController* cameraController = nullptr;
    std::unique_ptr<AudioSource> bgm;
    std::unique_ptr<AudioSource> sceneChange;
    std::unique_ptr<AudioSource>lose_voice_1;
    std::unique_ptr<AudioSource>lose_voice_2;
    std::unique_ptr<AudioSource>lose_voice_3;
    std::unique_ptr<AudioSource>lose_voice_4;
};