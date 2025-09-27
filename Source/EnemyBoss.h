#pragma once
#include "Graphics/Model.h"
#include "Enemy.h"

// �{�X�G
class EnemyBoss : public Enemy
{
public:
    EnemyBoss();
    ~EnemyBoss() override;

    // �X�V����
    void Update(float elapsedTime) override;

    //�`�揈��
    void Render(ID3D11DeviceContext* dc, Shader* shader)override;

    // �s��
    void DecidedMove(float elapsedTime);

    //�f�o�b�N�v���~�e�B�u�`��
    void DrawDebugPrimitive() override;

protected:
    void OnDead() override;
    //void OnDamaged() override;

    // �e���˃^�C�}�[�X�V
    void UpdateShootingTimer(float elapsedTime);

private:
    void ShotBothHands(float elapsedTime);
    void ShotCenter(float elapsedTime);
    void ShotRandom(float elapsedTime);

private:
    enum class State
    {
        DESCENT,            // �~��
        STOP,               // ��~
        SHOT_BOTH_HANDS,    // ���肩��e����
        SHOT_CENTER,        // ���S����4�����ɒe���ˁA�~�T�C��
        SHOT_RANDOM,        // �����_���ɒe���ˁA�藠��
        RIZE,               // �㏸
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