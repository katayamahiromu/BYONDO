#pragma once
#include <DirectXMath.h>

// ゲームパラメーター
class GameParameter
{
private:
	GameParameter() = default;
	~GameParameter() = default;

public:
	// インスタンス取得
	static GameParameter& Instance()
	{
		static GameParameter instance;
		return instance;
	}

	// 更新処理
	void Update(float elapsedTime);

	// カメラシェイク設定
	void SetCameraShake(int frame, float range) { cameraShakeFrame = frame + 1; cameraShakeRange = range / cameraShakeFrame; }

	// カメラシェイクオフセット取得
	const DirectX::XMFLOAT3& GetCameraShakeOffset() const { return cameraShakeOffset; }

private:
	int					cameraShakeFrame = 0;
	float				cameraShakeRange = 0;
	DirectX::XMFLOAT3	cameraShakeOffset = { 0, 0, 0 };
};
