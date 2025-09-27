#include"Graphics/Graphics.h"
#include"SceneTitle.h"
#include"SceneGame.h"
#include"SceneManager.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"Camera.h"
#include"EffectManager.h"

//初期化
void SceneTitle::Initialize()
{
	//スプライト初期化
	title = new Sprite("Data/Sprite/Title.png");
	exit = new Sprite("Data/Sprite/EXITbutton.png");
	howTo = new Sprite("Data/Sprite/HOWTObutton.png");
	play = new Sprite("Data/Sprite/PLAYbutton.png");
	selecter = new Sprite();
	tutorial = new Sprite("Data/Sprite/tutorial.png");

	// シーン切り替えエフェクト読み込み
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

	sceneChange = Audio::Instance().LoadAudioSource("Data/Sound/SceneChange.wav");
	BGM = Audio::Instance().LoadAudioSource("Data/Audio/01.wav");
	exit_voice = Audio::Instance().LoadAudioSource("Data/Audio/Exit Voice.wav");
	exit_voice_kako = Audio::Instance().LoadAudioSource("Data/Audio/Exit voice kako.wav");
	select = Audio::Instance().LoadAudioSource("Data/Audio/choice.wav");
	select->SetVolume(0.5f);
	decide = Audio::Instance().LoadAudioSource("Data/Audio/dicide.wav");
	decide->SetVolume(0.2f);
	sceneChange->DC_Play();
	sceneInEffect->Play({ 0, 75, -8.5f }, 1.0f);
	BGM->SetVolume(0.7f);
	BGM->Play(true);
}

//終了化
void SceneTitle::Finalize()
{
	//スプライト終了化
	if (title != nullptr)
	{
		delete title;
		title = nullptr;
	}

	// エフェクト終了化
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

	if (exit != nullptr)
	{
		delete exit;
		exit = nullptr;
	}

	if (howTo != nullptr)
	{
		delete howTo;
		howTo = nullptr;
	}

	if (play != nullptr)
	{
		delete play;
		play = nullptr;
	}

	if (selecter != nullptr)
	{
		delete selecter;
		selecter = nullptr;
	}

	if (tutorial != nullptr)
	{
		delete tutorial;
		tutorial = nullptr;
	}
}


//更新処理
void SceneTitle::Update(float elapsedTime)
{
	if (effect) {
		if (!sceneEndEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
		{
			Scene_Change();
		}
	}

	if (finish)
	{
		Finish_Update();
	}
	else if (!isHowTo)
	{
		Update_Select_Timer(elapsedTime);
		Input_Select();
		Updata_Selecter_Pos();
		Decide();
		Select_Anime(elapsedTime);
	}
	else if (isHowTo)
	{
		//if (tutorialScale < 1.0f && !scaleDown)
		//{
		//	tutorialScale += 0.01f;
		//}

		//if (scaleDown)
		//{
		//	tutorialScale -= 0.01f;
		//}
		//if(tutorialScale < 0.0f)
		//{
		//	scaleDown = false;
		//	isHowTo = false;
		//}
		GamePad& gamePad = Input::Instance().GetGamePad();
		if (gamePad.GetButtonDown() & GamePad::BTN_START && tutorialScale >= 1.0f)
		{
			scaleDown = true;
			isHowTo = false;
		}
	}

	//エフェクト更新処理
	EffectManager::Instance().Update(elapsedTime);
}

//描画処理
void SceneTitle::Render()
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
		float textureWidth = static_cast<float>(title->GetTextureWidth());
		float textureHeight = static_cast<float>(title->GetTextureHeight());

		//タイトルスプライト
		title->Render(dc,
			0, 0, screenWidth, screenHeight,
			0, 0, textureWidth, textureHeight,
			0,
			1, 1, 1, 1
		);

		play->Render(dc,
			play_pos.x, play_pos.y, 200, 60,
			0, 0, 200, 60,
			0, 1, 1, 1, 1);

		howTo->Render(dc,
			howto_pos.x, howto_pos.y, 300, 60,
			0, 0, 300, 60,
			0, 1, 1, 1, 1);

		exit->Render(dc,
			exit_pos.x, exit_pos.y, 200, 60,
			0, 0, 200, 60,
			0, 1, 1, 1, 1);

		selecter->Render(dc,
			max_pos.x * shifft_pos, max_pos.y, tex.x, 5,
			0, 0, tex.x, 5,
			0, 0.9f, 0.9f, 0.9f, 0.7f
		);

		if (isHowTo)
		{
			tutorial->Render(dc,
				0, 0, screenWidth, screenHeight,
				0, 0, textureWidth, textureHeight * tutorialScale,
				0,
				1, 1, 1, 1
			);
		}
	};

	//3Dエフェクト描画
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}

}


