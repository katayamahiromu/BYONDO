#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"
#include "Audio/Audio.h"

// ���ʂ̓G(�e�����Ă���)
class BaseEnemyNormal : public Enemy
{
public:
    ~BaseEnemyNormal() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �X�V����
    void Render(ID3D11DeviceContext* dc, Shader* shader) override;

    // �s��
    virtual void DecidedMove(float elapsedTime) = 0;

    // �f�o�b�O�v���~�e�B�u�`��
    void DrawDebugPrimitive() override;

protected:
    // �e���˃^�C�}�[�X�V
    void UpdateShootingTimer(float elapsedTime);

private:
    // ���S��
    void OnDead() override;

protected:
    Model* model = nullptr;
    float shootingTimer = 0.0f;
    float SHOOTING_TIME = 0.5f;
};

// 0 ���ˏ�(�g�Q)
class EnemyNormal0 : public BaseEnemyNormal
{
public:
    EnemyNormal0();
    void DecidedMove(float elapsedTime) override;

private:
    enum class State
    {
        DESCENT,    // �~��
        STOP,       // ��~
        SHOT,       // ���ˏ�ɒe�𔭎�
        END_STOP,   // ��~
        RIZE,       // �㏸
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

// 1 ���@�_��(�藠��)
class EnemyNormal1 : public BaseEnemyNormal
{
public:
    EnemyNormal1();
    void DecidedMove(float elapsedTime) override;

private:
    enum class State
    {
        DESCENT,    // �~��
        STOP,       // ��~
        SHOT,       // ���@�����ɒe�𔭎�
        END_STOP,   // ��~
        RIZE,       // �㏸
    };
    State state = State::DESCENT;
    DirectX::XMFLOAT3 targetPosition = { 0, 0, 0 };
    float moveSpeed = 8.0f;
    float slowTurnSpeed = DirectX::XMConvertToRadians(180);
    float fastTurnSpeed = DirectX::XMConvertToRadians(360);
    int stopCount = 0;
    int shotCount = 0;
};