#include "SceneGame.h"
#include "SceneLoading.h"
#include "SceneTitle.h"
#include "SceneClear.h"
#include "SceneGameover.h"
#include "SceneManager.h"
#include "Camera.h"
#include "EnemyManager.h"
#include "EnemySlime.h"
#include "EffectManager.h"
#include "Input/Input.h"
#include "BackManager.h"
#include "EnemySmallFry.h"
#include "playerManager.h"
#include "EnemyNormal.h"
#include "EnemyBoss.h"
#include "GameParameter.h"
#include "Stage.h"

// カメラシェイク
static int cameraShakeFrame = 0;
static float cameraShakeRange = 0.1f;

// 初期化
void SceneGame::Initialize()
{
	//背景の初期化
	Back_Manager& back_Manager = Back_Manager::Instance();
	for (int i = 0;i < 2;i++) {
		back_Manager.Register(new Back(DirectX::XMFLOAT3(0, 0, i*-1280.0f)));
	}
	//自機の初期化
	PlayerManager::Instance().Register(new Player("Data/Model/Player_Blue/player_blue.mdl"));
	PlayerManager::Instance().Register(new Player("Data/Model/Player_Red/player_red.mdl"));
	PlayerManager::Instance().getCurrentPlayer()->bomb_voice = Audio::Instance().LoadAudioSource("Data/Audio/bomb Voice mirai.wav");
	PlayerManager::Instance().getStandByPlayer()->bomb_voice = Audio::Instance().LoadAudioSource("Data/Audio/bomb Voice kako.wav");
	PlayerManager::Instance().getCurrentPlayer()->damage_voice = Audio::Instance().LoadAudioSource("Data/Audio/Damage Voice mirai.wav");
	PlayerManager::Instance().getStandByPlayer()->damage_voice = Audio::Instance().LoadAudioSource("Data/Audio/Damage Voice kako.wav");
	PlayerManager::Instance().getCurrentPlayer()->dead_voice = Audio::Instance().LoadAudioSource("Data/Audio/dead voice mirai.wav");
	PlayerManager::Instance().getStandByPlayer()->dead_voice = Audio::Instance().LoadAudioSource("Data/Audio/dead voice kako.wav");
	PlayerManager::Instance().getStandByPlayer()->change_voice = Audio::Instance().LoadAudioSource("Data/Audio/change voice mirai.wav");
	PlayerManager::Instance().getCurrentPlayer()->change_voice = Audio::Instance().LoadAudioSource("Data/Audio/change voice kako.wav");
	PlayerManager::Instance().getCurrentPlayer()->victory_voice = Audio::Instance().LoadAudioSource("Data/Audio/victory voice mirai.wav");
	PlayerManager::Instance().getStandByPlayer()->victory_voice = Audio::Instance().LoadAudioSource("Data/Audio/victory voice kako.wav");
	PlayerManager::Instance().getCurrentPlayer()->boss_away_voice = Audio::Instance().LoadAudioSource("Data/Audio/boss away voice mirai.wav");
	PlayerManager::Instance().getStandByPlayer()->boss_away_voice = Audio::Instance().LoadAudioSource("Data/Audio/boss away voice kako.wav");
	PlayerManager::Instance().getCurrentPlayer()->Set_Speed(10.0f);
	PlayerManager::Instance().getCurrentPlayer()->Set_Attack(1.0f);
	PlayerManager::Instance().getCurrentPlayer()->Set_Defence(0.5f);
	PlayerManager::Instance().getStandByPlayer()->Set_Speed(30.0f);
	PlayerManager::Instance().getStandByPlayer()->Set_Attack(2.0f);
	PlayerManager::Instance().getStandByPlayer()->Set_Defence(0.0f);
	PlayerManager::Instance().getCurrentPlayer()->SetPosition(DirectX::XMFLOAT3(-20, 0, -20));

	//カメラコントローラー初期化
	cameraController = new CameraController;
	cameraController->SetTarget(DirectX::XMFLOAT3(0, 80, 0));

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

	// シーン切り替えエフェクト読み込み
	sceneInEffect = new Effect("Data/Effect/Scene_in.efk");
	sceneEndEffect = new Effect("Data/Effect/Scene_end.efk");

	PlayerManager::Instance().SetCameraShake(cameraShakeFrame, cameraShakeRange);

	//エネミー初期化
	//for (int i = 0;i < 2;++i) {
	//	EnemySmallFry0* slime = new EnemySmallFry0;
	//	slime->SetPosition(DirectX::XMFLOAT3(i * 2.0f, 0, 40));
	//	EnemyManager::Instance().Register(slime);
	//}
	Enemy* enemy1 = new EnemySmallFry1;
	enemy1->SetPosition(DirectX::XMFLOAT3(40, 0, -50));
	EnemyManager::Instance().Register(enemy1);
	//EnemySmallFry2* enemy2 = new EnemySmallFry2;
	//enemy2->SetPosition(DirectX::XMFLOAT3(10, 0, -25));
	//EnemyManager::Instance().Register(enemy2);
	//EnemyNormal0* enemy3 = new EnemyNormal0;
	//enemy3->SetPosition(DirectX::XMFLOAT3(-5, 0, 20));
	//EnemyManager::Instance().Register(enemy3);
	//EnemyNormal1* enemy4 = new EnemyNormal1;
	//enemy4->SetPosition(DirectX::XMFLOAT3(5, 0, 10));
	//EnemyManager::Instance().Register(enemy4);
	//EnemyBoss* enemy5 = new EnemyBoss;
	//enemy5->SetPosition(DirectX::XMFLOAT3(0, 0, 20));
	//EnemyManager::Instance().Register(enemy5);
	stage = new Stage();

	//スプライト
	gauge = new Sprite();
	UI_Base = new Sprite("Data/Sprite/UIBack_Base.png");
	separator_left = new Sprite("Data/Sprite/Separator01.png");
	separator_right = new Sprite("Data/Sprite/Separator02.png");

	isGameClear = false;
	isGameover = false;
	HP_Gauge::Instance().Initialize();
	UIManager::Instance().Register(&HP_Gauge::Instance());
	UIManager::Instance().Register(&Bomb_Gauge::Instance());
	UIManager::Instance().Register(&Handl::Instance());
	sceneChange = Audio::Instance().LoadAudioSource("Data/Sound/SceneChange.wav");
	//オーディオ
	BGM = Audio::Instance().LoadAudioSource("Data/Audio/Rusty Gears.wav");
	BGM->Filter(BGM->HIGH_PASS_FILTER);
	sceneChange = Audio::Instance().LoadAudioSource("Data/Sound/SceneChange.wav");
}

