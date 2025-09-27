#include"playerManager.h"
#include"Graphics/Graphics.h"
#include <algorithm> 


void PlayerManager::Updata(float elapsedTime)
{
	Players[0]->Update(elapsedTime);

	Players[1]->SetPosition(Players[0]->GetPosition());
}

void PlayerManager::Render(ID3D11DeviceContext* dc, Shader* shader)
{
	Players[0]->CheckDraw();
	Players[0]->Render(dc, shader);
}

void PlayerManager::Clear()
{
	for (Player* player : Players) {
		delete player;
	}
	Players.clear();
}

void PlayerManager::Register(Player* play)
{
	Players.emplace_back(play);
}

void PlayerManager::DrawDebugGui()
{
	if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen)) {
		//トランスフォーム
		if (ImGui::TreeNode("Tranceform"))
		{
			//位置
			DirectX::XMFLOAT3& pos = const_cast<DirectX::XMFLOAT3&>(Players[0]->GetPosition());
			ImGui::InputFloat3("Position", &pos.x);
			//回転
			DirectX::XMFLOAT3 a;
			a.x = DirectX::XMConvertToDegrees(Players[0]->GetAngle().x);
			a.y = DirectX::XMConvertToDegrees(Players[0]->GetAngle().y);
			a.z = DirectX::XMConvertToDegrees(Players[0]->GetAngle().z);
			ImGui::InputFloat3("Angle", &a.x);
			DirectX::XMFLOAT3& angle = const_cast<DirectX::XMFLOAT3&>(Players[0]->GetAngle());
			angle.x = DirectX::XMConvertToRadians(a.x);
			angle.y = DirectX::XMConvertToRadians(a.y);
			angle.z = DirectX::XMConvertToRadians(a.z);
			//スケール
			DirectX::XMFLOAT3& scale = const_cast<DirectX::XMFLOAT3&>(Players[0]->GetScale());
			ImGui::InputFloat3("Scale", &scale.x);

			ImGui::TreePop();
		}
		if (ImGui::TreeNodeEx("others"))
		{
			int health = Players[0]->GetHealth();
			ImGui::InputInt("Health", &health);
			Players[0]->projectileManager.DrawDebugGui();
			Players[1]->projectileManager.DrawDebugGui();

			ImGui::TreePop();
		}
	}
}

void PlayerManager::DrawDebugPrimitive()
{
	DebugRenderer* debugRenderer = Graphics::Instance().GetDebugRenderer();
	debugRenderer->DrawCylinder(Players[0]->GetPosition(), 
		Players[0]->GetRadius(), Players[0]->GetHeight(), DirectX::XMFLOAT4(0, 0, 0, 1));
}

void PlayerManager::Player_Swap()
{
	Players[1]->SetPosition(Players[0]->GetPosition());
	Players[1]->SetAngle(Players[0]->GetAngle());
	Players[1]->SetScale(Players[0]->GetScale());
	Players[1]->SetHealth(Players[0]->GetHealth());
	std::swap(Players[0], Players[1]);
}