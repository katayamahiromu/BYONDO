#pragma once

#include"Graphics/Sprite.h"
#include"Scene.h"
#include"CameraController.h"
#include"Audio/Audio.h"

//クリアシーン
class SceneGameover : public Scene
{
public:
    SceneGameover() {};
    ~SceneGameover() {};

    //初期化
    virtual void Initialize() override;

    //終了化
    virtual void Finalize() override;

    //更新処理
    virtual void Update(float elapsedTime) override;

    //描画処理
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