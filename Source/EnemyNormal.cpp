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

    // 速力更新
    UpdateVelocity(elapsedTime);

    // オブジェクト行列更新
    UpdateTransform();

    // モデル行列更新
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
    // 自身を破棄
    Destroy();
}

void BaseEnemyNormal::UpdateShootingTimer(float elapsedTime)
{
    if (shootingTimer > 0.0f)
        shootingTimer -= elapsedTime;
    else shootingTimer = SHOOTING_TIME;
}

/* EnemyNormal0 放射状(トゲ) */
EnemyNormal0::EnemyNormal0()
{
    model = new Model("Data/Model/toge/toge.mdl");

    // サイズ調整
    scale.x = scale.y = scale.z = 2.0f;
    radius = 1.0f;
    height = 1.0f;

    // 体力
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

        // 弾撃つ(8方向)
        if (shootingTimer < 0.0f)
        {
            for (int i = 0; i < 8; i++)
            {
                // 発射方向
                DirectX::XMFLOAT3 dir;
                dir.x = -sinf(angles[i]);
                dir.y = 0;
                dir.z = -cosf(angles[i]);

                //　発射位置
                DirectX::XMFLOAT3 pos = GetPosition();
                pos.x += -dir.x;
                pos.y = pos.y;
                pos.z += -dir.z;

                // 発射(弾生成)
                ProjectileStraight* projectile = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
                projectile->Launch(dir, pos);
            }

            shotCount++;

            // 5連撃ったら休憩
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

/* EnemyNormal1 自機狙い(手裏剣) */
EnemyNormal1::EnemyNormal1()
{
    model = new Model("Data/Model/shuriken/shuriken.mdl");

    // サイズ調整
    scale.x = scale.y = scale.z = 2.0f;
    radius = 1.0f;
    height = 1.0f;

    // 体力
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

        // 弾撃つ(自機方向)
        if (shootingTimer < 0.0f)
        {
            // 発射方向
            targetPosition = PlayerManager::Instance().getCurrentPlayer()->GetPosition();
            DirectX::XMFLOAT3 pos = GetPosition();
            DirectX::XMFLOAT3 dir;
            dir.x = targetPosition.x - position.x;
            dir.y = 0;
            dir.z = targetPosition.z - position.z;
            float dist = sqrtf(dir.x * dir.x + dir.z * dir.z);
            dir.x /= dist;
            dir.z /= dist;

            //　発射位置
            pos.x += -dir.x;
            pos.y = pos.y;
            pos.z += -dir.z;

            // 発射(弾生成)
            ProjectileStraight* projectile = new EProjectileStraight(&EnemyManager::Instance().projectileManager);
            projectile->Launch(dir, pos);

            shotCount++;

            // 5連撃ったら休憩
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