void SceneTitle::Input_Select()
{
	//入力情報を取得 
	GamePad& gamePad = Input::Instance().GetGamePad();
	float ay = gamePad.GetAxisLY();
	if (ay == 0.0f)return;
	else if (ay < 0.0f && select_timer < 0.0f)
	{
		select->DC_Play();
		select_state++;
		tex.x = 0.0f;
		shifft_pos = 1.5;
	}
	else if (ay > 0.0f && select_timer < 0.0f)
	{
		select->DC_Play();
		select_state--;
		tex.x = 0.0f;
		shifft_pos = 1.5;
	}
	
	if (select_state < 0)select_state = MAX_ACTION - 1;
	if (select_state > MAX_ACTION - 1)select_state = 0;
}

void SceneTitle::Update_Select_Timer(float elapsedTime)
{
	if (select_timer > 0.0f) {
		select_timer -= elapsedTime;
	}
	else
	{
		select_timer = 0.3f;
	}
}

void SceneTitle::Updata_Selecter_Pos()
{
	switch (select_state)
	{
	case State::PLAY:

		max_pos = { play_pos.x ,play_pos.y + Get_Play_Tex().y };
		if (tex.x > Get_Play_Tex().x )tex.x = Get_Play_Tex().x ;
		break;
	case State::HOW_TO:

		max_pos = { howto_pos.x,howto_pos.y + Get_How_To_Tex().y };
		if (tex.x > Get_How_To_Tex().x)tex.x = Get_How_To_Tex().x;
		break;
	case State::EXIT:

		max_pos = { exit_pos.x ,exit_pos.y + Get_Exit_Tex().y };
		if (tex.x > Get_Exit_Tex().x )tex.x = Get_Exit_Tex().x ;
		break;
	}
	if (shifft_pos < 1.0f) shifft_pos = 1.0f;
}

void SceneTitle::Select_Anime(float elapsedTime)
{
	shifft_pos -= elapsedTime;
	tex.x += elapsedTime * anime_speed * anime_speed;
}

void SceneTitle::Decide()
{
	GamePad& gamePad = Input::Instance().GetGamePad();
	if (gamePad.GetButtonDown() & GamePad::BTN_START) {
		decide->DC_Play();
		switch (select_state)
		{
		case State::PLAY:
			effect = true;
			sceneEndEffect->Play({ 0, 75, -8.5f }, 1.0f);
			sceneChange->DC_Play();

			break;

		case State::HOW_TO:
			isHowTo = true;
			break;

		case State::EXIT:
			finish = true;
			break;
		}
	}
}

void SceneTitle::Scene_Change()
{
	effect = true;
	SceneManager::Instance().ChangeScene(new SceneLoading(new SceneGame));
}

void SceneTitle::Finish_Update()
{

	if (isExit)
	{
		if (!sceneEndEffect->IsPlay(sceneEndEffect->Get_Last_Handle()))
		{
			HANDLE hProcess = GetCurrentProcess();
			TerminateProcess(hProcess, 0);
		}
	}
	else
	{
		if (use_finish_sound) {
			bool flag = exit_voice->IsPlay()||exit_voice_kako->IsPlay();
			if (!exit_voice->IsPlay()) {
				isExit = true;
				sceneEndEffect->Play({ 0, 75, -8.5f }, 1.0f);
				sceneChange->DC_Play();
				use_finish_sound = false;
			}
		}
		else {
			int rnd = rand() % 2;
			switch (rnd) {
			case 0:
				exit_voice->DC_Play();
				break;
			case 1:
				exit_voice_kako->DC_Play();
			}
			use_finish_sound = true;
		}
	}

}