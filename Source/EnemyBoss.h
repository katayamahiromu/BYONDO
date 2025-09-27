#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

// ボス敵
class EnemyBoss : public Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss() override;

    // 更新処理
    void Update(float elapsedTime) override;

    //描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    // 行動
    void DecidedMove(float elapsedTime);

    //デバックプリミティブ描画
    void DrawDebugPrimitive() override;

protected:
    void OnDead() override;
    //void OnDamaged() override;

    // 弾発射タイマー更新
    void UpdateShootingTimer(float elapsedTime);

private:
    void ShotBothHands(float elapsedTime);
    void ShotCenter(float elapsedTime);
    void ShotRandom(float elapsedTime);

private:
    enum class State
    {
        DESCENT,            // 降下
        STOP,               // 停止
        SHOT_BOTH_HANDS,    // 両手から弾発射
        SHOT_CENTER,        // 中心から4方向に弾発射、ミサイル
        SHOT_RANDOM,        // ランダムに弾発射、手裏剣
        RIZE,               // 上昇
    };

    State state = State::DESCENT;
    State nextState = State::SHOT_BOTH_HANDS;
    Model* model = nullptr;
    float shootingTimer = 0.0f;
    float SHOOTING_TIME = 0.5f;
    float moveSpeed = 3.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    int stopCount = 0;
    int shotCount = 0;

    float handsShotAngles[7] = { DirectX::XMConvertToRadians(45),
    DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(135), DirectX::XMConvertToRadians(0),
    DirectX::XMConvertToRadians(225), DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(315) };
    float centerShotAngles[4] = { DirectX::XMConvertToRadians(-50), DirectX::XMConvertToRadians(-15),
    DirectX::XMConvertToRadians(15), DirectX::XMConvertToRadians(50) };
    bool voice_flag = false;
};