// 終了化
void SceneGame::Finalize()
{
	//ゲージスプライト終了化
	if (gauge != nullptr)
	{
		delete gauge;
		gauge = nullptr;
	}


	//ステージ終了化
	//StageManager::Instance().Clear();
	if (stage != nullptr)
	{
		stage->Clear();
		delete stage;
		stage = nullptr;
	}

	//エフェクト終了化
	if (sceneInEffect != nullptr)
	{
		delete sceneInEffect;
		sceneInEffect = nullptr;
	}
	if (sceneEndEffect != nullptr)
	{
		delete sceneEndEffect;
		sceneEndEffect = nullptr;
	}

	if (cameraController != nullptr)
	{
		delete cameraController;
		cameraController = nullptr;
	}

	if (UI_Base != nullptr)
	{
		delete UI_Base;
		UI_Base = nullptr;
	}

	if (bomb_gauge != nullptr)
	{
		delete bomb_gauge;
		bomb_gauge = nullptr;
	}

	if (separator_left != nullptr)
	{
		delete separator_left;
		separator_left = nullptr;
	}

	if (separator_right != nullptr)
	{
		delete separator_right;
		separator_right = nullptr;
	}

	PlayerManager::Instance().Clear();
	EnemyManager::Instance().Clear();
	Back_Manager::Instance().Clear();
}

