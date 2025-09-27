#include "Stage.h"
#include "EnemyBoss.h"
#include "EnemyNormal.h"
#include "EnemySmallFry.h"
#include "EnemyManager.h"

enum
{
    MISSILE = 0, 
    SIN, 
    ROTATION, 
    TOGE, 
    SHURIKEN, 
    BOSS, 
};

StageEnemy stageEnemy[] = {
    {5, SIN, {-24, 0, 50}, false}, 
    {5, SIN, {14, 0, 50}, false},
    {7,MISSILE,{-7,0,50},false},
    {7,MISSILE,{7,0,50},false},
    {7,TOGE,{0,0,50},false},
    {10, SIN, {-24, 0, 50}, false},
    {10, SIN, {14, 0, 50}, false},
    {10, MISSILE, {0, 0, 50}, false},
    {10, MISSILE, {8, 0, 50}, false},
    {13,SIN,{5,0,50},false},
    {13,SIN,{-5,0,50},false},
    {13,SHURIKEN,{0,0,50},false},
    {15, SIN, {-24, 0, 50}, false},
    {15, MISSILE, {-10, 0, 50}, false},
    {15, MISSILE, {10, 0, 50}, false},
    {15, SIN, {14, 0, 50}, false},
    {15, SIN, {0, 0, 50}, false},
    {20, SIN, {-24, 0, 50}, false},
    {20, SIN, {14, 0, 50}, false},
    {20, SHURIKEN, {0, 0, 50}, false},
    {20, TOGE, {0, 0, 50}, false},
    {25, SIN, {-24, 0, 50}, false},
    {25, SIN, {14, 0, 50}, false},
    {30, MISSILE, {14, 0, 50}, false},
    {40, SHURIKEN, {14, 0, 50}, false},
    {40, SHURIKEN, {-24, 0, 50}, false},
    {45, SIN, {0, 0, 50}, false},
    {50, SIN, {10, 0, 50}, false},
    {55, SIN, {-15, 0, 50}, false},
    {57, SIN, {-24, 0, 50}, false},
    {57, SIN, {14, 0, 50}, false},
    {57, MISSILE, {0, 0, 50}, false},
    {60, SHURIKEN, {-24, 0, 50}, false},
    {60, SHURIKEN, {0, 0, 50}, false},
    {60, SHURIKEN, {14, 0, 50}, false},
    {70, TOGE, {-19, 0, 50}, false},
    {75, SIN, {24, 0, 50}, false},
    {75, MISSILE, {14, 0, 50}, false},
    {80, SIN, {4, 0, 50}, false},
    {80, MISSILE, {27, 0, 50}, false},
    {80, TOGE, {10, 0, 50}, false},
    {80, TOGE, {-20, 0, 50}, false},
    {85, SIN, {-4, 0, 50}, false},
    {85, MISSILE, {10, 0, 50}, false},
    {90, SIN, {14, 0, 50}, false},
    {90, MISSILE, {-7, 0, 50}, false},

    {90, BOSS, {0, 0, 50}, false},

    {95, SIN, {10, 0, 50}, false}, 
    {95, MISSILE, {-10, 0, 50}, false}, 
    {100, TOGE, {-15, 0, 50}, false},
    {100, TOGE, {15, 0, 50}, false},
    {105, SIN, {20, 0, 50}, false},
    {105, MISSILE, {-9, 0, 50}, false},
    {105, MISSILE, {9, 0, 50}, false},
    {105, MISSILE, {-14, 0, 50}, false},
    {105, MISSILE, {14, 0, 50}, false},
    {105, MISSILE, {24, 0, 50}, false},
    {105, MISSILE, {-24, 0, 50}, false},
    {105, SHURIKEN, {30, 0, 50}, false},
    {105, TOGE, {30, 0, 50}, false},
    {110, SIN, {-25, 0, 50}, false},
    {110, MISSILE, {20, 0, 50}, false},

    {-1, -1, {}, false},
};

void Stage::stageUpdate(float elapsedTime)
{
    timer += elapsedTime;
    switch (state)
    {
    case 0:
        enemy = stageEnemy;

        state++;
        /*fallthrough*/
    case 1:
    {
        enemy = stageEnemy;
        while (enemy->time <= timer)
        {
            if (enemy->enemyType < 0) break;

            if (enemy->appear)
            {
                enemy++;
                continue;
            }

            switch (enemy->enemyType)
            {
            case MISSILE:
            {
                Enemy* e = new EnemySmallFry0;
                DirectX::XMFLOAT3 pos = enemy->position;
                pos.x -= 10.0f;
                e->SetPosition(DirectX::XMFLOAT3(pos));
                EnemyManager::Instance().Register(e);

                break;
            }
            case SIN:
            {
                Enemy* e = new EnemySmallFry1;
                DirectX::XMFLOAT3 pos = enemy->position;
                pos.x -= 10.0f;
                e->SetPosition(DirectX::XMFLOAT3(pos));
                EnemyManager::Instance().Register(e);

                break;
            }
            case ROTATION:
            {
                Enemy* e = new EnemySmallFry2;
                DirectX::XMFLOAT3 pos = enemy->position;
                pos.x -= 10.0f;
                e->SetPosition(DirectX::XMFLOAT3(pos));
                EnemyManager::Instance().Register(e);

                break;
            }
            case TOGE:
            {
                Enemy* e = new EnemyNormal0;
                DirectX::XMFLOAT3 pos = enemy->position;
                pos.x -= 10.0f;
                e->SetPosition(DirectX::XMFLOAT3(pos));
                EnemyManager::Instance().Register(e);

                break;
            }
            case SHURIKEN:
            {
                Enemy* e = new EnemyNormal1;
                DirectX::XMFLOAT3 pos = enemy->position;
                pos.x -= 10.0f;
                e->SetPosition(DirectX::XMFLOAT3(pos));
                EnemyManager::Instance().Register(e);

                break;
            }
            case BOSS:
            {
                Enemy* e = new EnemyBoss;
                DirectX::XMFLOAT3 pos = enemy->position;
                pos.x -= 10.0f;
                e->SetPosition(DirectX::XMFLOAT3(pos));
                EnemyManager::Instance().Register(e);

                break;
            }
            }
            enemy->appear = true;

            enemy++;
        }
    }
    }
}

void Stage::Clear()
{
    StageEnemy* stage = stageEnemy;

    while (stage->enemyType >= 0)
    {
        stage->appear = false;
        stage++;
    }
}