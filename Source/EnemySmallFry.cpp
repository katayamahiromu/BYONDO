#include "EnemySmallFry.h"
#include "playerManager.h"

/* BaseEnemySmallFry */
BaseEnemySmallFry::~BaseEnemySmallFry()
{
    delete model;
}

void BaseEnemySmallFry::Update(float elapsedTime)
{
    DecidedMove(elapsedTime);

    // ���͍X�V
    UpdateVelocity(elapsedTime);

    // �I�u�W�F�N�g�s��X�V
    UpdateTransform();

    // ���f���s��X�V
    model->UpdateTransform(transform);
}

void BaseEnemySmallFry::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    if (!isDraw) return;

    shader->Draw(dc, model);
}

void BaseEnemySmallFry::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
}

void BaseEnemySmallFry::OnDead()
{
    // ���g��j��
    Destroy();
}

/* EnemySmallFry0 : �~�T�C�����i */
EnemySmallFry0::EnemySmallFry0()
{
    model = new Model("Data/Model/misairu/misairu.mdl");

    // �T�C�Y����
    scale.x = scale.y = scale.z = 1.0f;
    radius = 1.0f;
    height = 1.0f;

    // �̗�
    health = 2;
    
    // �����ݒ�
    state = State::DESCENT;
    timer = 0;
    targetPosition = {};
    straightVec = {};
}

//void EnemySmallFry0::Spin(float elapsedTime, float vx, float vz, float speed)
//{
//
//}

void EnemySmallFry0::DecidedMove(float elapsedTime)
{
    switch (state)
    {
    case State::DESCENT:
    {
        float vx = 0.0f;
        float vz = -1.0f;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);
        //angle.z += turnSpeed * elapsedTime;

        timer += elapsedTime;
        if (timer >= 3.0f)
        {
            state = State::HOMING;
            timer = 0.0f;
        }

        break;
    }
    case State::HOMING:
    {
        // �ڕW�n�_���v���C���[�ʒu�ɐݒ�
        targetPosition = PlayerManager::Instance().getCurrentPlayer()->GetPosition();

        // �ڕW�n�_�ֈړ�
        float vx = targetPosition.x - position.x;
        float vz = targetPosition.z - position.z;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed * 0.5f);
        Turn(elapsedTime, vx, vz, turnSpeed * 0.5f);
        //angle.z += turnSpeed * elapsedTime;

        // �^�C�}�[����
        timer += elapsedTime;
        if (timer >= 13.0f)
        {
            state = State::STRAIGHT;
            timer = 0.0f;
            straightVec = { vx, 0, vz };
        }

        break;
    }
    case State::STRAIGHT:
    {
        float vx = straightVec.x;
        float vz = straightVec.z;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);

        break;
    }
    }
}

/* EnemySmallFry1 */
EnemySmallFry1::EnemySmallFry1()
{
    model = new Model("Data/Model/teki1/teki1.mdl");

    // �T�C�Y����
    scale.x = scale.y = scale.z = 2.0f;
    radius = 1.0f;
    height = 1.0f;

    // �̗�
    health = 2;
}

void EnemySmallFry1::DecidedMove(float elapsedTime)
{
    const float sinYofs = 10.0f;

    switch (state)
    {
    case State::DESCENT:
        position.z += sinYofs;

        state = State::HOMING;
        /*fallthrough*/
    case State::HOMING:
        float vx = sinf(timer / 30.0f);
        float vz = -0.7f;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);
        Turn(elapsedTime, vx, vz, turnSpeed);
    }
    timer++;
}

/* EnemySmallFry2 */
EnemySmallFry2::EnemySmallFry2()
{
    //model = new Model("Data/Model/teki1/teki1.mdl");
    model = new Model("Data/Model/boss/boss.mdl");

    // �T�C�Y����
    scale.x = scale.y = scale.z = 5.0f;
    radius = 7.0f;
    height = 10.0f;

    // �̗�
    health = 1;
}

void EnemySmallFry2::DecidedMove(float elapsedTime)
{
    const float sinYofs = 50.0f;

    switch (state)
    {
    case State::DESCENT:
        position.z += sinYofs;

        state = State::HOMING;
        /*fallthrough*/
    case State::HOMING:
        float vx = -cosf(timer / 200.0f) * 5;
        float vz = -sinf(timer / 200.0f) * 5;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);
        Turn(elapsedTime, vx, vz, turnSpeed);
    }
    timer++;
}
