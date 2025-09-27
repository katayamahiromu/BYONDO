#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

// �G���G(�e�����Ȃ�)
class BaseEnemySmallFry : public Enemy
{
public:
    ~BaseEnemySmallFry() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // �s��
    virtual void DecidedMove(float elapsedTime) = 0;

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive() override;

private:
    // ���S��
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
        DESCENT,    // �~��
        HOMING,     // �ǔ�
        STRAIGHT,   // �ǔ������㒼�i
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
        DESCENT,    // �~��
        HOMING,     // �ǔ�
        STRAIGHT,   // �ǔ������㒼�i
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
        DESCENT,    // �~��
        HOMING,     // �ǔ�
        STRAIGHT,   // �ǔ������㒼�i
    };
    State state = State::DESCENT;
    float moveSpeed = 5.0f;
    float turnSpeed = DirectX::XMConvertToRadians(720);
};