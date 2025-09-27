#pragma once
#include"Projectile.h"
#include"Graphics/Sprite.h"

class Bomb:public Projectile
{
public:
	Bomb(ProjectileManager* manager);
	~Bomb() override;
	void Update(float elapsedTime)override;
	void Render(ID3D11DeviceContext* dc,Shader*shader)override;
private:
	DirectX::XMFLOAT2 pos = { 0,0 };
	DirectX::XMFLOAT2 width = { 853,720 };
	//x=r,y=g,z=b,a=zÇ≈çÏÇ¡ÇƒÇ‹Ç∑
	DirectX::XMFLOAT4 color = { 1,1,1,1 };
	Sprite* booom;
};

