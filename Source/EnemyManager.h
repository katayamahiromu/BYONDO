#pragma once
#include<vector>
#include<set>
#include"Enemy.h"
#include "ProjectileManager.h"
#include "Effect.h"
#include "Audio/Audio.h"

class EnemyManager
{
private:
	EnemyManager();
	~EnemyManager();
public:
	static EnemyManager& Instance()
	{
		static EnemyManager instance;
		return instance;
	}
	//�X�V����
	void Update(float elapsedTime, ID3D11DeviceContext* dc, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);

	//�`�揈��
	void Render(ID3D11DeviceContext* dc, Shader* shader, const DirectX::XMFLOAT4X4& view, const DirectX::XMFLOAT4X4& projection);
	//�G�l�~�[�o�^
	void Register(Enemy* enemy);
	//�G�l�~�[�폜
	void Remove(Enemy* enemy);
	//�G�l�~�[�S�폜
	void Clear();
	//�f�o�b�O�v���~�e�B�u�`��
	void DrawDebugPrimitive();
	// �f�o�b�O�pGUI�`��
	void DrawDebugGui();
	//�G�l�~�[���擾
	int GetEnemyCount() const { return static_cast<int>(enemies.size()); }
	//�G�l�~�[�擾
	Enemy* GetEnemy(int index) { return enemies.at(index); }
	//�G�l�~�[���m�̏Փ˔���
	void CollisionEnemyVsEnemy();
	// �v���C���[�ƃG�l�~�[�̒e�̏Փ˔���
	void CollisionPlayerVsEnemyProjectiles();

public:
	ProjectileManager projectileManager;

private:
	std::vector<Enemy*> enemies;
	std::set<Enemy*>removes;

	bool visibleDebugPrimitive = true;
	Effect* hitEffect = nullptr;
	//std::unique_ptr<AudioSource> playerHit;
};