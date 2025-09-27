#pragma once
#include"Graphics/Sprite.h"
#include"Graphics/Graphics.h"
class Back
{
public:
	Back(DirectX::XMFLOAT3);
	~Back();

	//更新処理
	void Update(float elapsedTiem);

	//描画処理
	void Render(ID3D11DeviceContext* dc);

private:
	float screenHeight = static_cast<float>(Graphics::Instance().GetScreenHeight());
	DirectX::XMFLOAT3 Back_Pos = {0, 0, 0};
	float shift = 0; //位置をずらす
	Sprite* back = nullptr;
	const float speed = 100;	// スクロール速度
	int state_ = 0;
};
