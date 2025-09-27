#pragma once
#include"Graphics/Shader.h"
#include"Character.h"

//エネミー
class Enemy :public Character
{
public:
	~Enemy()override{}
	//更新処理
	virtual void Update(float elapsedTime) = 0;
	//描画処理
	virtual void Render(ID3D11DeviceContext* dc, Shader* sheder) = 0;
	//回転処理
	void Turn(float elapsedTime, float vx, float vz, float speed) override;
	//デバッグプリミティブ描画
	virtual void DrawDebugPrimitive();
	// デバッグ用GUI描画
	virtual void DrawDebugGui();
	//破棄
	void Destroy();
};