// 更新処理
void SceneGame::Update(float elapsedTime)
{
	if (!isOnEffect)
	{
		BGM->Play(true);
		sceneInEffect->Play({ 0, 65, 0 }, 1.0f);
		isOnEffect = true;
	}

	switch (state_)
	{
	case 0:
		if (!sceneInEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
		{
			sceneChange->DC_Play();
			state_++;
			//isOnEffect = false;
			stage->stageUpdate(elapsedTime);
		}

		EffectManager::Instance().Update(elapsedTime);

		break;

	case 1:
		stage->stageUpdate(elapsedTime);

		GamePad& gamePad = Input::Instance().GetGamePad();
		Graphics& graphics = Graphics::Instance();
		ID3D11DeviceContext* dc = graphics.GetDeviceContext();

		const GamePadButton anyButton =
			GamePad::BTN_BACK;
		if (gamePad.GetButtonDown() & anyButton)
		{
			SceneManager::Instance().ChangeScene((new SceneTitle));
			break;
		}
		//if (gamePad.GetButtonDown() & GamePad::BTN_START)
		if (isGameClear)
		{
			effect = true;
		}
		if (PlayerManager::Instance().GetPlayerHealth() <= 0 && !effect)
		{
			effect = true;
			isGameover = true;
		}

		GameParameter::Instance().Update(elapsedTime);
		Back_Manager::Instance().Updata(elapsedTime);
		PlayerManager::Instance().Updata(elapsedTime);

		//カメラコントローラー更新処理
		//DirectX::XMFLOAT3 target = player->GetPosition();
		//target.y += 0.5f;
		cameraController->SetTarget(DirectX::XMFLOAT3(0, 80, 0));
		cameraController->Update(elapsedTime);

		if (effect) {
			if (!sceneEndEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
			{
				if (isGameClear)
				{
					GameClear();
				}
				if (isGameover)
				{
					Gameover();
				}
			}
		}

		EnemyManager::Instance().Update(elapsedTime, dc, Camera::Instance().GetView(), Camera::Instance().GetProjection());
		//エフェクト更新処理
		EffectManager::Instance().Update(elapsedTime);
		UIManager::Instance().Updata(elapsedTime);
		break;
	}
}

// 描画処理
void SceneGame::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	// 画面クリア＆レンダーターゲット設定
	FLOAT color[] = { 0.0f, 0.0f, 0.5f, 1.0f };	// RGBA(0.0～1.0)
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// 描画処理
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ライト方向（下方向）

	//カメラのパラメーター設定
	Camera& camera = Camera::Instance();
	rc.view		   = camera.GetView();
	rc.projection  = camera.GetProjection();


	Back_Manager::Instance().Render(dc);
	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		PlayerManager::Instance().Render(dc, shader);
		EnemyManager::Instance().Render(dc, shader, rc.view, rc.projection);
		shader->End(dc);
	}
	// 2Dスプライト描画
	{

		//if(!isOnEffect)
		{
			RenderEnemyGauge(dc, rc.view, rc.projection);
			float offset = GameParameter::Instance().GetCameraShakeOffset().x * 100;
			UI_Base->Render(dc, 853 + offset, 0, 853, 720,
				1, 1, 853, 720, 0,
				1, 1, 1, 1);

			separator_left->Render(
				dc, 0, 0, 140, 720,
				0, 0, 140, 720, 0,
				1, 1, 1, 1
			);

			separator_left->Render(
				dc, 800, 0, 60, 720,
				0, 0, 60, 720, 0,
				1, 1, 1, 1
			);

			UI_Base->Render(dc, 853 + offset, 0, 853, 720,
				1, 1, 853, 720, 0,
				1, 1, 1, 1);
			UIManager::Instance().Render(dc);
		}
	}
	// 3Dモデル描画
	{
		Shader* shader = graphics.GetShader();
		shader->Begin(dc, rc);
		//ステージ描画
		PlayerManager::Instance().Render(dc, shader);
		EnemyManager::Instance().Render(dc, shader, rc.view, rc.projection);
		shader->End(dc);
	}

	// 3Dデバッグ描画
	{
		//プレイヤーデバッグプリミティブ描画
		PlayerManager::Instance().DrawDebugPrimitive();
		//エネミーデバッグプリミティブ
		EnemyManager::Instance().DrawDebugPrimitive();
		// ラインレンダラ描画実行
		graphics.GetLineRenderer()->Render(dc, rc.view, rc.projection);

		// デバッグレンダラ描画実行
		graphics.GetDebugRenderer()->Render(dc, rc.view, rc.projection);
	}

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

	// 2DデバッグGUI描画
	/* {
		ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize(ImVec2(300, 300), ImGuiCond_FirstUseEver);

		if (ImGui::Begin("Debug Param", nullptr, ImGuiWindowFlags_None))
		{
			int id = 0;

			// プレイヤーデバッグ描画
			ImGui::PushID(id++);
			{
				PlayerManager::Instance().DrawDebugGui();
			}
			ImGui::PopID();

			// カメラデバッグ描画
			ImGui::PushID(id++);
			{
				camera.DrawDebugGui();
			}
			ImGui::PopID();

			// カメラコントローラーデバッグ描画
			ImGui::PushID(id++);
			{
				cameraController->DrawDebugGui();
			}
			ImGui::PopID();

			// エネミーマネージャーデバッグ描画
			ImGui::PushID(id++);
			{
				EnemyManager::Instance().DrawDebugGui();
			}
			ImGui::PopID();
		}
		ImGui::End();
	}*/

}


