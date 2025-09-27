#include"ProjectileManager.h"
#include"Graphics/Graphics.h"
#include <imgui.h>

//コンストラクタ
ProjectileManager::ProjectileManager() {
}

//デストラクタ
ProjectileManager::~ProjectileManager() {
	Clear();
}

//更新処理
void ProjectileManager::Update(float elapsedTime) {
	//更新処理
	for (auto& projectiles : projectiles) {
		projectiles->Update(elapsedTime);
	}
	//破棄処理
	//*Projectilesの範囲for文中でerase()すると不具合が発生してしまうため
	//更新処理が終わったあとに破棄リストに積まれたオブジェクトを破棄する
	for (Projectile* projectile : remove) {
		//std::vectorから要素を削除する場合はイテレーターを使わなければならない
		std::vector<Projectile*>::iterator it = std::find(projectiles.begin(), projectiles.end(), projectile);
		if (it != projectiles.end()) {
			projectiles.erase(it);
		}
		//弾丸の破棄処理
		delete projectile;
	}
	//破棄リストをクリア
	remove.clear();
}

//描画処理
void ProjectileManager::Render(ID3D11DeviceContext* dc, Shader* sheder) {
	for (auto& projectiles : projectiles) {
		if (!projectiles->CheckDraw()) continue;
		projectiles->Render(dc, sheder);
	}
}

//デバックプリミティブ描画
void ProjectileManager::DrawDebugPrimitive() {
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	for (auto& projectiles : projectiles) {
		debugRenderer->DrawSphere(projectiles->GetPosition(),0.5, DirectX::XMFLOAT4(0, 0, 0, 1));
	}
}

// デバッグGUI描画
void ProjectileManager::DrawDebugGui()
{
	if (ImGui::CollapsingHeader("ProjectileManager"))
	{
		int index = 0;
		for (Projectile* projectile : projectiles)
		{
			char name[32];
			::sprintf_s(name, sizeof(name), "%d", index);

			ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::TreeNodeEx(projectile, node_flags, name))
			{
				ImGui::PushID(index);
				projectile->DrawDebugGui();
				ImGui::PopID();
				ImGui::TreePop();
			}
			index++;
		}
	}
}

//弾丸登録
void ProjectileManager::Register(Projectile* projectile) {
	projectiles.push_back(projectile);
}

//弾丸全削除
void ProjectileManager::Clear() {
	for (Projectile* projectile : projectiles)
	{
		delete projectile;
	}
	projectiles.clear();
}

//弾丸削除
void ProjectileManager::Remove(Projectile* projectile) {
	//破棄リストに追加
	remove.insert(projectile);
}