#include"Graphics/Graphics.h"
#include"Effect.h"
#include"EffectManager.h"

//コンストラクタ
Effect::Effect(const char* filename)
{
	//別スレッド中にデバイスコンテキストが使われた場合に
	//同時アクセスしないように排他制御する
	std::lock_guard<std::mutex>lock(Graphics::Instance().GetMutex());

	//Effekseerのリソースを読み込む
	//EffekseerはUTF-16のファイルパス以外は対応してないため文字コード変換が必要
	char16_t utf16FIlename[256];
	Effekseer::ConvertUtf8ToUtf16(utf16FIlename, 256, filename);

	//Effekseer::Managerを取得
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();

	//Effekseerエフェクトを読み込む
	effekseerEffect = Effekseer::Effect::Create(effekseerManager, (EFK_CHAR*)utf16FIlename);
}

Effect::~Effect()
{
	handle_Manager.clear();
}

//再生
Effekseer::Handle Effect::Play(const DirectX::XMFLOAT3& position, float scale)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	Effekseer::Handle handle = effekseerManager->Play(effekseerEffect, position.x, position.y, position.z);
	handle_Manager.push_back(handle);
	effekseerManager->SetScale(handle, scale, scale, scale);
	return handle;
}

//停止
void Effect::Stop(Effekseer::Handle handle)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	effekseerManager->StopEffect(handle);
}


bool Effect::IsPlay(Effekseer::Handle handle)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	return effekseerManager->Exists(handle);
}

//座標設定
void Effect::SetPosition(Effekseer::Handle handle, const DirectX::XMFLOAT3& position)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	effekseerManager->SetLocation(handle, position.x, position.y, position.z);
}

//スケール設定
void Effect::SetScale(Effekseer::Handle handle, const DirectX::XMFLOAT3& scale)
{
	Effekseer::ManagerRef effekseerManager = EffectManager::Instance().GetEffekseerManager();
	effekseerManager->SetScale(handle, scale.x, scale.y, scale.z);
}

Effekseer::Handle  Effect::Get_Last_Handle()
{
	if (!handle_Manager.empty()) {
		return handle_Manager.back();
	}
}