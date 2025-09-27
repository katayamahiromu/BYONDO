#include <imgui.h>
#include"EnemyManager.h"
#include"Graphics/Graphics.h"
#include"Collision.h"
#include "EnemySmallFry.h"
#include "EnemyNormal.h"
#include "playerManager.h"
#include "GameParameter.h"
#include "EnemyBoss.h"

EnemyManager::EnemyManager()
{
    hitEffect = new Effect("Data/Effect/Damage.efk");
    //playerHit = Audio::Instance().LoadAudioSource("Data/Sound/player_hit.wav");
}

// �f�X�g���N�^
EnemyManager::~EnemyManager()
{
    projectileManager.Clear();
    delete hitEffect;
}

//�X�V����
void EnemyManager::Update(float elapsedTime, ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection)
{
	for (Enemy* enemy : enemies) {
		enemy->Update(elapsedTime);
        //enemy->ToScreenCoordinate(dc, view, projection);
        enemy->CheckDeath();
	}

    projectileManager.Update(elapsedTime);

    //�j������
    //�X�V�������I��������Ƃɔj�����X�g�ɐς܂ꂽ�I�u�W�F�N�g��j������
    for (Enemy* enemy : removes) {
        //std::vector����v�f���폜����ꍇ�̓C�e���[�^�[���g��Ȃ���΂Ȃ�Ȃ�
        std::vector<Enemy*>::iterator it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end()) {
            enemies.erase(it);
        }
        //�G�l�~�[�̔j������
        delete enemy;
    }
    //�j�����X�g���N���A
    removes.clear();
	//�G���m�̏Փ˔���
	//CollisionEnemyVsEnemy();
    // �v���C���[�ƓG�̒e�̏Փ˔���
    CollisionPlayerVsEnemyProjectiles();
}

//�`�揈��
void EnemyManager::Render(ID3D11DeviceContext* dc, Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection) {
	for (Enemy* enemy : enemies) {
        enemy->CheckDraw();
		enemy->Render(dc, shader);
	}

    projectileManager.Render(dc, shader);
}

//�G�l�~�[�o�^
void EnemyManager::Register(Enemy* enemy) {
	enemies.emplace_back(enemy);
}

//�G�l�~�[�폜
void EnemyManager::Remove(Enemy* enemy){
    //�j�����X�g�ɒǉ�
    removes.insert(enemy);
}

//�G�l�~�[�S�폜
void EnemyManager::Clear() {
	for (Enemy* enemy : enemies) {
		delete enemy;
	}
	enemies.clear();
}

//�f�o�b�O�v���~�e�B�u�`��
void EnemyManager::DrawDebugPrimitive() {
	for (Enemy* enemy : enemies) {
		enemy->DrawDebugPrimitive();
	}
}

// �f�o�b�O�pGUI�`��
void EnemyManager::DrawDebugGui()
{
    if (ImGui::CollapsingHeader("EnemyManager"))
    {
        if (ImGui::TreeNode("Add"))
        {
            DirectX::XMFLOAT3 position =
            {
                ((static_cast<float>(rand() * 2) / RAND_MAX) - 1.0f) * 5.0f,
                0.0f,
                ((static_cast<float>(rand() * 2) / RAND_MAX) - 1.0f) * 5.0f
            };
            if(ImGui::Button("sf0:Missile Homing"))
            {
                Enemy* enemy = new EnemySmallFry0();
                enemy->SetPosition(position);
                Register(enemy);
            }
            if (ImGui::Button("sf1:Teki1 Sin"))
            {
                Enemy* enemy = new EnemySmallFry1();
                enemy->SetPosition(position);
                Register(enemy);
            }
            if (ImGui::Button("sf2:Teki1 Rotate"))
            {
                Enemy* enemy = new EnemySmallFry2();
                enemy->SetPosition(position);
                Register(enemy);
            }
            if (ImGui::Button("n0:Toge Radiate"))
            {
                Enemy* enemy = new EnemyNormal0();
                enemy->SetPosition(position);
                Register(enemy);
            }
            if (ImGui::Button("n1:Shuriken HomingShot"))
            {
                Enemy* enemy = new EnemyNormal1();
                enemy->SetPosition(position);
                Register(enemy);
            }
            if (ImGui::Button("boss"))
            {
                Enemy* enemy = new EnemyBoss();
                enemy->SetPosition(position);
                Register(enemy);
            }

            ImGui::TreePop();
        }
        if (ImGui::Button("Clear"))
        {
            Clear();
        }

        ImGui::Checkbox("VisivleDebugPrimitive", &visibleDebugPrimitive);

        projectileManager.DrawDebugGui();

        int index = 0;
        for (Enemy* enemy : enemies)
        {
            char name[32];
            ::sprintf_s(name, sizeof(name), "%d", index);

            ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;

            if (ImGui::TreeNodeEx(enemy, node_flags, name))
            {
                ImGui::PushID(index);
                enemy->DrawDebugGui();
                ImGui::PopID();
                ImGui::TreePop();
            }
            index++;
        }
    }
}

// �G���m�̏Փ˔���
void EnemyManager::CollisionEnemyVsEnemy() {
    int enemyCount = GetEnemyCount();

    for (int i = 0; i < enemyCount; ++i)
    {
        Enemy* enemy1 = GetEnemy(i);
        for (int j = 0; j < enemyCount; ++j)
        {
            Enemy* enemy2 = GetEnemy(j);
            if (enemy1 == enemy2)continue;

            // �Փˏ���
            DirectX::XMFLOAT3 outPosition;
            if (Collision::IntersectCylinderVsCylinder
            (enemy1->GetPosition(),
             enemy1->GetRadius(),
             enemy1->GetHeight(),
             enemy2->GetPosition(),
             enemy2->GetRadius(),
             enemy2->GetHeight(),
             outPosition))
            {
                // �����o����̈ʒu����
                enemy2->SetPosition(outPosition);
            }
        }
    }
}

// �v���C���[�ƓG�̒e�̏Փ˔���
void EnemyManager::CollisionPlayerVsEnemyProjectiles()
{
    Player* player = PlayerManager::Instance().getCurrentPlayer();
    
    int projectileCount = projectileManager.GetProjectileCount();
    
    for (int i = 0; i < projectileCount; ++i)
    {
        Projectile* projectile = projectileManager.GetProjectile(i);

        // �Փˏ���
        DirectX::XMFLOAT3 outPosition;
        if (Collision::IntersctSphereVsCylinder(
            projectile->GetPosition(), projectile->GetRadius(),
            player->GetPosition(), player->GetRadius(), player->GetHeight(), outPosition))
        {
            // �_���[�W��^����
            if (player->ApplyDamage(1.0-player->Get_Defence(), 2.0f))
            {
                DirectX::XMFLOAT3 p = player->GetPosition();
                hitEffect->Play(p);

                player->Camera_Shake();
                //playerHit->Play(false);
                player->isHit = true;

                // �e�۔j��
                projectile->Destroy();
            }
        }
    }
}