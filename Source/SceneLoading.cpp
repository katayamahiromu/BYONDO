#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Camera.h"
#include"EffectManager.h"

//初期化
void SceneLoading::Initialize()
{
	srand((unsigned)time(NULL));
	random = rand() % 3;
	//スプライトの初期化
	//スレッド開始
	thread = new std::thread(SceneLoading::LoadingThread, this);
	// シーン切り替えエフェクト読み込み
	sceneInEffect = new Effect("Data/Effect/Scene_in.efk");
	sceneEndEffect = new Effect("Data/Effect/Scene_end.efk");

	// カメラコントローラー初期化
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
	explanation1 = new Sprite("Data/Sprite/Load_Explanation1.png");
	explanation2 = new Sprite("Data/Sprite/Load_Explanation2.png");
	explanation3 = new Sprite("Data/Sprite/Load_Explanation3.png");

	start_voice_mirai = Audio::Instance().LoadAudioSource("Data/Audio/Start1 Voice.wav");
	start_voice_kako = Audio::Instance().LoadAudioSource("Data/Audio/Start2 Voice.wav");

	voice_random = rand() % 2;
	switch (random)
	{
	case 0:
		switch (voice_random)
		{
		case 0:
			start_voice_mirai->DC_Play();
			break;
		case 1:
			start_voice_kako->DC_Play();
			break;
		}
		break;
	case 1:
		start_voice_mirai->DC_Play();
		break;
	case 2:
		start_voice_kako->DC_Play();
		break;
	}
}

//終了化
void SceneLoading::Finalize()
{
	//スレッド終了化
	thread->join();
	if (thread != nullptr)
	{
		delete thread;
		thread = nullptr;
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

	if(explanation1!=nullptr)
	{
		delete explanation1;
		explanation1 = nullptr;
	}


	if (explanation2 != nullptr)
	{
		delete explanation2;
		explanation2 = nullptr;
	}
	
	if (explanation3 != nullptr)
	{
		delete explanation3;
		explanation3 = nullptr;
	}
	
	// ローディング中にゲームを落としたとき、次のシーンの終了化もする
	if (SceneManager::Instance().GetCurrentScene() == this
		&& SceneManager::Instance().GetNextScene() == nullptr)
	{
		nextScene->Finalize();
		delete nextScene;
		nextScene = nullptr;
	}
}

//更新処理
void SceneLoading::Update(float elapsedTime)
{

	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_START) {
		if (nextScene->IsReady())
		{
			SceneManager::Instance().ChangeScene(nextScene);
		}
	}
	EffectManager::Instance().Update(elapsedTime);
}

//描画処理
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//画面クリア＆ターゲット設定
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
		switch (random)
		{
		case 0:
			explanation1->Render(dc,
				0, 0, 1280, 720,
				0, 0, 1280, 720,
				0,
				1, 1, 1, 1
			);
			break;
		case 1:
			explanation2->Render(dc,
				0, 0, 1280, 720,
				0, 0, 1280, 720,
				0,
				1, 1, 1, 1
			);
			break;
		case 2:
			explanation3->Render(dc,
				0, 0, 1280, 720,
				0, 0, 1280, 720,
				0,
				1, 1, 1, 1
			);
			break;
		}
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM関連の初期化でスレッド毎に呼ぶ必要がある
	CoInitialize(nullptr);

	// 次のシーンの初期化を行う
	scene->nextScene->Initialize();

	//スレッドが終わる前にCOM関連の終了化
	CoUninitialize();

	//次のシーンの準備完了設定
	scene->nextScene->SetReady();
}