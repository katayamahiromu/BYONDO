#include "EnemyNormal.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "EnemyManager.h"
#include "playerManager.h"

/* BaseEnemyNormal */
BaseEnemyNormal::~BaseEnemyNormal()
{
    delete model;
}

void BaseEnemyNormal::Update(float elapsedTime)
{
    DecidedMove(elapsedTime);

    // ���͍X�V
    UpdateVelocity(elapsedTime);

    // �I�u�W�F�N�g�s��X�V
    UpdateTransform();

    // ���f���s��X�V
    model->UpdateTransform(transform);

    if (position.z < -100)
    {
        OnDead();
    }
}

void BaseEnemyNormal::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    if (!isDraw) return;

    shader->Draw(dc, model);
}

void BaseEnemyNormal::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
}

void BaseEnemyNormal::OnDead()
{
    // ���g��j��
    Destroy();
}

void BaseEnemyNormal::UpdateShootingTimer(float elapsedTime)
{
    if (shootingTimer > 0.0f)
        shootingTimer -= elapsedTime;
    else shootingTimer = SHOOTING_TIME;
}

/* EnemyNormal0 ���ˏ�(�g�Q) */
EnemyNormal0::EnemyNormal0()
{
    model = new Model("Data/Model/toge/toge.mdl");

    // �T�C�Y����
    scale.x = scale.y = scale.z = 2.0f;
    radius = 1.0f;
    height = 1.0f;

    // �̗�
    health = 7;
    SHOOTING_TIME = 0.1f;
}

void EnemyNormal0::DecidedMove(float elapsedTime)
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

        timer += elapsedTime;
        if (timer >= 4.0f)
        {
            state = State::STOP;
            timer = 0.0f;
        }

        break;
    }
    case State::STOP:
    {
        timer += elapsedTime;
        if (timer >= 1.0f)
        {
            state = State::SHOT;
            timer = 0.0f;
            stopCount++;
        }

        break;
    }
    case State::SHOT:
    {
        UpdateShootingTimer(elapsedTime);

        // �e����(8����)
        if (shootingTimer < 0.0f)
        {
            for (int i = 0; i < 8; i++)
            {
                // ���˕���
                DirectX::XMFLOAT3 dir;
                dir.x = -sinf(angles[i]);
                dir.y = 0;
                dir.z = -cosf(angles[i]);

                //�@���ˈʒu
                DirectX::XMFLOAT3 pos = GetPosition();
                pos.x += -dir.x;
                pos.y = pos.y;
                pos.z += -dir.z;

                // ����(�e����)
                ProjectileStraight* projectile = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
                projectile->Launch(dir, pos);
            }

            shotCount++;

            // 5�A��������x�e
            if (shotCount >= 5)
            {
                (stopCount < 3) ? state = State::STOP : state = State::END_STOP;
                timer = 0;
                shotCount = 0;
            }
        }

        break;
    }
    case State::END_STOP:
    {
        float vx = 0.0f;
        float vz = 1.0f;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);
        Turn(elapsedTime, vx, vz, turnSpeed * 0.3f);

        break;
    }
    }
}

/* EnemyNormal1 ���@�_��(�藠��) */
EnemyNormal1::EnemyNormal1()
{
    model = new Model("Data/Model/shuriken/shuriken.mdl");

    // �T�C�Y����
    scale.x = scale.y = scale.z = 2.0f;
    radius = 1.0f;
    height = 1.0f;

    // �̗�
    health = 5;
    SHOOTING_TIME = 0.2f;
};

void EnemyNormal1::DecidedMove(float elapsedTime)
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
        angle.y += slowTurnSpeed * elapsedTime;

        timer += elapsedTime;
        if (timer >= 4.0f)
        {
            state = State::STOP;
            timer = 0.0f;
        }

        break;
    }
    case State::STOP:
    {
        angle.y += fastTurnSpeed * elapsedTime;

        timer += elapsedTime;
        if (timer >= 1.0f)
        {
            state = State::SHOT;
            timer = 0.0f;
            stopCount++;
        }

        break;
    }
    case State::SHOT:
    {
        UpdateShootingTimer(elapsedTime);
        angle.y += fastTurnSpeed * elapsedTime;

        // �e����(���@����)
        if (shootingTimer < 0.0f)
        {
            // ���˕���
            targetPosition = PlayerManager::Instance().getCurrentPlayer()->GetPosition();
            DirectX::XMFLOAT3 pos = GetPosition();
            DirectX::XMFLOAT3 dir;
            dir.x = targetPosition.x - position.x;
            dir.y = 0;
            dir.z = targetPosition.z - position.z;
            float dist = sqrtf(dir.x * dir.x + dir.z * dir.z);
            dir.x /= dist;
            dir.z /= dist;

            //�@���ˈʒu
            pos.x += -dir.x;
            pos.y = pos.y;
            pos.z += -dir.z;

            // ����(�e����)
            ProjectileStraight* projectile = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
            projectile->Launch(dir, pos);

            shotCount++;

            // 5�A��������x�e
            if (shotCount >= 5)
            {
                (stopCount < 3) ? state = State::STOP : state = State::END_STOP;
                timer = 0;
                shotCount = 0;
            }
        }

        break;
    }
    case State::END_STOP:
    {
        float vx = 0.0f;
        float vz = 1.0f;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);
        angle.y += slowTurnSpeed * elapsedTime;

        break;
    }
    }
}
