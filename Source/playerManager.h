#pragma once
#include<imgui.h>
#include"Player.h"

enum class HitStrength
{
	Weak = 0,
	Midium,
	Strong,

	EnumCount
};

class PlayerManager
{
public:
	PlayerManager() {};
	~PlayerManager() {};
	static PlayerManager& Instance()
	{
		static PlayerManager instance;
		return instance;
	}

	void Updata(float elapsedTime);

	void Render(ID3D11DeviceContext* dc,Shader*shader);

	void Clear();

	void Register(Player* play);

	void Player_Swap();

	Player* getCurrentPlayer() { return Players[0]; }
	Player* getStandByPlayer() { return Players[1]; }

	// カメラシェイク設定
	void SetCameraShake(int frame, float range)
	{
		cameraShakeFrame = frame;
		cameraShakeRange = range;
	}

	//現在のプレイヤーの体力の取得
	int GetPlayerHealth()
	{
		return getCurrentPlayer()->GetHealth();
	}

	//デバッグ用
	void DrawDebugGui();
	void DrawDebugPrimitive();
private:
	std::vector<Player*>Players;

protected:
	int	cameraShakeFrame = 10;
	float cameraShakeRange = 0.1f;
};