#include "EnemyBoss.h"
#include "ProjectileStraight.h"
#include "ProjectileHoming.h"
#include "EnemyManager.h"
#include "playerManager.h"
#include "SceneGame.h"
#include"SceneManager.h"

/* EnemyBoss */
EnemyBoss::EnemyBoss()
{
    model = new Model("Data/Model/boss/boss.mdl");

    // �X�P�[�����O
    scale.x = scale.y = scale.z = 8.0f;
    radius = 9.0f;
    height = 10.0f;

    health = 100;
}

EnemyBoss::~EnemyBoss()
{
    delete model;
}

void EnemyBoss::Update(float elapsedTime)
{
    DecidedMove(elapsedTime);

    // ���͍X�V
    UpdateVelocity(elapsedTime);

    // �I�u�W�F�N�g�s��X�V
    UpdateTransform();

    // ���f���s��X�V
    model->UpdateTransform(transform);
}

void EnemyBoss::Render(ID3D11DeviceContext* dc, Shader* shader)
{
    if (!isDraw) return;

    shader->Draw(dc, model);
}

void EnemyBoss::DrawDebugPrimitive()
{
    Enemy::DrawDebugPrimitive();
}

void EnemyBoss::OnDead()
{
    SceneManager::Instance().GetCurrentScene()->isGameClear = true;
    PlayerManager::Instance().getCurrentPlayer()->victory_voice->Play(false);
    // ���g��j��
    Destroy();
}

void EnemyBoss::UpdateShootingTimer(float elapsedTime)
{
    if (shootingTimer > 0.0f)
        shootingTimer -= elapsedTime;
    else shootingTimer = SHOOTING_TIME;
}

//void EnemyBoss::OnDamaged()
//{
//}

void EnemyBoss::DecidedMove(float elapsedTime)
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
        if (timer >= 2.0f)
        {
            state = nextState;
            timer = 0.0f;
            stopCount++;
        }

        break;
    }
    case State::SHOT_BOTH_HANDS:
    {
        ShotBothHands(elapsedTime);
        break;
    }
    case State::SHOT_CENTER:
    {
        ShotCenter(elapsedTime);
        break;
    }
    case State::SHOT_RANDOM:
    {
        ShotRandom(elapsedTime);
        break;
    }
    case State::RIZE:
        float vx = 0.0f;
        float vz = 1.0f;
        float dist = sqrtf(vx * vx + vz * vz);
        vx /= dist;
        vz /= dist;

        Move(vx, vz, moveSpeed);
        Turn(elapsedTime, vx, vz, turnSpeed * 0.3f);

        if (position.z>40.0) {
            if (!voice_flag) {
                PlayerManager::Instance().getCurrentPlayer()->boss_away_voice->Play(false);
                voice_flag = true;
            }
            SceneManager::Instance().GetCurrentScene()->isGameover = true;
        }

        break;
    }
}

void EnemyBoss::ShotBothHands(float elapsedTime)
{
    SHOOTING_TIME = 0.2f;
    UpdateShootingTimer(elapsedTime);

    // �e����(7����)
    if (shootingTimer < 0.0f)
    {
        for (int i = 0; i < 7; i++)
        {
            // ���˕���
            DirectX::XMFLOAT3 dir;
            dir.x = -sinf(handsShotAngles[i]);
            dir.y = 0;
            dir.z = -cosf(handsShotAngles[i]);
            if (dir.z >= 0.9f && dir.z <= 1.1f)
            {
                int a = 0;
            }
            //�@���ˈʒu
            DirectX::XMFLOAT3 pos = GetPosition();
            pos.x += 21.0f;
            pos.y = pos.y;
            pos.z += -9.0f;

            // ����(�e����)
            ProjectileStraight* projectile_right = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
            projectile_right->Launch(dir, pos);

            //�@���ˈʒu
            pos = GetPosition();
            pos.x += -21.0f;
            pos.y = pos.y;
            pos.z += -9.0f;

            // ����(�e����)
            ProjectileStraight* projectile_left = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
            projectile_left->Launch(dir, pos);
        }

        shotCount++;
    }

    // 10�A��������x�e
    if (shotCount >= 10)
    {
        timer = 0;
        shotCount = 0;
        state = State::STOP;
        if (stopCount < 5)
        {
            nextState = State::SHOT_BOTH_HANDS;
        }
        else
        {
            nextState = State::SHOT_CENTER;
            stopCount = -1;
        }
    }
}

void EnemyBoss::ShotCenter(float elapsedTime)
{
    SHOOTING_TIME = 0.5f;
    UpdateShootingTimer(elapsedTime);

    timer += elapsedTime;

    // �e����(4����)
    if (shootingTimer < 0.0f)
    {
        for (int i = 0; i < 4; i++)
        {
            // ���˕���
            DirectX::XMFLOAT3 dir;
            dir.x = -sinf(centerShotAngles[i]);
            dir.y = 0;
            dir.z = -cosf(centerShotAngles[i]);

            //�@���ˈʒu
            DirectX::XMFLOAT3 pos = GetPosition();
            pos.x = pos.x;
            pos.y = pos.y;
            pos.z += -7.0f;

            // ����(�e����)
            ProjectileStraight* projectile_right = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
            projectile_right->Launch(dir, pos);
        }
    }

    // 10�b��������x�e
    if (timer >= 10.0f)
    {
        timer = 0;
        shotCount = 0;
        state = State::STOP;
        nextState = State::SHOT_RANDOM;
    }
}

void EnemyBoss::ShotRandom(float elapsedTime)
{
    SHOOTING_TIME = 0.05f;
    UpdateShootingTimer(elapsedTime);

    timer += elapsedTime;

    // �e����(�����_������)
    if (shootingTimer < 0.0f)
    {
        // ���˕���
        int randAngle = rand() % 360;
        DirectX::XMFLOAT3 dir = {};
        dir.x = -sinf(static_cast<float>(randAngle));
        dir.y = 0;
        dir.z = -cosf(static_cast<float>(randAngle));

        //�@���ˈʒu
        DirectX::XMFLOAT3 pos = GetPosition();
        pos.x = pos.x;
        pos.y = pos.y;
        pos.z += -7.0f;

        // ����(�e����)
        ProjectileStraight* projectile_right = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
        projectile_right->Launch(dir, pos);
    }

    // 10�b��������x�e
    if (timer >= 10.0f)
    {
        timer = 0;
        shotCount = 0;
        state = State::STOP;
        nextState = State::RIZE;
    }
}