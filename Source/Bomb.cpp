#include"Bomb.h"

Bomb::Bomb(ProjectileManager* manager):Projectile(manager)
{
	booom = new Sprite();
	position = { -18.0f,0.0f,-2.0f };
	radius = 40.0f;
}

Bomb::~Bomb()
{
	delete booom;
}

void Bomb::Update(float elapsedTime)
{
	color.w -= elapsedTime;
	if (color.w < 0)
	{
		Destroy();
	}
}

void Bomb::Render(ID3D11DeviceContext* dc,Shader*shader)
{
	booom->Render(dc, pos.x, pos.y, width.x, width.y,
		1, 1, 1, 1, 0,
		color.x, color.y, color.z, color.w);
}