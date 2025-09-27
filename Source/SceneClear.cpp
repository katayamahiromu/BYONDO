#include"Graphics/Graphics.h"
#include "SceneClear.h"
#include"SceneTitle.h"
#include"SceneLoading.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"Input/Input.h"
#include"Camera.h"
#include"EffectManager.h"

void SceneClear::Initialize()
{
    //スプライト初期化
    clear = new Sprite("Data/Sprite/Clear.png");

    //シーン切り替えエフェクト読み込み
    sceneInEffect = new Effect("Data/Effect/Scene_in.efk");
    sceneEndEffect = new Effect("Data/Effect/Scene_end.efk");

	//カメラコントローラー初期化
	cameraController = new CameraController;
	cameraController->SetTarget(DirectX::XMFLOAT3(0, 30, 0));

	//カメラ初期設定
	Graphics& graphics = Graphics::Instance();
	Camera& camera = Camera::Instance();
	camera.SetLookAt(
		DirectX::XMFLOAT3(0, 100, -10),
		DirectX::XMFLOAT3(0, 0, 0),
		DirectX::XMFLOAT3(0, 1, 0)
	);
	camera.SetPerspectiveFov(
		DirectX::XMConvertToRadians(45),
		graphics.GetScreenWidth() / graphics.GetScreenHeight(),
		0.1f,
		1000.0f
	);

	bgm = Audio::Instance().LoadAudioSource("Data/Audio/Clear.wav");
	sceneChange = Audio::Instance().LoadAudioSource("Data/Sound/SceneChange.wav");
	praise_voice_1 = Audio::Instance().LoadAudioSource("Data/Audio/praise voice kako.wav");
	praise_voice_2 = Audio::Instance().LoadAudioSource("Data/Audio/praise voice mirai.wav");
	Abuse_voice_1 = Audio::Instance().LoadAudioSource("Data/Audio/abou voice kako.wav");
	Abuse_voice_2 = Audio::Instance().LoadAudioSource("Data/Audio/abou voice mirai.wav");


	bgm->Play(true);
	sceneInEffect->Play({ 0, 75, -8.5f }, 1.0f);
	sceneChange->DC_Play();

	int rnd = rand() % 2;
	switch (rnd)
	{
	case 0:
		praise_voice_1->Play(false);
		break;
	case 1:
		praise_voice_2->Play(false);
		break;
	}
	sceneChange->DC_Play();
}


void SceneClear::Finalize()
{
	//スプライト終了化
	if (clear != nullptr)
	{
		delete clear;
		clear = nullptr;
	}

	//エフェクト終了化
	delete sceneInEffect;
	delete sceneEndEffect;

	// カメラ終了化
	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	state_ = 0;
	isOnEffect = false;

}

void SceneClear::Update(float elapsedTime)
{
	GamePad& gamePad = Input::Instance().GetGamePad();

	if (gamePad.GetButtonDown() & GamePad::BTN_START)
	{
		effect = true;
		sceneEndEffect->Play({ 0, 75, -8.5f }, 1.0f);
		sceneChange->DC_Play();
	}

	if (effect)
	{
		if (!sceneEndEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
		{
			Scene_Change();
		}
	}

	// エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
	Update_intarval(elapsedTime);
}

void SceneClear::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆レンダーターゲット
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラのパラメーター設定
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//2Dスプライト描画
	{
		float screenWidth = static_cast<float>(graphics.GetScreenWidth());
		float screenHeight = static_cast<float>(graphics.GetScreenHeight());
		float textureWidth = static_cast<float>(clear->GetTextureWidth());
		float textureHeight = static_cast<float>(clear->GetTextureHeight());

		//クリアスプライト
		clear->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);
	};

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
}

void SceneClear::Scene_Change()
{
	effect = true;
	SceneManager::Instance().ChangeScene(new SceneTitle);
}

void SceneClear::Update_intarval(float elapsedTime)
{
	if (intarval >= 0) {
		intarval -= elapsedTime;
	}
	else
	{
		intarval = 20.0f;
		voice_play_count++;

		int rnd = rand() % 2;
		if (voice_play_count > 3) {
			switch (rnd)
			{
			case 0:
				Abuse_voice_1->Play(false);
				break;
			case 1:
				Abuse_voice_2->Play(false);
				break;
			}
		}
		else
		{
			switch (rnd)
			{
			case 0:
				praise_voice_1->Play(false);
				break;
			case 1:
				praise_voice_2->Play(false);
				break;
			}
		}
	}
}
