#include"Graphics/Graphics.h"
#include"Input/Input.h"
#include"SceneLoading.h"
#include"SceneManager.h"
#include"SceneGame.h"
#include"Camera.h"
#include"EffectManager.h"

//������
void SceneLoading::Initialize()
{
	srand((unsigned)time(NULL));
	random = rand() % 3;
	//�X�v���C�g�̏�����
	//�X���b�h�J�n
	thread = new std::thread(SceneLoading::LoadingThread, this);
	// �V�[���؂�ւ��G�t�F�N�g�ǂݍ���
	sceneInEffect = new Effect("Data/Effect/Scene_in.efk");
	sceneEndEffect = new Effect("Data/Effect/Scene_end.efk");

	// �J�����R���g���[���[������
	cameraController = new CameraController;
	cameraController->SetTarget(DirectX::XMFLOAT3(0, 30, 0));

	//�J���������ݒ�
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

//�I����
void SceneLoading::Finalize()
{
	//�X���b�h�I����
	thread->join();
	if (thread != nullptr)
	{
		delete thread;
		thread = nullptr;
	}

	//�G�t�F�N�g�I����
	delete sceneInEffect;
	delete sceneEndEffect;

	// �J�����I����
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
	
	// ���[�f�B���O���ɃQ�[���𗎂Ƃ����Ƃ��A���̃V�[���̏I����������
	if (SceneManager::Instance().GetCurrentScene() == this
		&& SceneManager::Instance().GetNextScene() == nullptr)
	{
		nextScene->Finalize();
		delete nextScene;
		nextScene = nullptr;
	}
}

//�X�V����
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

//�`�揈��
void SceneLoading::Render()
{
	Graphics& graphics = Graphics::Instance();
	ID3D11DeviceContext* dc = graphics.GetDeviceContext();
	ID3D11RenderTargetView* rtv = graphics.GetRenderTargetView();
	ID3D11DepthStencilView* dsv = graphics.GetDepthStencilView();

	//��ʃN���A���^�[�Q�b�g�ݒ�
	FLOAT color[] = { 0.0f,0.0f,0.5f,1.0f };
	dc->ClearRenderTargetView(rtv, color);
	dc->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dc->OMSetRenderTargets(1, &rtv, dsv);

	// �`�揈��
	RenderContext rc;
	rc.lightDirection = { 0.0f, -1.0f, 0.0f, 0.0f };	// ���C�g�����i�������j

	//�J�����̃p�����[�^�[�ݒ�
	Camera& camera = Camera::Instance();
	rc.view = camera.GetView();
	rc.projection = camera.GetProjection();

	//2D�X�v���C�g�`��
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

	//3D�G�t�F�N�g�`��
	{
		EffectManager::Instance().Render(rc.view, rc.projection);
	}
}

void SceneLoading::LoadingThread(SceneLoading* scene)
{
	//COM�֘A�̏������ŃX���b�h���ɌĂԕK�v������
	CoInitialize(nullptr);

	// ���̃V�[���̏��������s��
	scene->nextScene->Initialize();

	//�X���b�h���I���O��COM�֘A�̏I����
	CoUninitialize();

	//���̃V�[���̏��������ݒ�
	scene->nextScene->SetReady();
}