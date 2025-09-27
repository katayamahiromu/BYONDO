#include"UI.h"
#include"GameParameter.h"
#include"Player.h"
#include"playerManager.h"

HP_Gauge::HP_Gauge()
{
	HP = new Sprite("Data/Sprite/UI_HP_Gage.png");
	tex_odd = Player::Instance().GetHealth();
}

HP_Gauge::~HP_Gauge()
{
	delete HP;
}

void HP_Gauge::Initialize()
{
	tex_odd = Player::Instance().GetHealth();
}

void HP_Gauge::Update(float elapsedTime)
{
	float hp = PlayerManager::Instance().getCurrentPlayer()->GetHealth();
	if (tex_odd >= hp)
	{
		tex_odd -= elapsedTime;
	}
}

void HP_Gauge::Render(ID3D11DeviceContext* dc)
{
	float offset = GameParameter::Instance().GetCameraShakeOffset().x * 100;
	HP->Render(dc, 860 + offset, 0, 84 * tex_odd * 0.95f, 720,
		1, 1, 84 * tex_odd * 0.95f, 720, 0,
		1, 1, 1, 1);
}





Bomb_Gauge::Bomb_Gauge()
{
	bomb = new Sprite("Data/Sprite/UIneedle.png");
}

Bomb_Gauge::~Bomb_Gauge()
{
	delete bomb;
}

void Bomb_Gauge::Update(float elapsedTime)
{
	if (use) {
		Use_Update(elapsedTime);
	}
	else {
		angle += elapsedTime * speed;
		if (angle >= 340) {
			angle = 340;
			bomb_flag = true;
		}
		else
		{
			bomb_flag = false;
		}
	}
}

void Bomb_Gauge::Render(ID3D11DeviceContext* dc)
{
	bomb->Render_Add(dc, 852, 0, 420, 720,
		1, 1, 420, 720, 
		340,423,
		angle,
		1, 1, 1, 1);
}

void Bomb_Gauge::Use_Update(float elapsedTime)
{
	if (angle >= 0) {
		angle -= elapsedTime * 40;
		bomb_flag = false;
	}
	else
	{
		use = false;
	}
}



Handl::Handl()
{
	handle = new Sprite("Data/Sprite/UIhandle.png");
}

Handl::~Handl()
{
	delete handle;
}

void Handl::Update(float elapsedTime)
{
	angle = -DirectX::XMConvertToDegrees(PlayerManager::Instance().getCurrentPlayer()->GetAngle().z);
}

void Handl::Render(ID3D11DeviceContext* dc)
{
	handle->Render_Add(dc, 860, 0, 420, 720,
		1, 1,420,720,
		207, 637, angle,
		1, 1, 1, 1);
}