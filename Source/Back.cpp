#include"Back.h"
#include"GameParameter.h"

Back::Back(DirectX::XMFLOAT3 position)
{
	back = new Sprite("Data/Sprite/back.png");
	Back_Pos = position;
}

Back::~Back()
{
	if (back != nullptr)
	{
		delete back;
		back = nullptr;
	}
}


void Back::Update(float elapsedTime)
{
	switch (state_)
	{
	case 0:
		shift += speed * elapsedTime;
		if (shift > 740.0f)
		{
			shift -= 1280.0f;
		}
		break;

	case 1:
		///// ƒ{ƒXíŽž /////

		break;
	}
}

void Back::Render(ID3D11DeviceContext* dc)
{
	float offset = GameParameter::Instance().GetCameraShakeOffset().x * 100;
	back->Render(dc,
		Back_Pos.x + 140.0f + offset, Back_Pos.z + shift, 720.0f, 1280.0f,
		0.0f, 0.0f, 720.0f, 1280.0f,
		0.0f,
		1.0f, 1.0f, 1.0f, 1.0f);
}