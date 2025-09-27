#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

// 雑魚敵(弾撃たない)
class BaseEnemySmallFry : public Enemy
{
public:
    ~BaseEnemySmallFry() override;

    // 更新処理
    void Update(float elapsedTime) override;

    // 描画処理
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // 行動
    virtual void DecidedMove(float elapsedTime) = 0;

    // デバッグプリミティブ描画
    void DrawDebugPrimitive() override;

private:
    // 死亡時
    void OnDead() override;

protected:
    Model* model = nullptr;
};

// 0
class EnemySmallFry0 : public BaseEnemySmallFry
{
public:
    EnemySmallFry0();
    void DecidedMove(float elapsedTime) override;

private:
    //void Spin(float elapsedTime, float vx, float vz, float speed);

private:
    enum class State
    {
        DESCENT,    // 降下
        HOMING,     // 追尾
        STRAIGHT,   // 追尾解除後直進
    };
    State state = State::DESCENT;
    DirectX::XMFLOAT3 targetPosition = { 0, 0, 0 };
    DirectX::XMFLOAT3 straightVec = { 0, 0, 0 };
    float moveSpeed = 10.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
};

// 1
class EnemySmallFry1 : public BaseEnemySmallFry
{
public:
    EnemySmallFry1();
    void DecidedMove(float elapsedTime) override;

private:
    enum class State
    {
        DESCENT,    // 降下
        HOMING,     // 追尾
        STRAIGHT,   // 追尾解除後直進
    };
    State state = State::DESCENT;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
};

// 2
class EnemySmallFry2 : public BaseEnemySmallFry
{
public:
    EnemySmallFry2();
    void DecidedMove(float elapsedTime) override;

private:
    enum class State
    {
        DESCENT,    // 降下
        HOMING,     // 追尾
        STRAIGHT,   // 追尾解除後直進
    };
    State state = State::DESCENT;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
};