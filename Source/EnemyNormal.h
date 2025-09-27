#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "Audio/Audio.h"

// 普通の敵(弾撃ってくる)
class BaseEnemyNormal : public Enemy
{
public:
    ~BaseEnemyNormal() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 更新処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // 行動
    virtual void DecidedMove(float elapsedTime) = 0;

    // デバッグプリミティブ描画
    void DrawDebugPrimitive() override;

protected:
    // 弾発射タイマー更新
    void UpdateShootingTimer(float elapsedTime);

private:
    // 死亡時
    void OnDead() override;

protected:
    Model* model = nullptr;
    float shootingTimer = 0.0f;
    float SHOOTING_TIME = 0.5f;
};

// 0 放射状(トゲ)
class EnemyNormal0 : public BaseEnemyNormal
{
public:
    EnemyNormal0();
    void DecidedMove(float elapsedTime) override;

private:
    enum class State
    {
        DESCENT,    // 降下
        STOP,       // 停止
        SHOT,       // 放射状に弾を発射
        END_STOP,   // 停止
        RIZE,       // 上昇
    };
    State state = State::DESCENT;
    float moveSpeed = 8.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
    int stopCount = 0;
    int shotCount = 0;
    float angles[8] = { DirectX::XMConvertToRadians(0), DirectX::XMConvertToRadians(45),
        DirectX::XMConvertToRadians(90), DirectX::XMConvertToRadians(135), DirectX::XMConvertToRadians(180), 
        DirectX::XMConvertToRadians(225), DirectX::XMConvertToRadians(270), DirectX::XMConvertToRadians(315) };
};

// 1 自機狙い(手裏剣)
class EnemyNormal1 : public BaseEnemyNormal
{
public:
    EnemyNormal1();
    void DecidedMove(float elapsedTime) override;

private:
    enum class State
    {
        DESCENT,    // 降下
        STOP,       // 停止
        SHOT,       // 自機方向に弾を発射
        END_STOP,   // 停止
        RIZE,       // 上昇
    };
    State state = State::DESCENT;
    DirectX::XMFLOAT3 targetPosition = { 0, 0, 0 };
    float moveSpeed = 8.0f;
    float slowTurnSpeed = DirectX::XMConvertToRadians(180);
    float fastTurnSpeed = DirectX::XMConvertToRadians(360);
    int stopCount = 0;
    int shotCount = 0;
};