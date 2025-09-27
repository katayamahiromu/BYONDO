#pragma once
#include"Graphics//Sprite.h"
#include"Audio/Audio.h"
#include"Scene.h"
#include"Effect.h"
#include"CameraController.h"
#include<memory>

//タイトルシーン
class SceneTitle :public Scene
{
public:
	SceneTitle() {};
	~SceneTitle() {};

	//初期化
	virtual void Initialize() override;

	//終了化
	virtual void Finalize() override;

	//更新処理
	virtual void Update(float elapsedTime) override;

	//描画処理
	virtual void Render() override;
private:
	void Input_Select();
	void Update_Select_Timer(float elapsedTime);
	void Updata_Selecter_Pos();
	void Select_Anime(float elapsedTime);
	void Decide();
	void Scene_Change();
	void Finish_Update();
private:
	Sprite* title = nullptr;
	Sprite* exit = nullptr;
	Sprite* howTo = nullptr;
	Sprite* play = nullptr;
	Sprite* selecter = nullptr;
	Sprite* tutorial = nullptr;
private:
	DirectX::XMFLOAT2 pos = { 0,0 };
	DirectX::XMFLOAT2 tex = { 0,0 };
	DirectX::XMFLOAT2 max_pos = { 0,0 };
	float shifft_pos = 1.5f;
	float anime_speed = 20.0;
private:
	inline DirectX::XMFLOAT2 Get_Play_Tex() { return DirectX::XMFLOAT2(play->GetTextureWidth(), play->GetTextureHeight()); }
	inline DirectX::XMFLOAT2 Get_How_To_Tex() { return DirectX::XMFLOAT2(howTo->GetTextureWidth(), howTo->GetTextureHeight()); }
	inline DirectX::XMFLOAT2 Get_Exit_Tex() { return DirectX::XMFLOAT2(exit->GetTextureWidth(), exit->GetTextureHeight()); }
private:
	const DirectX::XMFLOAT2 play_pos = { 200, 350 };
	const DirectX::XMFLOAT2 howto_pos = { 150, 450 };
	const DirectX::XMFLOAT2 exit_pos = { 200, 550 };
	float select_timer = 0.3f;
	enum State
	{
		PLAY,
		HOW_TO,
		EXIT
	};
	int select_state = State::PLAY;
	static const int MAX_ACTION = 3;
	//Effect* sceneChangeEffect = nullptr;
	CameraController* cameraController = nullptr;
	bool effect = false;
	bool isExit = false;
	bool isHowTo = false;
	float tutorialScale = 1.0f;
	bool scaleDown = false;

private:
	std::unique_ptr<AudioSource> sceneChange;
	bool finish = false;
	bool use_finish_sound = false;
	std::unique_ptr<AudioSource> exit_voice;
	std::unique_ptr<AudioSource> exit_voice_kako;
	std::unique_ptr<AudioSource> BGM;
	std::unique_ptr<AudioSource> select;
	std::unique_ptr<AudioSource> decide;
};