//エネミーHPゲージ描画
void SceneGame::RenderEnemyGauge(
	ID3D11DeviceContext* dc,
	const DirectX::XMFLOAT4X4& view,
	const DirectX::XMFLOAT4X4& projection
)
{
	//ビューポート
	D3D11_VIEWPORT viewport;
	UINT numViewports = 1;
	dc->RSGetViewports(&numViewports, &viewport);

	//変換行列
	DirectX::XMMATRIX View = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX Projection = DirectX::XMLoadFloat4x4(&projection);
	DirectX::XMMATRIX World = DirectX::XMMatrixIdentity();

	//全ての敵の頭上にHPゲージを表示
	EnemyManager& enemyManager = EnemyManager::Instance();
	int enemyCount = enemyManager.GetEnemyCount();

	for (int i = 0; i < enemyCount; ++i)
	{
		Enemy* enemy = enemyManager.GetEnemy(i);
		DirectX::XMVECTOR ScreenPosition;
		DirectX::XMFLOAT3 Position;
		Position = enemy->GetPosition();
		Position.y += enemy->GetHeight();
		ScreenPosition = DirectX::XMVector3Project(
			DirectX::XMLoadFloat3(&Position),
			viewport.TopLeftX,
			viewport.TopLeftY,
			viewport.Width,
			viewport.Height,
			0.0f,
			1.0f,
			Projection,
			View,
			World
		);
		DirectX::XMFLOAT3 pos;
		DirectX::XMStoreFloat3(&pos, ScreenPosition);
		float offset = GameParameter::Instance().GetCameraShakeOffset().x * 100;
		gauge->Render(dc,
			pos.x - 50.0f * 0.5f + offset, pos.y - 10.0f,
			10.0f * enemy->GetHealth(), enemy->GetMaxhealth(),
			0.0f, 0.0f,
			0.0f, 0.0f,
			0.0f,
			1.0f, 0.0f, 0.0f, 1.0f);
	}
}

//DirectX::XMFLOAT3 SceneGame::Position3Dto2D(
//	ID3D11DeviceContext* dc,
//	const DirectX::XMFLOAT4X4& view,
//	const DirectX::XMFLOAT4X4& projection,
//	DirectX::XMFLOAT3 position
//)
//{
//	D3D11_VIEWPORT viewport;
//	UINT numViewports = 1;
//	dc->RSGetViewports(&numViewports, &viewport);
//}
//

void SceneGame::GameClear()
{
	effect = true;

	switch (endState)
	{
	case 0:
		if (!PlayerManager::Instance().getCurrentPlayer()->victory_voice->IsPlay() || !PlayerManager::Instance().getCurrentPlayer()->boss_away_voice->IsPlay())
		{
			sceneEndEffect->Play({ 0, 70, 0 }, 1.0f);
			sceneChange->DC_Play();
			endState++;
			break;
		}
		break;

	case 1:
		if (!sceneEndEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
		{
			endState++;
			break;
		}
		break;

	case 2:
		SceneManager::Instance().ChangeScene(new SceneClear);
		break;
	}
}

void SceneGame::Gameover()
{
	effect = true;

	switch (endState)
	{
	case 0:
		if (!PlayerManager::Instance().getCurrentPlayer()->dead_voice->IsPlay())
		{
			sceneEndEffect->Play({ 0, 70, 0 }, 1.0f);
			sceneChange->DC_Play();
			endState++;
			break;
		}
		break;

	case 1:
		if (!sceneEndEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
		{
			endState++;
			break;
		}
		break;

	case 2:
		SceneManager::Instance().ChangeScene(new SceneGameover);
		break;
	}

}